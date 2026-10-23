// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/items/objects/grabber.h"

#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

#include <string.h>
#include "decomp.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/light/surfaces.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/motion.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/rtl.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/render/fx.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nurndr.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/fx/explosions.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/light/shadow.h"

GRABBER_s *Grab_grabber;

static void Grab_invert_x(nufpar_s *);
static void Grab_move_xy(nufpar_s *parser);
static void Grab_radius(nufpar_s *parser);
static void Grab_rotate(nufpar_s *parser);
static void Grab_scale(nufpar_s *parser);
static void Grab_shadow(nufpar_s *parser);
static void Grab_speed(nufpar_s *parser);
static void Grab_type(nufpar_s *parser);

static NUFPCOMJMP Grabber_ConfigKeywords[] = {
    {"radius", Grab_radius},     {"speed", Grab_speed},     {"rotate", Grab_rotate},
    {"scale", Grab_scale},       {"move_xy", Grab_move_xy}, {"type", Grab_type},
    {"invert_x", Grab_invert_x}, {"shadow", Grab_shadow},   {NULL, NULL}};

extern "C++" float FRAMETIME;

// Static grabber victim-pos helper. Moved from gizmisc_stubs.cpp.

extern ADDPART_s Default_ADDPART;
// Retail only has the IPA-SRA clone of this function.
static __attribute__((noinline, noclone)) void Grabber_Drop(GRABBER_s *grabber, NUVEC *previous_position)
    __asm__("_ZL12Grabber_DropP9GRABBER_sP7nuvec_s.isra.4");

// ===========================================================================
// Episode bookkeeping
// ===========================================================================

extern i32 obstacle_gizmotype_id;

static void Grab_invert_x(nufpar_s *) {
    Grab_grabber->invert_x = 1;
}

static i32 IsGrabbable(GameObject_s *object) {
    return (object->apiobj.character_data->game_character->flags_090 & GAMECHARACTER_FLAG_GRAB_DISABLED) == 0;
}

void PartKill_Grabber(PART_s *part, i32) {
    GIZMOBLOWUP_s *blowup = part->grabber_blowup;
    NUVEC *position = &part->position;
    PlaySfx("Explode2", position);
    GameCam_Judder(GameCam, 0.5f, 0, position);
    if (blowup->type->particle_types[4] != -1)
        AddGameDebris(WORLD->debris_sys, blowup->type->particle_types[4], position);
    if (blowup->type->particle_types[5] != -1)
        AddGameDebris(WORLD->debris_sys, blowup->type->particle_types[5], position);
    if (blowup->type->particle_types[6] != -1)
        AddGameDebris(WORLD->debris_sys, blowup->type->particle_types[6], position);
    if (blowup->type->particle_types[0] != -1)
        AddFiniteShotPART(blowup->type->particle_types[0], position, 1);
    if (blowup->type->particle_types[1] != -1)
        AddFiniteShotPART(blowup->type->particle_types[1], position, 1);
    AddExplosion(position, 0.6f, 0.3f, NULL, -1, 0x27);
    GameCam_NewShake(GameCam, 0.5f, 0.5f, 1.0f);
    NewRumbleAllPlayers(0.7f, 0.0f, 0, 0);
}

static void Grab_shadow(nufpar_s *parser) {
    Grab_grabber->flags_559 |= 0x10;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, const_cast<char *>("off")) == 0) {
        Grab_grabber->flags_559 &= ~0x10;
    }
}

static void Grab_move_xy(nufpar_s *parser) {
    if (NuFParGetInt(parser) != 0) {
        Grab_grabber->move_xy = 1;
    }
}

static void Grab_rotate(nufpar_s *parser) {
    GRABBER_s *grabber = Grab_grabber;
    i32 rotate = NuFParGetInt(parser) & 1;
    grabber->flags_559 = (grabber->flags_559 & ~0x08) | (rotate << 3);
}

static void Grab_scale(nufpar_s *parser) {
    Grab_grabber->scale = NuFParGetFloat(parser);
}

static void Grab_speed(nufpar_s *parser) {
    Grab_grabber->speed = NuFParGetFloat(parser);
}

static void Grab_radius(nufpar_s *parser) {
    Grab_grabber->radius = NuFParGetFloat(parser);
}

static void Grab_type(nufpar_s *parser) {
    if (NuFParGetWord(parser) == 0) {
        return;
    }
    if (NuStrICmp(parser->word_buf, const_cast<char *>("GRABMAGNET")) == 0) {
        Grab_grabber->character_id = id_GRABMAGNET;
    } else if (NuStrICmp(parser->word_buf, const_cast<char *>("ROBOTBASE")) == 0) {
        Grab_grabber->character_id = id_ROBOTBASE;
    } else {
        Grab_grabber->character_id = id_GRABMACHINE;
    }
}

