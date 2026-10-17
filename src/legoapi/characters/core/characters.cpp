#include "nu2api/nu3d/nuportal.h"
#include "decomp.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/actions/character/streaks.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nutex.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/gizmos/object/lever.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/gizmos/trigger/minicut.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/traps/gizturrets.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/gizmos/trigger/giztimer.h"
#include "legoapi/gizmos/trigger/gizrandom.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmos/door/plugs.h"
#include "legoapi/gizmos/door/push.h"
#include "legoapi/gizmos/fx/edgizshadowmachine.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/level.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edstubs.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "legoapi/menus/core/text.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numath/nutrig.h"

static CHARSCENE_s *CharScene_Area;

// LoadPerm1 is one of the few game-level entry points which wires together
// otherwise C-linkage engine subsystems.  Keep these declarations local: the
// individual subsystem TUs intentionally expose their original plain names.
extern "C" {
    i32 rtlInitDynamic(VARIPTR *, VARIPTR, i32);
    void DebrisSetup(VARIPTR *, VARIPTR, char *, i32, i32, i32);
    void DebrisRegisterCutoffCameraVec(void *);
    void edgraSetup(VARIPTR *, VARIPTR, i32, i32, i32);
    void InitParts(i32, VARIPTR *, VARIPTR *);
    void ParticleReset(void);
    i32 NuFileExists(char *);
    i32 NuStrCpy(char *, const char *);
    i32 edppLoadPage(char *, i32, usize);
    void NuRndrShadowInit(u8 *);
    void NuTexAnimProgSysInit(void);
    void terrainpickupinit(char *, void **);

    extern f32 animduration_blendouttime;
    extern i32 Grass_Available;
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern void *perm_debrissys;
}

struct AISYS_s;
void AIScriptLoadAll(char *path, VARIPTR *buf, VARIPTR *buf_end, AISYS_s *sys);
void InitTexAnimScripts(char **names);
void BackDrop_Init(char *path, VARIPTR *buf, VARIPTR *buf_end);

extern i32 PARTPAGE_GENERAL;

// The original table at 0x006281a0.  These are the material animation
// scripts made available before the permanent things scene is loaded.
extern char * TexAnimList_LSW[32];

void InitRipples(ripple_set_s **, VARIPTR *, VARIPTR *, i32);
void CreateFadeMaterials();
void CreateUsefulMaterials();
extern "C" ripple_set_s *ripples;
extern "C" NUMTL *ShadowMat;

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void CharConfig_ConfigureAll(i32 permanent, nufpcomjmp_s *game_keywords);
void ExtraCharacterFixUpAfterConfig();
extern i32 CHARPAK;

extern i32 GetMenuID(void);
extern i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count);
extern i32 Collection_Got(i32 id);
extern void Customiser_SaveModelTextureIDs(CUSTOMISER *customiser, CHARACTERMODEL_s *model);
extern CUSTOMISER *CharacterCustomiser;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;
extern "C" i32 waiting_for_character;
extern f32 WaitingForCharacterTime;

extern "C" {
extern i32 hub_character_ready;
}

extern APICHARACTERMODELLIST_s PermModelList[1];

#include "legoapi/characters/core/characters_types.h"

void LSW_SetIndy(i32);

void HairMovement(GameObject_s *object) {
    constexpr f32 gravity = -1.8325958f;
    u8 current_index;
    u32 state_index;
    u8 *state;

    const u8 index = object->field_0x1089;
    if (index > 2) {
        return;
    }
    if (object->apiobj.character_data->game_character->cloak_joint_2 == -1) {
        return;
    }

    f32 phase = NuFsqrt(*reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(object) + index * 0x34 + 0xf50) / gravity);
    if (LEGOACT_FALL == -1 ||
        (object->apiobj.anim_packet.blending != 0 ? LEGOACT_FALL != object->apiobj.anim_packet.blend_animation_b
                                                  : LEGOACT_FALL != object->apiobj.anim_packet.animation_index)) {
        phase = phase - (FRAMETIME + FRAMETIME);
        if (phase < 0.0f) {
            current_index = object->field_0x1089;
            state_index = current_index;
            state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
            const u8 joint = static_cast<u8>(object->apiobj.character_data->game_character->cloak_joint_2);
            *reinterpret_cast<f32 *>(state + 0xf50) = 0.0f;
            state[0xf78] = joint;
            goto increment_index;
        }
    } else {
        phase = FRAMETIME + FRAMETIME + phase;
        if (phase > 1.0f) {
            current_index = object->field_0x1089;
            state_index = current_index;
            state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
            state[0xf78] = static_cast<u8>(object->apiobj.character_data->game_character->cloak_joint_2);
            *reinterpret_cast<f32 *>(state + 0xf50) = gravity;
            goto setup_state;
        }
    }

    {
        const f32 squared = phase * phase;
        const f32 value = gravity * squared + (1.0f - squared) * 0.0f;
        current_index = object->field_0x1089;
        state_index = current_index;
        state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
        state[0xf78] = static_cast<u8>(object->apiobj.character_data->game_character->cloak_joint_2);
        *reinterpret_cast<f32 *>(state + 0xf50) = value;
        if (value == 0.0f) {
            goto increment_index;
        }
    }