void Grabber_Configure(WORLDINFO_s *world, char *config) {
    if (VehicleArea != 0 || (world->current_level->flags & 0xe0) != 0 || world->grabber != NULL) {
        return;
    }

    nuhspecial_s special;
    if (NuSpecialFind(world->current_gscn, &special, "grabber", 1) == 0) {
        return;
    }

    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 16);
    GRABBER_s *grabber = reinterpret_cast<GRABBER_s *>(world->giz_buffer.addr);
    world->grabber = grabber;
    Grab_grabber = grabber;
    world->giz_buffer.addr += sizeof(*grabber);
    Grab_grabber->special = special;

    NUFPAR *parser = NuFParCreateMem("grabbers", config, 0xffff);
    if (parser == NULL) {
        return;
    }

    NuFParPushCom(parser, Grabber_ConfigKeywords);
    u8 active = 0;
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) != 0) {
            if (NuStrICmp(parser->word_buf, "grabber_start") != 0) {
                if (NuStrICmp(parser->word_buf, "grabber_end") == 0) {
                    break;
                } else if (active != 0) {
                    NuFParInterpretWord(parser);
                }
            } else {
                Grab_grabber->radius = 0.5f;
                Grab_grabber->move_xy = 0;
                Grab_grabber->flags_559 = (Grab_grabber->flags_559 & ~0x08) | 0x10;
                Grab_grabber->invert_x = 0;
                Grab_grabber->speed = 1.0f;
                Grab_grabber->character_id = id_GRABMACHINE;
                Grab_grabber->scale = 1.0f;
                active = 1;
            }
        }
    }
    NuFParDestroy(parser);

    if (Grab_grabber->character_id != -1) {
        Grab_grabber->character_model = APICharacterLoaded(Grab_grabber->character_id);
    } else {
        Grab_grabber->character_model = NULL;
    }

    if (Grab_grabber->character_model == NULL) {
        NuSpecialSetVisibility(&Grab_grabber->special, 1);
        Grab_grabber->character_id = -1;
    } else {
        NuSpecialSetVisibility(&Grab_grabber->special, 0);
    }

    Grab_grabber->matrix = *NuSpecialGetDrawMtx(&Grab_grabber->special);
    if (world->current_level == DEATHSTARRESCUEA_LDATA) {
        Grab_grabber->matrix.m31 -= 0.3f;
    }

    memmove(&Grab_grabber->initial_position, &Grab_grabber->position, sizeof(Grab_grabber->position));
    NuSpecialFind(world->current_gscn, &Grab_grabber->shadow_special, "grabber_shadow", 1);
    Grab_grabber->platform_id = -1;
}

// Original 0x22aae0, 264 bytes.
NUVEC *Grabber_GetGrabPos(GRABBER_s *grabber, numtx_s *matrix) {
    if (grabber->character_model != NULL && (grabber->flags_559 & 1) != 0 &&
        grabber->character_model->points_of_interest[0] != NULL) {
        if (matrix != NULL)
            *matrix = grabber->grab_matrix;
        return reinterpret_cast<NUVEC *>(&grabber->grab_matrix.m30);
    }
    if (matrix != NULL)
        *matrix = numtx_identity;
    return &grabber->grab_position;
}

static void Grabber_SetVictimPos(GRABBER_s *grabber) {
    if (grabber->victim != NULL) {
        NUVEC position = *Grabber_GetGrabPos(grabber, NULL);
        GameObject_s *victim = grabber->victim;
        if (grabber->character_id == id_GRABMAGNET) {
            f32 fraction = victim->context_animation_timer / victim->airborne_action_duration;
            if (fraction > 1.0f)
                fraction = 1.0f;
            NUVEC start = victim->external_force;
            victim->apiobj.position.x = start.x + (position.x - start.x) * fraction;
            victim->apiobj.position.y = start.y + (position.y - start.y) * fraction;
            victim->apiobj.position.z = start.z + (position.z - start.z) * fraction;
            victim->apiobj.position.x -= NU_SIN_LUT(victim->apiobj.field_0x276) * victim->apiobj.field_0x1e0 * fraction;
            victim->apiobj.position.z -=
                NU_SIN_LUT(victim->apiobj.field_0x276 + 0x4000) * victim->apiobj.field_0x1e0 * fraction;
            if (victim->field_0x7a3 == 0 && fraction == 1.0f) {
                PlaySfx("imp_C3PO_magnet", &position);
                grabber->victim->field_0x7a3 = 1;
                victim = grabber->victim;
            }
        } else if (victim->context_animation == 5) {
            victim->apiobj.position.x =
                position.x - NU_SIN_LUT(victim->apiobj.field_0x276) * victim->apiobj.field_0x1e0;
            victim->apiobj.position.y = position.y;
            victim->apiobj.position.z =
                position.z - NU_SIN_LUT(victim->apiobj.field_0x276 + 0x4000) * victim->apiobj.field_0x1e0;
        } else {
            victim->apiobj.position = position;
        }
        victim->touched_ground = 0;
        grabber->victim->saved_position = grabber->victim->apiobj.position;
    }
}

void Grabber_Reset(WORLDINFO_s *world) {
    GRABBER_s *g = world->grabber;
    if (g == NULL)
        return;
    GIZMO *handle;
    handle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, (char *)"pad_drop");
    if (handle)
        Grab_grabber->action_switch = (GIZOBSTACLE_s *)handle->object;
    else
        Grab_grabber->action_switch = NULL;
    if (Grab_grabber->action_switch && !Grab_grabber->action_switch->anim_set)
        Grab_grabber->action_switch = NULL;
    handle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, (char *)"pad_n");
    Grab_grabber->direction_switches[0] = handle ? (GIZOBSTACLE_s *)handle->object : NULL;
    if (__builtin_expect(Grab_grabber->direction_switches[0] && !Grab_grabber->direction_switches[0]->anim_set, 1))
        Grab_grabber->direction_switches[0] = NULL;
    handle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, (char *)"pad_s");
    if (handle)
        Grab_grabber->direction_switches[1] = (GIZOBSTACLE_s *)handle->object;
    else
        Grab_grabber->direction_switches[1] = NULL;
    if (Grab_grabber->direction_switches[1] && !Grab_grabber->direction_switches[1]->anim_set)
        Grab_grabber->direction_switches[1] = NULL;
    handle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, (char *)"pad_e");
    Grab_grabber->direction_switches[2] = handle ? (GIZOBSTACLE_s *)handle->object : NULL;
    if (Grab_grabber->direction_switches[2] && !Grab_grabber->direction_switches[2]->anim_set)
        Grab_grabber->direction_switches[2] = NULL;
    handle = GizmoFindByName(world->gizmo_sys, obstacle_gizmotype_id, (char *)"pad_w");
    Grab_grabber->direction_switches[3] = handle ? (GIZOBSTACLE_s *)handle->object : NULL;
    if (Grab_grabber->direction_switches[3] && !Grab_grabber->direction_switches[3]->anim_set)
        Grab_grabber->direction_switches[3] = NULL;
    g->grab_position.y = g->initial_position.y;
    LEVEL_PROGRESS_s *progress = world->level_progress;
    if (progress && progress->grabber_field_0x484 != 2000000.0f) {
        g->grab_position.x = progress->grabber_field_0x48c;
        g->grab_position.z = progress->grabber_field_0x494;
    } else {
        g->grab_position.x = g->initial_position.x;
        g->grab_position.z = g->initial_position.z;
    }
    g->shadow_height = 2000000.0f;
    g->floor_height = 2000000.0f;
    ResetAnimPacket(&g->animation, -1);
    g->state = 0;
    if (g->victim) {
        Grabber_SetVictimPos(g);
        g->victim->field_0x7a5 = 0xff;
    }
    g->victim = NULL;
    g->carried_blowup = NULL;
    NUVEC_ALIGNED16 query = g->grab_position;
    query.y += 0.25f;
    if (WORLD->current_level != CLOUDCITYTRAPA_LDATA)
        NewTerrPlatformsOff();
    f32 height = GameShadow(NULL, &query, 5.0f, -1);
    if (height != 2000000.0f) {
        i32 surface = ShadowInfo();
        if (__builtin_expect((u32)surface <= 31 && (TerSurface[surface].flags & 0x200), 1)) {
            g->shadow_height = height;
            FindAnglesZX(&ShadNorm, &g->shadow_z_angle, &g->shadow_x_angle);
            g->floor_height = (TerSurface[surface].flags & 2) ? height : 2000000.0f;
        }
    }
    g->flags_559 &= 0xfa;
    g->target_yaw = 0;
    g->yaw = 0;
    g->velocity.x = 0;
    g->target_velocity.z = g->target_velocity.y = g->target_velocity.x = 0;
    g->velocity.y = g->velocity.z = 0;
    g->platform_contact_timer = 0;
    if (world->terrain)
        g->platform_id = FindPlatInst(NuSpecialGetInstanceix(&g->special));
}