setup_state:
    state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
    state[0xf79] = 0x21;
    *reinterpret_cast<i16 *>(state + 0xf70) = 0;
    *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(0xb556);

increment_index:
    object->field_0x1089 = current_index + 1;
}

void HeadMovement(GameObject_s *object) {
    f32 previous;
    NUVEC direction;
    if (CInfo[object->character_context].flags & 0x200000) {
        return;
    }
    if (!(object->apiobj.character_data->game_character->field_0x94 & 0x40000000) &&
        !(object->apiobj.character_data->model_flags & 0x20000)) {
        CHARACTERDATA *character = object->apiobj.character_data;

        if (object->field_0x1089 < 3 && character->game_character->head_joint != -1) {
            object->field_0xefe &= ~1;
            i32 allowed;
            i16 animation;
            if (!object->apiobj.anim_packet.blending) {
                animation = object->apiobj.anim_packet.animation_index;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_ordinary;
            } else {
                animation = object->apiobj.anim_packet.blend_animation_b;
                if (animation >= 0 && animation < apicharsys->model_id_capacity &&
                    object->apiobj.character_model->model_data_b[animation] != NULL) {
                    if (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                        0x40) {
                        allowed = 0;
                        goto gate_done_ordinary;
                    }
                }
                animation = object->apiobj.anim_packet.blend_animation_a;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_ordinary;
            }

            if (animation < apicharsys->model_id_capacity &&
                object->apiobj.character_model->model_data_b[animation] != NULL) {
                allowed =
                    (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                     0x40) == 0;
            }
        gate_done_ordinary:;

            f32 first = 0.0f;
            f32 second = 0.0f;
            if (object->head_target != NULL && object->head_target_timer >= object->head_target_delay &&
                character->game_character->head_locator != -1 && allowed) {
                object->field_0xefe |= 1;

                NuVecInvMtxTransform(&direction, &object->head_target_position,
                                     &object->joint_matrices[character->game_character->head_locator]);
                if (direction.z < 0.0f) {
                    f32 angle = NuAtan2(-direction.y, NuFsqrt(direction.x * direction.x + direction.z * direction.z));
                    first = object->joint_modifiers[object->field_0x1089].rotation.y - angle;
                    f32 limit = character->game_character->field_0x54;
                    if (first >= limit)
                        first = limit;
                    else if (first <= -limit)
                        first = -limit;
                    angle = NuAtan2(-direction.x, -direction.z);
                    second = angle + object->joint_modifiers[object->field_0x1089].rotation.z;
                    limit = character->game_character->field_0x50;
                    if (second >= limit)
                        second = limit;
                    else if (second <= -limit)
                        second = -limit;
                    else
                        object->field_0xefe &= ~1;
                }
            }
            object->joint_modifiers[object->field_0x1089].joint_index = character->game_character->head_joint;
            object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(0);
            f32 blend = character->game_character->field_0x58 * FRAMETIME;

            if (blend > 1.0f) {
                blend = 1.0f;
                previous = 0.0f;
            } else {
                previous = 1.0f - blend;
            }
            object->joint_modifiers[object->field_0x1089].rotation.x = 0.0f;
            object->joint_modifiers[object->field_0x1089].rotation.y =
                first * blend + object->joint_modifiers[object->field_0x1089].rotation.y * previous;
            object->joint_modifiers[object->field_0x1089].rotation.z =
                second * blend + object->joint_modifiers[object->field_0x1089].rotation.z * previous;
            i32 limit = static_cast<i32>(character->game_character->field_0x54 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[1] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[1] = -limit;
            limit = static_cast<i32>(character->game_character->field_0x50 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[2] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[2] = -limit;
            if (object->joint_modifiers[object->field_0x1089].rotation.y > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.y < -0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z < -0.0017453293548896909f) {
                object->joint_modifiers[object->field_0x1089].flags = NUJOINTANIM_ROTATION;
                if (allowed) {
                    if (object->joint_modifiers[object->field_0x1089].rotation.y != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags =
                            static_cast<NUJOINTANIM_FLAGS>(NUJOINTANIM_ROTATION | NUJOINTANIM_LIMIT_ROTATION_Y);
                    if (object->joint_modifiers[object->field_0x1089].rotation.z != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(
                            object->joint_modifiers[object->field_0x1089].flags | NUJOINTANIM_LIMIT_ROTATION_Z);
                }
            }
            if (object->head_target != NULL) {
                object->head_target_timer -= FRAMETIME;
                if (object->head_target_timer < 0.0f) {
                    object->head_target_timer = 0.0f;
                    object->head_target_delay = 0.0f;
                    object->head_target = NULL;
                    object->head_target_priority = 0;
                }
            }
            object->field_0x1089 = object->field_0x1089 + 1;
        }
    } else if (!(object->apiobj.character_data->game_character->field_0x94 & 0x40000000)) {
        CHARACTERDATA *character = object->apiobj.character_data;

        if (object->field_0x1089 > 2 || character->game_character->head_joint == -1) {
            return;
        }

        i32 allowed;
        i16 animation;
        if (!object->apiobj.anim_packet.blending) {
            animation = object->apiobj.anim_packet.animation_index;
            allowed = 1;
            if (animation < 0)
                goto gate_done_old;
        } else {
            animation = object->apiobj.anim_packet.blend_animation_b;
            if (animation >= 0 && animation < apicharsys->model_id_capacity &&
                object->apiobj.character_model->model_data_b[animation] != NULL) {
                if (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                    0x40) {
                    allowed = 0;
                    goto gate_done_old;
                }
            }
            animation = object->apiobj.anim_packet.blend_animation_a;
            allowed = 1;
            if (animation < 0)
                goto gate_done_old;
        }

        if (animation < apicharsys->model_id_capacity &&
            object->apiobj.character_model->model_data_b[animation] != NULL) {
            allowed = (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                       0x40) == 0;
        }
    gate_done_old:;

        f32 first = 0.0f;
        f32 second = 0.0f;
        if (object->head_target != NULL && object->head_target_timer >= object->head_target_delay &&
            character->game_character->head_locator != -1 && allowed) {

            NuVecInvMtxTransform(&direction, &object->head_target_position,
                                 &object->joint_matrices[character->game_character->head_locator]);
            if (direction.z < 0.0f) {
                f32 angle = NuAtan2(-direction.x, -direction.z);
                first = object->joint_modifiers[object->field_0x1089].rotation.x - angle;
                f32 limit = character->game_character->field_0x50;
                if (first >= limit)
                    first = limit;
                else if (first <= -limit)
                    first = -limit;
                angle = NuAtan2(-direction.y, -direction.z);
                second = object->joint_modifiers[object->field_0x1089].rotation.y - angle;
                limit = character->game_character->field_0x54;
                if (second >= limit)
                    second = limit;
                else if (second <= -limit)
                    second = -limit;
            }
        }
        object->joint_modifiers[object->field_0x1089].joint_index = character->game_character->head_joint;
        object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(0);
        f32 blend = character->game_character->field_0x58 * FRAMETIME;
        f32 previous;
        if (blend > 1.0f) {
            blend = 1.0f;
            previous = 0.0f;
        } else {
            previous = 1.0f - blend;
        }
        object->joint_modifiers[object->field_0x1089].rotation.z = 0.0f;
        object->joint_modifiers[object->field_0x1089].rotation.x =
            first * blend + object->joint_modifiers[object->field_0x1089].rotation.x * previous;
        object->joint_modifiers[object->field_0x1089].rotation.y =
            second * blend + object->joint_modifiers[object->field_0x1089].rotation.y * previous;
        i32 limit = static_cast<i32>(character->game_character->field_0x50 * 10430.3779296875f);
        object->joint_modifiers[object->field_0x1089].rotation_limit_start[0] = limit;
        object->joint_modifiers[object->field_0x1089].rotation_limit_end[0] = -limit;
        limit = static_cast<i32>(character->game_character->field_0x54 * 10430.3779296875f);
        object->joint_modifiers[object->field_0x1089].rotation_limit_start[1] = limit;
        object->joint_modifiers[object->field_0x1089].rotation_limit_end[1] = -limit;
        if (object->joint_modifiers[object->field_0x1089].rotation.x > 0.0017453293548896909f ||
            object->joint_modifiers[object->field_0x1089].rotation.x < -0.0017453293548896909f ||
            object->joint_modifiers[object->field_0x1089].rotation.y > 0.0017453293548896909f ||
            object->joint_modifiers[object->field_0x1089].rotation.y < -0.0017453293548896909f) {
            object->joint_modifiers[object->field_0x1089].flags = NUJOINTANIM_ROTATION;
            if (allowed) {
                if (object->joint_modifiers[object->field_0x1089].rotation.x != 0.0f)
                    object->joint_modifiers[object->field_0x1089].flags =
                        static_cast<NUJOINTANIM_FLAGS>(NUJOINTANIM_ROTATION | NUJOINTANIM_LIMIT_ROTATION_X);
                if (object->joint_modifiers[object->field_0x1089].rotation.y != 0.0f)
                    object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(
                        object->joint_modifiers[object->field_0x1089].flags | NUJOINTANIM_LIMIT_ROTATION_Y);
            }
        }
        if (object->head_target != NULL) {
            object->head_target_timer -= FRAMETIME;
            if (object->head_target_timer < 0.0f) {
                object->head_target_timer = 0.0f;
                object->head_target_delay = 0.0f;
                object->head_target = NULL;
                object->head_target_priority = 0;
            }
        }
        object->field_0x1089 = object->field_0x1089 + 1;
    } else {
        if (object->field_0x1089 < 3 && object->apiobj.character_data->game_character->head_joint != -1) {
            object->field_0xefe &= ~1;
            i32 allowed;
            i16 animation;
            if (!object->apiobj.anim_packet.blending) {
                animation = object->apiobj.anim_packet.animation_index;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_alternate;
            } else {
                animation = object->apiobj.anim_packet.blend_animation_b;
                if (animation >= 0 && animation < apicharsys->model_id_capacity &&
                    object->apiobj.character_model->model_data_b[animation] != NULL) {
                    if (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                        0x40) {
                        allowed = 0;
                        goto gate_done_alternate;
                    }
                }
                animation = object->apiobj.anim_packet.blend_animation_a;
                allowed = 1;
                if (animation < 0)
                    goto gate_done_alternate;
            }

            if (animation < apicharsys->model_id_capacity &&
                object->apiobj.character_model->model_data_b[animation] != NULL) {
                allowed =
                    (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[animation])->flags &
                     0x40) == 0;
            }
        gate_done_alternate:;

            f32 first = 0.0f;
            f32 second = 0.0f;
            if (object->head_target != NULL && object->head_target_timer >= object->head_target_delay &&
                object->apiobj.character_data->game_character->head_locator != -1 && allowed) {
                object->field_0xefe |= 1;

                NuVecInvMtxTransform(
                    &direction, &object->head_target_position,
                    &object->joint_matrices[object->apiobj.character_data->game_character->head_locator]);
                if (direction.z < 0.0f) {
                    f32 angle = NuAtan2(-direction.y, NuFsqrt(direction.x * direction.x + direction.z * direction.z));
                    first = angle - object->joint_modifiers[object->field_0x1089].rotation.y;
                    f32 limit = object->apiobj.character_data->game_character->field_0x54;
                    if (first >= limit)
                        first = limit;
                    else if (first <= -limit)
                        first = -limit;
                    angle = NuAtan2(-direction.x, -direction.z);
                    second = -angle - object->joint_modifiers[object->field_0x1089].rotation.z;
                    limit = object->apiobj.character_data->game_character->field_0x50;
                    if (second >= limit)
                        second = limit;
                    else if (second <= -limit)
                        second = -limit;
                    else
                        object->field_0xefe &= ~1;
                }
            }
            CHARACTERDATA *character = object->apiobj.character_data;
            object->joint_modifiers[object->field_0x1089].joint_index = character->game_character->head_joint;
            object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(0);
            f32 blend = object->apiobj.character_data->game_character->field_0x58 * FRAMETIME;
            f32 previous;
            if (blend > 1.0f) {
                blend = 1.0f;
                previous = 0.0f;
            } else {
                previous = 1.0f - blend;
            }
            object->joint_modifiers[object->field_0x1089].rotation.x = 0.0f;
            object->joint_modifiers[object->field_0x1089].rotation.y =
                first * blend + object->joint_modifiers[object->field_0x1089].rotation.y * previous;
            object->joint_modifiers[object->field_0x1089].rotation.z =
                second * blend + object->joint_modifiers[object->field_0x1089].rotation.z * previous;
            i32 limit = static_cast<i32>(object->apiobj.character_data->game_character->field_0x54 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[1] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[1] = -limit;
            limit = static_cast<i32>(object->apiobj.character_data->game_character->field_0x50 * 10430.3779296875f);
            object->joint_modifiers[object->field_0x1089].rotation_limit_start[2] = limit;
            object->joint_modifiers[object->field_0x1089].rotation_limit_end[2] = -limit;
            if (object->joint_modifiers[object->field_0x1089].rotation.y > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.y < -0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z > 0.0017453293548896909f ||
                object->joint_modifiers[object->field_0x1089].rotation.z < -0.0017453293548896909f) {
                object->joint_modifiers[object->field_0x1089].flags = NUJOINTANIM_ROTATION;
                if (allowed) {
                    if (object->joint_modifiers[object->field_0x1089].rotation.y != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags =
                            static_cast<NUJOINTANIM_FLAGS>(NUJOINTANIM_ROTATION | NUJOINTANIM_LIMIT_ROTATION_Y);
                    if (object->joint_modifiers[object->field_0x1089].rotation.z != 0.0f)
                        object->joint_modifiers[object->field_0x1089].flags = static_cast<NUJOINTANIM_FLAGS>(
                            object->joint_modifiers[object->field_0x1089].flags | NUJOINTANIM_LIMIT_ROTATION_Z);
                }
            }
            if (object->head_target != NULL) {
                object->head_target_timer -= FRAMETIME;
                if (object->head_target_timer < 0.0f) {
                    object->head_target_timer = 0.0f;
                    object->head_target_delay = 0.0f;
                    object->head_target = NULL;
                    object->head_target_priority = 0;
                }
            }
            object->field_0x1089 = object->field_0x1089 + 1;
        }
    }
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

i32 CharIDFromName(char *name) {
    for (i32 i = 0; i < CHARCOUNT; i++) {
        if (NuStrICmp(CDataList[i].file, name) == 0) {
            return i;
        }
    }

    return -1;
}

CHARACTERDATA *CDataFromName(char *name) {
    for (i32 i = 0; i < CHARCOUNT; i++) {
        if (NuStrICmp(CDataList[i].file, name) == 0) {
            return &CDataList[i];
        }
    }

    return nullptr;
}

// Retail copies the first argument into the fourth and appends the animation name there.
// Retail also returns 0 for a NULL redirect table; that check is not reproduced yet.
i32 RedirectAnim(char *directory, ANIMREDIRECT *redirects, ANIMLIST_s *animation_list, char *path) {
    CHARACTERANIM_s *animation = reinterpret_cast<CHARACTERANIM_s *>(animation_list);
    for (ANIMREDIRECT *redirect = redirects; redirect->name != NULL; ++redirect) {
        if (!(redirect->animation_id == animation->animation_id && NuStrICmp(redirect->name, animation->name) == 0)) {
            continue;
        }
        NuStrCpy(path, directory);
        NuStrCat(path, animation->name);
        animation->flags &= ~CHARACTER_ANIMATION_FLAG_BSA;
        return 1;
    }
    return 0;
}

void CharScenes_Init(variptr_u *buf, variptr_u *) {
    const usize size = static_cast<usize>(CHARCOUNT) * sizeof(*CharScene_Area);
    const usize area = ALIGN(buf->addr, 4);
    CharScene_Area = reinterpret_cast<CHARSCENE_s *>(area);
    buf->addr = area + size;
    memset(reinterpret_cast<void *>(area), 0, size);
}

void CharScenes_LevelLoad(WORLDINFO *world) {
    if (CHARCOUNT <= 0) {
        return;
    }

    for (i32 i = 0; i < CHARCOUNT; i++) {
        CHARSCENE_s *entry = &world->minikit.character_scenes[i];
        entry->scene = NULL;

        // Check if we should load this character scene
        if ((CharScene_Area == NULL || CharScene_Area[i].scene == NULL) && (CDataList[i].flags & 1) != 0 &&
            world->cutscene_sys != NULL) {
            // Check if this character is in a cutscene
            u32 *cutscene_flags = *(u32 **)((char *)world->cutscene_sys + 8);
            u32 flag = (cutscene_flags[i >> 5] >> (i & 0x1f)) & 1;
            if (flag != 0) {
                // Load the character scene
                char path[128];
                VARIPTR buf_end = world->unknown_0108;
                sprintf(path, "chars\\%s\\%s.gsc", CDataList[i].dir, CDataList[i].file);
                NUGSCN *scene = NuGScnRead(&world->giz_buffer, buf_end, path);
                entry->scene = scene;
                if (scene != NULL) {
                    NuSpecialFind(scene, &entry->special_scene, CDataList[i].file, 1);
                }
            }
        }
    }
}

extern i16 tUNKNOWN;
extern "C" {
    extern i16 id_R2Q5;
    void PlaySfx(char *, nuvec_s *);
    void PlaySfxAndSetVolume(char *, nuvec_s *, f32);
}
void Move_CHARACTER(GameObject_s *);
void Animate_CHARACTER(GameObject_s *);

void FixUpCharacters(CHARFIXUP *fixup);

void PostAnimate_FETT(GameObject_s *object);

void ResetAICreature(GameObject_s *, AISYS_s *);
void LightGameObject(GameObject_s *, void *);
void InitSurfaceInfo(GameObject_s *);
i32 SetObjOnSurface(GameObject_s *, i32);
extern "C" NUVEC plr_lastpos;

void FinishWeirdoNames(i32 which);

GameObject_s *ActivateCharacter(char *name, nuvec_s *position, i32 angle);

extern i32 addcreature_override_id_check;

extern f32 default_mover_extra;
extern void SetGameObjectCharacterData(GameObject_s *obj);
extern void GetTopBot(GameObject_s *obj);
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern void ResetCharacterIdle(GameObject_s *obj, i32 mode, i32 idle);
extern void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);


static void SetLayers_BOB(GameObject_s *object);

// Retail keeps only the IPA-SRA clone, which receives &*layers.
#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif
static MOSEISLEY_LAYERS_ATTR void SetLayers_MOSEISLEYCITIZEN(u32 *layers)
    __asm__("_ZL26SetLayers_MOSEISLEYCITIZENP12GameObject_s.isra.2");
static void SetLayers_MOSEISLEYCITIZEN(u32 *layers);

i32 InitCreature(GameObject_s *obj, i32 id, i32 param);

void Shards_HandleLostObj(WORLDINFO_s *, GameObject_s *);
void LoseHelmet(GameObject_s *, i32, i32);
void DestroySnakeBody(GameObject_s *);
void InitPlayerAI(GameObject_s *);
void Player_ClearContext(GameObject_s *, i32);
void ResetPlayerMoves(GameObject_s *);
void Player_ResetContexts(PLAYERPACKET_s *);
void InitSurfaceInfo(GameObject_s *);
i32 SetObjOnSurface(GameObject_s *, i32);
void GizForce_ResetLOS(GameObject_s *);
i32 StartSlide(GameObject_s *, i32);

i32 NewPlayerCharacter(GameObject_s *object, i32 id, i32 old_id, i32);


i32 GetDefaultIdle(GameObject_s *obj);

void ResetCharacterIdle(GameObject_s *object, i32 mode, i32 animation);

static void NewCharacterIdle(GameObject_s *object, i32 default_idle);

void UpdateCharacterIdle(GameObject_s *object);

void UpdateCharacterIDs() {
    const u16 count = ShopCollection.count_y;
    if (count) {
        COLLECTID *collect = ShopCollection.list;
        shopitem_s *item = CharItems;
        for (i32 i = 0; i < count; ++i, ++item, ++collect) {
            item->price = collect->field3_0x4;
            item->item_id = collect->id;
        }
    }
}

void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *list, variptr_u *buf, variptr_u buf_end) {
    char path[128];
    if (CharScene_Area == NULL) {
        return;
    }

    for (i32 i = 0; i < apicharsys->loaded_model_count; ++i) {
        const i32 model_id = apicharsys->models[i].model_id;
        CHARSCENE_s &entry = CharScene_Area[model_id];
        if (entry.scene == NULL && (CDataList[model_id].flags & 1) != 0) {
            // The original reuses the loop counter as InModelList's output index.
            if (InModelList(list, model_id, &i) == 0 || list[i].count == 0) {
                continue;
            }

            sprintf(path, "chars\\%s\\%s.gsc", CDataList[model_id].dir, CDataList[model_id].file);
            entry.scene = NuGScnRead(buf, buf_end, path);
            if (entry.scene != NULL) {
                NuSpecialFind(entry.scene, &entry.special_scene, CDataList[model_id].file, 1);
            }
        }
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

// Original characters.cpp cluster: SetMoveAndAnimateFunctions (ELF 0x460dc0)
// follows the scene/category helpers and precedes character model loading.
void SetMoveAndAnimateFunctions(u32 model_flag_mask, u32 model_flag_value, u32 game_flag_mask, u32 game_flag_value,
                                i32 movement_type, void *move_function, void *animate_function, void *draw_function) {
    for (i32 i = 0; i < CHARCOUNT; ++i) {
        if (model_flag_mask != 0 && (CDataList[i].model_flags & model_flag_mask) != model_flag_value)
            continue;
        if (game_flag_mask != 0 && (GCDataList[i].flags_090 & game_flag_mask) != game_flag_value)
            continue;
        if (movement_type != -1 && static_cast<i8>(GCDataList[i].uses_weapon_action) != movement_type)
            continue;
        if (move_function != NULL)
            CDataList[i].move_fn = reinterpret_cast<CHARACTERUPDATEFN>(move_function);
        if (animate_function != NULL)
            CDataList[i].animate_fn = reinterpret_cast<CHARACTERUPDATEFN>(animate_function);
        if (draw_function != NULL)
            CDataList[i].draw_fn = reinterpret_cast<CHARACTERUPDATEFN>(draw_function);
    }
}

void DeactivateGameObject(GameObject_s *);

void DeactivateCharacter(char *name);

void LoadSingleCharacter(bgprocinfo_s *);

void CharScenes_LevelDump(WORLDINFO_s *world) {
    CHARSCENE_s *scenes = world->minikit.character_scenes;
    if (scenes != NULL) {
        for (i32 i = 0; i < CHARCOUNT; ++i) {
            if (world->minikit.character_scenes[i].scene != NULL) {
                NuGScnRemove(world->minikit.character_scenes[i].scene);
            }
            world->minikit.character_scenes[i].scene = NULL;
        }
    }
}

void UpdateCharacterLoad();

extern "C" VARIPTR characterbuffer_base;
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

void NextStatusStage(STATUSPACKET_s *);
i32 AddToCollection(i32);
void NewStatusRumbleBuzz(i32, f32, f32, i32);
void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
extern "C" void PlaySfx(char *, nuvec_s *);
void Collection_Draw(COLLECTION_s *, f32, f32, f32, APICHARACTERMODELLIST_s *, f32, i32);
void Collection_GetPos(COLLECTION_s *, i32, f32 *, f32 *);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
extern f32 ICONSIZE;
extern i32 STATUS_R, STATUS_G, STATUS_B;
f32 COLLECTION_Y_STATUS = -0.3f;
static i32 playedappearsfx;
static i32 playedmovesfx;

void CollectCharcters_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    COLLECTION_s *collection = packet->vehicle_area ? &VehicleCollection : &CharacterCollection;
    if (active == 0)
        return;
    if (stage->field_0x14 == 0) {
        const f32 offscreen = (1.0f - fabsf(COLLECTION_Y_STATUS)) + 1.0f;
        const f32 ratio = NuFdiv(stage->field_0x18, stage->field_0x1c);
        Collection_Draw(collection, 0.0f,
                        NuTrigTable[(static_cast<i32>(ratio * 16384.0f) >> 1) & 0x7fff] *
                                (COLLECTION_Y_STATUS + offscreen) -
                            offscreen,
                        collection->field_10, NULL, 1.0f, 0);
        return;
    }
    const i32 id = StatusCollectList.ids[stage->field_0x14 - 1];
    if (id == -1)
        return;
    Collection_Draw(collection, 0.0f, COLLECTION_Y_STATUS, collection->field_10, NULL, 1.0f, 0);
    f32 time = stage->field_0x18;
    f32 x = 0.0f, y = 0.7f, size = 0.4f, icon_alpha = 1.0f, text_alpha;
    if (time < 0.5f) {
        if (time > 0.25f && playedappearsfx == 0) {
            PlaySfx(const_cast<char *>("Char_Icon_App"), NULL);
            playedappearsfx = 1;
            time = stage->field_0x18;
        }
        text_alpha = NuTrigTable[(static_cast<i32>((time + time) * 16384.0f) >> 1) & 0x7fff];
        icon_alpha = time + time;
    } else if (time < 2.0f) {
        text_alpha = 1.0f;
        playedappearsfx = 0;
        playedmovesfx = 0;
    } else {
        text_alpha = 1.0f - NuTrigTable[(static_cast<i32>((time - 2.0f) * 16384.0f) >> 1) & 0x7fff];
        if (playedmovesfx == 0) {
            PlaySfx(const_cast<char *>("Char_Icon_Slide"), NULL);
            playedmovesfx = 1;
        }
        const f32 blend =
            1.0f - (NuTrigTable[(static_cast<i32>((time - 2.0f) * 32768.0f + 16384.0f) >> 1) & 0x7fff] + 1.0f) * 0.5f;
        if (InCollectList_Index(id, collection->list, collection->count_y) != -1) {
            f32 target_x, target_y;
            Collection_GetPos(collection, id, &target_x, &target_y);
            f32 target_size = collection->field_10 * ICONSIZE;
            if (Game_OptionsSave != NULL && Game_OptionsSave->field11_0xb != 0)
                target_size *= 0.875f;
            x = target_x * blend + 0.0f;
            y = (target_y - 0.7f) * blend + 0.7f;
            size = (target_size - 0.4f) * blend + 0.4f;
        }
    }
    DrawCharIcon(id, x, y, 0.0f, size, 0xa7, icon_alpha, icon_alpha, 1, NULL);
    SmartTextEx(TTab[CDataList[id].name_id], 0.0f, 0.35f, 1.0f, 0.6f, 0.6f, 0.6f, 0, STATUS_R, STATUS_G, STATUS_B, 1.7f,
                1, NULL, 0, static_cast<i32>(text_alpha * 128.0f));
}

void CollectCharcters_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet) {
    if (stage->field_0x14 == 0)
        stage->field_0x14 = 1;
    while (StatusCollectList.ids[stage->field_0x14 - 1] != -1) {
        AddToCollection(StatusCollectList.ids[stage->field_0x14 - 1]);
        ++stage->field_0x14;
    }
    NextStatusStage(packet);
    NextStatusStage(packet);
}

nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id) {
    nuhspecial_s *special;
    if (__builtin_expect(CharScene_Area != NULL && CharScene_Area[character_id].scene != NULL, 1)) {
        special = &CharScene_Area[character_id].special_scene;
    } else if (world->minikit.character_scenes[character_id].scene != NULL) {
        special = &world->minikit.character_scenes[character_id].special_scene;
    } else {
        return NULL;
    }
    if (NuSpecialExistsFn(special) == 0) {
        special = NULL;
    }
    return special;
}

void PostAnimate_ASTROMECH(GameObject_s *object);

AILOCATOR_s *LocalGetRandomLocator(AILOCATOR_s **locators, i32 count, f32 clip_radius, NUVEC *position,
                                   f32 max_distance, i32 outside_camera, f32 max_delta_y, f32 min_delta_y);

AILOCATOR_s *LocalGetNearestLocator(AILOCATOR_s **locators, i32 count, f32 clip_radius, NUVEC *position,
                                    f32 max_distance, i32 outside_camera, f32 max_delta_y, f32 min_delta_y);

i32 newCharactersCollected(STATUSPACKET_s *) {
    i32 count = 0;
    for (i16 *player_id = Area_PlayerIDList; *player_id != -1 && count < 7; ++player_id) {
        const i16 player = *player_id;
        const i32 id = player;
        i32 previous = 0;
        while (previous < count && StatusCollectList.ids[previous] != id) {
            ++previous;
        }
        if (previous == count) {
            if (__builtin_expect(Game_CharacterSave != NULL && (Game_CharacterSave[id] & 1) != 0, 1)) {
                continue;
            }
            const i32 index = InCollectList_Index(id, MasterCollection.list, MasterCollection.count_y);
            if (__builtin_expect(index != -1 && MasterCollection.list[index].can_buy == 0, 0)) {
                StatusCollectList.ids[count++] = player;
                if (MasterCollection.list[index].field5_0x9 != 0) {
                    AddToCompletionPoints(POINTS_PER_CHARACTER);
                }
            }
        }
    }
    StatusCollectList.ids[count] = -1;
    return count;
}

void CollectCharcters_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    if (stage->field_0x14 == 0) {
        stage->field_0x1c = 1.0f;
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= 1.0f) {
            stage->field_0x14 = 1;
            if (StatusCollectList.ids[0] == -1) {
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
                NewStatusRumbleBuzz(-1, 0.6f, 0.0f, 0);
                NextStatusStage(packet);
            } else {
                stage->field_0x18 = 0.0f;
                stage->field_0x1c = 3.0f;
            }
        }
    } else {
        stage->field_0x18 += elapsed;
        if (stage->field_0x18 >= stage->field_0x1c) {
            AddToCollection(StatusCollectList.ids[stage->field_0x14 - 1]);
            PlaySfx(const_cast<char *>("LegoClicks"), NULL);
            ++stage->field_0x14;
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 3.0f;
            if (StatusCollectList.ids[stage->field_0x14 - 1] == -1)
                NextStatusStage(packet);
        }
    }
}

i32 SetProtocolDroidFallAnim(GameObject_s *object);

void CollectCharactersOff_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active) {
    COLLECTION_s *collection = packet->vehicle_area ? &VehicleCollection : &CharacterCollection;
    if (active == 0)
        return;
    f32 alpha = 1.0f;
    if (stage->field_0x14 > 0)
        alpha = 1.0f - NuFdiv(stage->field_0x18, stage->field_0x1c);
    Collection_Draw(collection, 0.0f, COLLECTION_Y_STATUS, collection->field_10, NULL, alpha, 0);
}

void CollectCharactersOff_Skip(STATUS_STAGE_s *, STATUSPACKET_s *) {
}

void ScaleGameObject(GameObject_s *obj);

void SetGameObjectCharacterData(GameObject_s *obj);

void CollectCharactersOff_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed) {
    switch (stage->field_0x14) {
        case 0:
            stage->field_0x18 = 0.0f;
            stage->field_0x1c = 1.0f;
            stage->field_0x14 = 1;
            break;
        case 1:
            stage->field_0x18 += elapsed;
            if (stage->field_0x18 >= stage->field_0x1c)
                NextStatusStage(packet);
            break;
    }
}

extern "C" i32 dagobah_training;

i32 TakeOverYodaSeekDistanceHack(GameObject_s *object, GameObject_s *luke, nuvec_s *offset);

void SetProtocolDroidInterfaceAction(GameObject_s *object);

void SetProtocolDroidDeactivatedAction(GameObject_s *object);

extern NUMTL * lightningmat;
extern NUMTL *DebMat[10];
extern "C" void NuWaterInit(void);
extern "C" void NuBridgeInit(void);

void LoadPerm1();

void LoadPerm2();