static void Grabber_Drop(GRABBER_s *grabber, NUVEC *previous_position) {
    grabber->flags_559 |= 2;
    if (grabber->victim != NULL) {
        if (grabber->character_id == id_GRABMAGNET && grabber->victim->field_0x7a3 != 0)
            PlaySfx("imp_C3PO_magnet_drop", Grabber_GetGrabPos(grabber, NULL));
        Grabber_SetVictimPos(grabber);
        GameObject_s *victim = grabber->victim;
        victim->apiobj.velocity.x = ((grabber->grab_position.x - previous_position->x) / FRAMETIME) * 0.5f;
        if (grabber->character_id == id_GRABMAGNET)
            victim->apiobj.velocity.y = -1.0f;
        victim->apiobj.velocity.z = ((grabber->grab_position.z - previous_position->z) / FRAMETIME) * 0.5f;
        victim->field_0x7a5 = 0xff;
        grabber->victim = NULL;
    } else if (grabber->carried_blowup != NULL) {
        ADDPART_s params = Default_ADDPART;
        NUMTX matrix = grabber->carried_matrix;
        NUVEC velocity;
        NUVEC position = *Grabber_GetGrabPos(grabber, NULL);
        matrix.m30 = position.x;
        matrix.m31 = position.y + grabber->carried_height_offset;
        matrix.m32 = position.z;
        velocity.x = ((grabber->grab_position.x - previous_position->x) / FRAMETIME) * 0.5f;
        velocity.y = -0.1f;
        velocity.z = ((grabber->grab_position.z - previous_position->z) / FRAMETIME) * 0.5f;
        params.matrix = &matrix;
        params.velocity = &velocity;
        NUVEC center;
        NuSpecialGetRadius(&grabber->carried_blowup->type->special, &center, &params.field_14);
        params.flags = 0x311;
        params.field_18 = params.field_14;
        params.special = &grabber->carried_blowup->type->special;
        params.gravity = -6.0f;
        params.time_step = FRAMETIME;
        params.field_44 = PartKill_Grabber;
        params.field_a4 = 3.0f;
        PART_s *part = AddPart(&params);
        if (part != NULL)
            part->grabber_blowup = grabber->carried_blowup;
        grabber->carried_blowup = NULL;
    }
}

void Grabber_Update(WORLDINFO_s *world) {
    GRABBER_s *g = world->grabber;
    if (g == NULL)
        return;
    if (g->victim != NULL && ((g->victim->apiobj.field_0x1f8 & 0x1001) != 0x1001 || g->victim->apiobj.field_0x287 != 0))
        g->victim = NULL;
    if (g->platform_contact_timer > 0.0f)
        g->platform_contact_timer -= FRAMETIME;
    g->target_velocity.x = g->target_velocity.y = g->target_velocity.z = 0.0f;
    NUVEC next = g->grab_position, previous = g->grab_position;
    GameObject_s *control = FindGameObject(id_GRABCONTROL, 0, 0, 1, 0);
    GAMEPAD_s *pad = NULL;
    if (control == NULL) {
        if (id_GRABR2CONTROL != -1)
            FindGameObject(id_GRABR2CONTROL, 0, 0, 1, 0);
    } else
        pad = control->pad_gamepad;
    if (control != NULL && control->field_0xcc0 != NULL && (control->apiobj.player_controlled)) {
        if (pad->input_magnitude > 0.0f) {
            if (!Grab_grabber->move_xy) {
                u16 angle = GamePad_InputAngle(control, pad);
                g->target_velocity.x = NU_SIN_LUT(angle) * g->speed;
                if (Grab_grabber->invert_x)
                    g->target_velocity.x = -g->target_velocity.x;
                g->target_velocity.z = g->speed * NU_SIN_LUT(angle + 0x4000);
            } else {
                g->target_velocity.y = pad->input_direction_x * g->speed;
                g->target_velocity.x = 0.0f;
                if (!(Grab_grabber->invert_x)) {
                    g->target_velocity.z = g->speed * pad->input_direction_z;
                } else {
                    g->target_velocity.z = -(g->speed * pad->input_direction_z);
                }
            }
        }
    } else {
        bool minus = g->direction_switches[0] && (g->direction_switches[0]->progress_flags & 2) &&
                     g->direction_switches[0]->anim_set->state == 2;
        bool plus = g->direction_switches[1] && (g->direction_switches[1]->progress_flags & 2) &&
                    g->direction_switches[1]->anim_set->state == 2;
        if (minus && !plus)
            g->target_velocity.x = -g->speed;
        else if (plus && !minus)
            g->target_velocity.x = g->speed;
        plus = g->direction_switches[2] && (g->direction_switches[2]->progress_flags & 2) &&
               g->direction_switches[2]->anim_set->state == 2;
        minus = g->direction_switches[3] && (g->direction_switches[3]->progress_flags & 2) &&
                g->direction_switches[3]->anim_set->state == 2;
        if (plus && !minus)
            g->target_velocity.z = g->speed;
        else if (minus && !plus)
            g->target_velocity.z = -g->speed;
        if (!NuSpecialGetVisibilityFn(&g->special))
            g->target_velocity.z = g->target_velocity.x = 0.0f;
    }
    g->velocity.x = SeekValF(g->velocity.x, g->target_velocity.x, 5.0f);
    g->velocity.y = SeekValF(g->velocity.y, g->target_velocity.y, 5.0f);
    g->velocity.z = SeekValF(g->velocity.z, g->target_velocity.z, 5.0f);
    if ((g->flags_559 & 8) && (g->target_velocity.x != 0.0f || g->target_velocity.z != 0.0f))
        g->target_yaw = NuAtan2D(g->target_velocity.x, g->target_velocity.z);
    next.x += FRAMETIME * g->velocity.x;
    next.y += g->velocity.y * FRAMETIME;
    next.z += FRAMETIME * g->velocity.z;
    if (world->current_level != DEATHSTARESCAPEB_LDATA) {
        if (world->current_level == JABBASPALACEB_LDATA) {
            f32 pitch = (NuVecMag(&g->velocity) / g->speed) * 0.25f + 0.5f;
            if (pitch > 1.0f)
                pitch = 1.0f;
            PlaySfxAndSetPitch("env_hover_box_lp", &next, pitch);
            next.y = g->initial_position.y + 0.0125f * NU_SIN_LUT((u16)((NuFmod(GameTimer.time_elapsed, 2.0f) * 0.5f) * 65536.0f));
            g->target_velocity.y = g->velocity.y = 0.0f;
            if (g->platform_contact_timer > 0.0f)
                next.y -=
                    ((1.0f - NU_SIN_LUT((i32)((g->platform_contact_timer * 4.0f) * 65536.0f + 16384.0f))) * 0.5f) *
                    0.02f;
        }
    } else {
        if (next.z >= 11.84f) {
            next.z = 11.84f;
            g->target_velocity.z = g->velocity.z = 0.0f;
        } else if (next.z < 10.125f) {
            next.z = 10.125f;
            g->target_velocity.z = g->velocity.z = 0.0f;
        }
        if (next.y < -4.85f) {
            next.y = -4.85f;
            g->target_velocity.y = g->velocity.y = 0.0f;
        } else if (!(next.y <= -3.9f)) {
            next.y = -3.9f;
            g->target_velocity.y = g->velocity.y = 0.0f;
        }
    }
    i32 moved = 0;
    if (__builtin_expect(next.x != g->grab_position.x || next.y != g->grab_position.y || next.z != g->grab_position.z, 1)) {
        if (__builtin_expect(world->current_level != JABBASPALACEB_LDATA &&
            (g->target_velocity.x != 0.0f || g->target_velocity.y != 0.0f || g->target_velocity.z != 0.0f), 0))
            PlaySfx(const_cast<char *>(world->current_level == CLOUDCITYTRAPA_LDATA ? "CarbonFreezeCraneLp"
                                                                                    : "env_crane_mvt_lp"),
                    &g->grab_position);
        NUVEC query = next;
        query.y += 0.25f;
        if (WORLD->current_level != CLOUDCITYTRAPA_LDATA)
            NewTerrPlatformsOff();
        f32 height = GameShadow(NULL, &query, 5.0f, -1);
        i32 surface = -1;
        if (__builtin_expect(height != 2000000.0f, 1))
            surface = ShadowInfo();
        if (surface >= 0 && surface <= 31 && (TerSurface[surface].flags & 0x200)) {
            g->shadow_height = height;
            FindAnglesZX(&ShadNorm, &g->shadow_z_angle, &g->shadow_x_angle);
            g->floor_height = (TerSurface[surface].flags & 2) ? height : 2000000.0f;
            g->grab_position = next;
            if ((g->target_velocity.x != 0.0f || g->target_velocity.y != 0.0f || g->target_velocity.z != 0.0f) &&
                !Cheat_PowerUpActive(-1)) {
                i32 random = qrand();
                f32 mag = NuVecMag(&g->velocity);
                ConstantRumble(NULL, ((f32)random * 0.000015259021893143654f * 0.4f) * (mag / g->speed), 0.15f);
            }
            moved = 1;
        } else {
            g->target_velocity = v000;
            g->velocity = v000;
            if (__builtin_expect(world->current_level == JABBASPALACEB_LDATA, 1))
                g->grab_position.y = next.y;
        }
    }
    NUMTX matrix;
    if (g->flags_559 & 8)
        g->yaw = SeekRot(g->yaw, g->target_yaw, 3.0f);
    matrix = g->matrix;
    if (__builtin_expect(!!(g->flags_559 & 8), 1))
        NuMtxPreRotateY(&matrix, g->yaw);
    if (__builtin_expect(g->scale != 1.0f, 0)) {
        NUVEC scale = {g->scale, g->scale, g->scale};
        NuMtxPreScale(&matrix, &scale);
    }
    matrix.m30 = g->grab_position.x;
    matrix.m31 = g->grab_position.y;
    matrix.m32 = g->grab_position.z;
    NuSpecialSetDrawMtx(&g->special, &matrix);
    NuSpecialUpdate(&g->special);
    switch (g->state) {
        case 0:
            if (__builtin_expect(pad == NULL, 0)) {
                if ((g->flags_559 & 4) && g->action_switch && (g->action_switch->progress_flags & 2) &&
                    __builtin_expect(GameAnimSet_IsAnimationReset(g->action_switch->anim_set) != 0, 1))
                    g->flags_559 &= ~4;
                if (g->flags_559 & 4)
                    break;
                if (!g->action_switch || !(g->action_switch->progress_flags & 2) ||
                    (g->action_switch->anim_set->flags & 1))
                    break;
            } else {
                if (!(pad->buttons_pressed & (GAMEPAD_SPECIAL | GAMEPAD_ACTION)))
                    break;
            }
            if (!(g->character_model && g->character_model->model_data_b[102])) {
                if (g->character_id == id_GRABMAGNET) {
                    g->victim = FindNearestGameObject(Grabber_GetGrabPos(g, NULL), NULL, 16, 0.5f, 0, -1, -1, -1, NULL,
                                                      1, IsGrabbable, false);
                    if (g->victim) {
                        GameObject_s *v = g->victim;
                        v->character_context = 47;
                        v->context_animation = 104;
                        if (!v->apiobj.character_model->model_data_b[104])
                            v->context_animation = 5;
                        NewRumbleAllPlayers(0, 0.1f, 0, 0);
                        v->airborne_action_duration = 0.75f;
                        v->context_animation_timer = 0;
                        v->external_force = v->apiobj.start_position;
                        v->field_0x7a3 = 0;
                        Hint_SetComplete(651);
                        g->stuck_timer = 0;
                        g->state = 2;
                        if (!control)
                            g->flags_559 |= 4;
                    }
                }
            } else {
                g->state = 1;
                g->animation_timer = AnimDuration(g->character_model->model_id, 102, 0, 0, 1);
                g->flags_559 &= ~2;
                if (__builtin_expect(!control, 0))
                    g->flags_559 |= 4;
                NewRumbleAllPlayers(0, 0, 2, 0);
            }
            PlaySfx("env_grabber_pickup", Grabber_GetGrabPos(g, NULL));
            break;
        case 1: {
            f32 *frame = AnimPlaying(&g->animation, 102, 1, 0);
            if (__builtin_expect(!frame, 0))
                break;
            g->animation_timer -= FRAMETIME;
            if (g->animation_timer <= 0) {
                if (__builtin_expect(!(g->victim || g->carried_blowup), 1))
                    g->state = 0;
                else {
                    g->stuck_timer = 0;
                    g->state = 2;
                }
                break;
            }
            if (g->flags_559 & 2)
                break;
            f32 trigger = AnimListFrame(g->character_model, 102, 0);
            if (__builtin_expect(!(trigger > 0) || !(*frame >= trigger), 1))
                break;
            g->flags_559 |= 2;
            NUVEC *pos = Grabber_GetGrabPos(g, NULL);
            NUVEC low = {pos->x - 0.5f, pos->y - 0.5f, pos->z - 0.5f};
            NUVEC high = {pos->x + 0.5f, pos->y + 0.5f, pos->z + 0.5f};
            GIZMOPICKUP_s *pickup = GizmoPickup_InBox(world, 4, &low, &high);
            if (pickup) {
                pickup->state_flags = (pickup->state_flags & ~6) | 8;
                if (pickup->state_flags & 64)
                    Pup_CollectCoin(world, pickup, 2, control, 1);
                else
                    CollectMinikit(&pickup->position, "m_pup1", 1);
                NewRumbleAllPlayers(0, 0.1f, 0, 0);
                Hint_SetComplete(651);
                PlaySfx("env_grabber_down", pos);
            } else {
                g->victim = FindNearestGameObject(pos, NULL, 0, 0.5f, 0, -1, -1, -1, NULL, 0, IsGrabbable, false);
                if (g->victim) {
                    GameObject_s *v = g->victim;
                    v->character_context = 47;
                    v->context_animation = 104;
                    if (!v->apiobj.character_model->model_data_b[104])
                        v->context_animation = 5;
                    NewRumbleAllPlayers(0, 0.1f, 0, 0);
                    v->context_animation_timer = 0;
                    v->airborne_action_duration = 0.75f;
                    v->external_force = v->apiobj.start_position;
                    v->field_0x7a3 = 0;
                    Hint_SetComplete(651);
                    PlaySfx("env_grabber_down", pos);
                } else {
                    g->carried_blowup = FindNearestGizmoBlowUp(world, pos, 0.25f);
                    if (g->carried_blowup) {
                        g->carried_matrix = g->carried_blowup->transform;
                        GizmoBlowupBlowup(g->carried_blowup, 0, -1, -1, NULL, 1);
                        g->carried_height_offset = g->carried_blowup->transform.m31 - g->carried_blowup->mid_position.y;
                        NewRumbleAllPlayers(0, 0.1f, 0, 0);
                        Hint_SetComplete(651);
                        PlaySfx("env_grabber_down", pos);
                    }
                }
            }
            PlaySfx("env_grabber_up", pos);
            break;
        }
        case 2:
            if (!moved && g->victim && (g->victim->apiobj.player_controlled)) {
                g->stuck_timer += FRAMETIME;
                if (g->stuck_timer >= 3.0f)
                    goto opening;
            } else
                g->stuck_timer = 0;
            if (!control) {
                if ((g->flags_559 & 4) && g->action_switch && (g->action_switch->progress_flags & 2) &&
                    GameAnimSet_IsAnimationReset(g->action_switch->anim_set))
                    g->flags_559 &= ~4;
            }
            if (!g->victim && !g->carried_blowup) {
                g->state = 0;
                break;
            }
            if (control) {
                if (!(pad->buttons_pressed & (GAMEPAD_SPECIAL | GAMEPAD_ACTION)))
                    break;
            } else if ((g->flags_559 & 4) || !g->action_switch || !(g->action_switch->progress_flags & 2) ||
                       (g->action_switch->anim_set->flags & 1))
                break;
        opening:
            if (!(g->character_model && g->character_model->model_data_b[103])) {
                Grabber_Drop(g, &previous);
                if (!control)
                    g->flags_559 |= 4;
            } else {
                g->state = 3;
                g->animation_timer = AnimDuration(g->character_model->model_id, 103, 0, 0, 1);
                g->flags_559 &= ~2;
                if (!control)
                    g->flags_559 |= 4;
                NewRumbleAllPlayers(0, 0, 2, 0);
            }
            PlaySfx("env_grabber_down", Grabber_GetGrabPos(g, NULL));
            break;
        case 3: {
            f32 *frame = AnimPlaying(&g->animation, 103, 1, 0);
            if (!frame)
                break;
            g->animation_timer -= FRAMETIME;
            if (g->animation_timer <= 0) {
                g->state = 0;
                if (__builtin_expect(!(g->flags_559 & 2), 1))
                    Grabber_Drop(g, &previous);
                break;
            }
            if (g->flags_559 & 2)
                break;
            f32 trigger = AnimListFrame(g->character_model, 103, 0);
            if (trigger > 0 && !(*frame < trigger))
                Grabber_Drop(g, &previous);
            break;
        }
    }
    if (__builtin_expect(g->character_model != 0, 1)) {
        g->animation.previous_animation = g->animation.animation_index;
        g->animation.requested_animation = g->state == 1 ? 102 : g->state == 2 ? 15 : g->state == 3 ? 103 : 1;
        UpdateAnimPacket(g->character_model, &g->animation, FRAMETIME * 30.0f, g->speed, FRAMETIME, 0);
    }
    if (g->victim) {
        g->victim->context_animation_timer += FRAMETIME;
        Grabber_SetVictimPos(g);
        GameObject_s *v = g->victim;
        v->apiobj.velocity.x = v->apiobj.velocity.z = 0;
        v->apiobj.velocity.y = -0.1f;
        v->apiobj.field_0x27d = 0;
    }
    if (g->character_id == id_GRABMACHINE) {
        bool found = false;
        i32 mask = 0;
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
            GameObject_s *object = &Obj[i];
            if (__builtin_expect((object->apiobj.field_0x1f8 & 0x1001) != 0x1001, 0))
                continue;
            if (object->apiobj.character_data->game_character->flags_090 & 0x8000)
                found = true;
            else {
                u8 index = object->apiobj.field_0x289;
                mask |= (index & 32) ? 0 : (1u << (index & 31));
            }
        }
        if (found) {
            AIANTINODE_s *node = AIAntinodeCreateSingleFrame(&g->grab_position, 0.25f);
            if (node) {
                node->min_y = node->position.y - 4.0f;
                node->user_data[0] = (u32)mask;
                node->user_data[1] = (u32)(mask >> 31);
            }
        }
    }
}

void Grabber_Draw(WORLDINFO_s *world) {
    GRABBER_s *g = world->grabber;
    if (!g)
        return;
    NUVEC *grab_position = NULL;
    if (g->character_model) {
        grab_position = Grabber_GetGrabPos(g, NULL);
        rtldata_s lights;
        rtlResetEx(&lights, 1);
        rtlApplySetScale(WORLD->rtl_set, &lights, grab_position, NULL, -1, 1.0f);
        rtlSetLights(&lights);
    } else if (!NuSpecialGetVisibilityFn(&g->special))
        return;
    NUMTX matrix __attribute__((aligned(32)));
    memcpy(&matrix, NuSpecialGetDrawMtx(&g->special), sizeof(matrix));
    NUMTX reflection;
    if (world->current_level == BLOCKADERUNNERC_LDATA) {
        f32 distance = NuVecXZDist(&GameCam->pos, (NUVEC *)&matrix.m30, NULL);
        if (distance < 2.0f) {
            f32 fraction = 1.0f - distance * 0.5f;
            f32 wave = NU_SIN_LUT((i32)(fraction * 32768.0f + 16384.0f));
            matrix.m31 += (1.0f - (wave + 1.0f) * 0.5f) * 1.25f;
        }
    }
    if (g->shadow_height != 2000000.0f) {
        if ((g->flags_559 & 0x10) && CHARSHADOWS_ON) {
            NUVEC position;
            position.x = g->grab_position.x;
            position.y = g->shadow_height + 0.005f;
            if (WORLD->current_level == CLOUDCITYTRAPA_LDATA)
                position.y += 0.005f;
            position.z = g->grab_position.z;
            if (NuSpecialExistsFn(&g->shadow_special)) {
                // Inline NuMtxSetRotationZ by the x angle, then NuMtxRotateX by the z angle.
                const NUANG x_angle = g->shadow_x_angle;
                reflection.m00 = reflection.m11 = NU_COS_LUT(x_angle);
                reflection.m01 = NU_SIN_LUT(x_angle);
                reflection.m10 = -reflection.m01;
                reflection.m22 = 1.0f;
                reflection.m02 = reflection.m12 = reflection.m03 = reflection.m23 = reflection.m20 = reflection.m21 =
                    reflection.m13 = reflection.m30 = reflection.m31 = reflection.m32 = 0.0f;
                reflection.m33 = 1.0f;
                NuMtxRotateXInline(&reflection, g->shadow_z_angle);
                NuMtxTranslate(&reflection, &position);
                NuSpecialDrawAt(&g->shadow_special, &reflection);
            } else
                NuRndrAddShadow(&position, g->radius, 128, g->shadow_z_angle, 0, g->shadow_x_angle);
        }
        if (g->floor_height != 2000000.0f)
            MatrixReflectionVU0_AXISY(&matrix, g->floor_height, WORLD->current_level->unknown_0cc, &reflection);
    }
    if (g->character_model) {
        g->flags_559 &= ~1;
        MakeLayerList = GCDataList[g->character_model->model_id].make_layer_list;
        if (g->flags_559 & 0x10) {
            g->character_model->hierarchy->suppress_shadow_surface_points = 0;
            EnableShadowMapRendering(0);
        } else {
            g->character_model->hierarchy->suppress_shadow_surface_points = 1;
            ResetShadowMapRendering();
        }
        NUMTX joints[256];
        if (APIDrawCharacterModel(g->character_model, &CDataList[g->character_model->model_id], &g->animation, &matrix,
                                  NULL, ((u8)Reflections_On && g->floor_height != 2000000.0f) ? &reflection : NULL,
                                  NULL, &g->grab_matrix, NULL, 0xffff, NULL, 0, Paused, FRAMETIME, joints, 0,
                                  WORLD->debris_sys))
            g->flags_559 |= 1;
        if (g->flags_559 & 0x10)
            ResetShadowMapRendering();
        g->character_model->hierarchy->suppress_shadow_surface_points = 0;
    } else if ((u8)Reflections_On && g->floor_height != 2000000.0f)
        NuSpecialDrawAt(&g->special, &reflection);
    if (g->carried_blowup && grab_position) {
        memcpy(&matrix, &g->carried_matrix, sizeof(matrix));
        matrix.m30 = grab_position->x;
        matrix.m31 = grab_position->y;
        matrix.m31 += g->carried_height_offset;
        matrix.m32 = grab_position->z;
        NuSpecialDrawAt(&g->carried_blowup->type->special, &matrix);
        if ((u8)Reflections_On && g->floor_height != 2000000.0f &&
            MatrixReflection(&matrix, 2, g->floor_height, WORLD->current_level->unknown_0cc, &reflection))
            NuSpecialDrawAt(&g->carried_blowup->type->special, &reflection);
    }
}

void Grabber_StoreProgress(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress) {
    if (progress != NULL && world != NULL) {
        GRABBER_s *grabber = world->grabber;
        if (grabber != NULL) {
            progress->grabber_field_0x48c = grabber->field_0x48c;
            progress->grabber_field_0x484 = grabber->field_0x484;
            progress->grabber_field_0x494 = grabber->field_0x494;
        } else {
            progress->grabber_field_0x484 = 2000000.0f;
        }
    }
}

