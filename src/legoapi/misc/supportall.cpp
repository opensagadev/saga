#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/actions/combat/hits.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "globals.h"
#include "legoapi/core/input/qrand.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/mission.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizflow.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/areas.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "nu2api/nu3d/nuspecial.h"

void Hint_SetHintFromId(i32, i32, i32);
void MakeBaddiesForgetAboutParty(i32);
void ResetRadios();
void AITriggerSetSysReset(AITRIGGERSETSYS_s *);
void AITriggerSysAutoSetUp(WORLDINFO_s *, AITRIGGERSETSYS_s *);
void ResetPlayer(GameObject_s *, i32, nuvec_s *, i32);
f32 GetVehicleAreaRememberSpeed();
void CharPlatforms_Reset(CHARPLATFORMSYS_s *);
void SetSoundFadeDist(WORLDINFO_s *, OPTIONSSAVE_s *);
void GameCameraMakeMiniCut(nugspline_s *, f32, f32, f32, f32, i32, i32);
void CutScene_StartAudio();
void oneAtOnce_SetNumAttackers(i32);
void EffectOffProgress_Reset(LEVEL_PROGRESS_s *);
extern GameObject_s *alert_obj;
extern f32 alert_timer;
extern f32 LevelNameMul, LevelNameTime;
extern rtldata_s lev_rtldata;
extern "C" {
    extern f32 chattersfxwait, tieonsfxwait, tieoffsfxwait;
    extern i32 party_under_cover, nbaddies_can_see_players;
    extern i32 FalconDebKey[2];
    extern f32 TargetDist_Near2, TargetDist_Mid2;
    extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
    extern i32 makebaddiesforgetinresetbits;
    extern i32 reset_reimport;
    extern u32 arcade_placed_stud_total;
}

void CutScenes_Reset(WORLDINFO_s *);
void ClearLevelProgress(i32, WORLDINFO_s *);
void ResetScene(nugscn_s *, SCENEPROGRESS_s *);
void GizmoBlowupVisibilityOverrides(WORLDINFO_s *);
void SetTexAnimSignals(void);
void Customiser_SetUpCharacterData(CUSTOMISER *);
void Surfaces_Reset(void);
void Bolts_Reset(void);
void Batarangs_Reset(void);
void Detonators_Reset(void);
void ResetExplosions(void);
void ShoveObjectSysReset(void);
void ResetGameMessages(void);
void Tag_ResetTransfers(void);
void Tag_SetMode(i32 mode);
u32 TotalLevelCoinTally(WORLDINFO_s *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *);
void GameCameraMakeMiniCut(nugspline_s *, f32, f32, f32, f32, i32, i32);
extern i32 bonusmodearcade;
u32 arcade_placed_stud_total = 0;
extern f32 DEFAULT_MOVE_RANGE;
extern f32 drop_back_in_timer;
extern i32 last_chatter_sfx;
i32 FalconDebKey[2] = {-1, -1};
extern f32 LevelNameTime;
extern f32 LevelNameMul;
extern rtldata_s lev_rtldata;
void Hint_Reset(void);
void Hint_CancelCurrent(void);
void Hint_SetHintFromId(i32, i32, i32);
void TrafficAnimSys_Reset(TRAFFICANIMSYS_s *);
void Pulses_Reset(PULSESYS_s *);
void ResetRepeatSfx(void);
void ResetRippleSet(ripple_set_s *);
void Grabber_Reset(WORLDINFO_s *);
void Faders_Reset(WORLDINFO_s *);
void InitGameMode(void);
void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 progress_index);
void ResetForceBack(void);
void ClearAICreatures(void);
void ReStoreStatusTakeOverObjectSys(i32 restore_progress);
void InitPlayerAI(GameObject_s *object);
void ResetPlayer(GameObject_s *, i32, nuvec_s *, i32);
f32 GetVehicleAreaRememberSpeed();
void ResetRadios();
void AITriggerSetSysReset(AITRIGGERSETSYS_s *);
void AITriggerSysAutoSetUp(WORLDINFO_s *, AITRIGGERSETSYS_s *);
void CharPlatforms_Reset(CHARPLATFORMSYS_s *);
void CutScene_StartAudio();
void oneAtOnce_SetNumAttackers(i32);
void SetSoundFadeDist(WORLDINFO_s *, OPTIONSSAVE_s *);
void EffectOffProgress_Reset(LEVEL_PROGRESS_s *);
extern GAMECAMERA_s *GameCam;
extern ripple_set_s *ripples;
extern "C" void ResetParts(void);
extern "C" void NuSound3StopRumble(void);
extern "C" f32 chattersfxwait;
extern "C" f32 tieonsfxwait;
extern "C" f32 tieoffsfxwait;
extern GameObject_s *alert_obj;
extern f32 alert_timer;

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

static i32 TBGAMECOUNT;
static i32 TBDRAWCOUNT;
static i32 TBPLAYERCOUNT;
static i32 TBAICOUNT;

f32 TargetDist_Near2;
f32 TargetDist_Mid2;
u16 TargetDeg_Near;
u16 TargetDeg_Mid;
u16 TargetDeg_Far;
extern i32 party_under_cover;
i32 makebaddiesforgetinresetbits;
void MakeBaddiesForgetAboutParty(i32);
extern i32 nbaddies_can_see_players;
i32 reset_reimport;

void CatchUpCode(GameObject_s *object, f32 divisor, f32 maximum, i32 mode) {
    if (static_cast<u8>(object->apiobj.field_0x27c) == 0xff) {
        return;
    }

    const i32 socket_index = static_cast<i8>(object->field_0x661);
    object->field_0xc38 = 0.0f;
    if (socket_index == -1 || WORLD->sock_sys == NULL) {
        return;
    }
    if (mode != 0 && static_cast<i8>(object->apiobj.field_0x1f8) >= 0) {
        return;
    }

    const bool looping = WORLD->sock_sys->sock[socket_index].looping != 0;
    GameObject_s *other;
    if (Player[0] == object) {
        other = Player[1];
    } else if (Player[1] == object) {
        other = Player[0];
    } else {
        return;
    }

    if (other != NULL &&
        (static_cast<i8>(object->apiobj.field_0x1f8) >= 0 || static_cast<i8>(other->apiobj.field_0x1f8) < 0) &&
        static_cast<i8>(other->field_0x661) == socket_index &&
        (mode == 0 || static_cast<i8>(other->apiobj.field_0x1f8) < 0)) {
        f32 distance = object->sock_position.normalized_distance - other->sock_position.normalized_distance;
        if (looping) {
            if (distance >= 0.5f) {
                distance -= 1.0f;
            } else if (-0.5f >= distance) {
                distance += 1.0f;
            }
        }
        if (distance < 0.0f) {
            const f32 catchup = -distance / divisor;
            maximum = MIN(maximum, catchup);
            object->field_0xc38 = maximum;
        }
    }
}

struct TexQuadVertex {
    f32 x, y, z;
    u32 colour;
    union {
        f32 uv[2];
        u16 half_uv[4];
    };
};
static inline void TexQuadSubmit(NUVEC const &point, i32 colour, i32 u, i32 v) {
    TexQuadVertex *vertex = static_cast<TexQuadVertex *>(g_NuPrim_StreamBufferPtr->void_ptr);
    if (g_NuPrim_NeedsOverbrightening)
        vertex->colour = colour;
    else
        vertex->colour = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    if (g_NuPrim_NeedsHalfUVs) {
        vertex->half_uv[0] = u ? 0x3c00 : 0;
        vertex->half_uv[1] = v ? 0x3c00 : 0;
    } else {
        vertex->uv[0] = static_cast<f32>(u);
        vertex->uv[1] = static_cast<f32>(v);
    }
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * point.x, static_cast<f32>(PS2_VREZ_H) * point.y, 0.0f);
}
void RndrTexQuad(f32 x, f32 y, f32 width, f32 height, i32 colour, numtl_s *material, i32 angle) {
    NUVEC points[4] = {};
    points[0].x = -0.5f;
    points[0].y = -0.5f;
    points[1].x = 0.5f;
    points[1].y = -0.5f;
    points[2].x = -0.5f;
    points[2].y = 0.5f;
    points[3].x = 0.5f;
    points[3].y = 0.5f;
    NuVecRotateZ(&points[0], &points[0], angle);
    NuVecRotateZ(&points[1], &points[1], angle);
    NuVecRotateZ(&points[2], &points[2], angle);
    NuVecRotateZ(&points[3], &points[3], angle);
    points[0].x *= width;
    points[0].y *= height;
    points[1].x *= width;
    points[1].y *= height;
    points[2].x *= width;
    points[2].y *= height;
    points[3].x *= width;
    points[3].y *= height;
    points[0].x += x;
    points[0].y += y;
    points[1].x += x;
    points[1].y += y;
    points[2].x += x;
    points[2].y += y;
    points[3].x += x;
    points[3].y += y;
    NuPrim2DBegin(1, 7, material);
    TexQuadSubmit(points[0], colour, 0, 0);
    TexQuadSubmit(points[1], colour, 1, 0);
    TexQuadSubmit(points[2], colour, 0, 1);
    TexQuadSubmit(points[3], colour, 1, 1);
    NuPrim2DEnd();
}

i32 SuperWeirdo(GameObject_s *object) {
    if ((object->apiobj.flags_low & 0x80) != 0 && (Game.field_0x7c26[1] & 1) != 0 && CharacterCustomiser != NULL &&
        (object->id == CharacterCustomiser->character_ids[0] || object->id == CharacterCustomiser->character_ids[1])) {
        return 1;
    }
    return 0;
}

void bgProcClose() {
    STUBBED();
}

extern "C" void edrtlCalculateBurnoutEx(burnset_s *set, NuBloomParameters *parameters, NUVEC *camera_position,
                                        f32 frame_time);

void BurnoutApply(i32 paused) {
    NuBloomParameters parameters = {};
    CUTINFO *cut = static_cast<CUTINFO *>(CutStopInfo);
    if (cut != NULL && (cut->flags & 0x80) != 0)
        return;
    if (WORLD->burnset == NULL)
        return;
    edrtlCalculateBurnoutEx(WORLD->burnset, &parameters, reinterpret_cast<NUVEC *>(&global_camera.mtx.m30), FRAMETIME);
    if (parameters.intensity > 0.0f)
        NuPostBloom(paused, &parameters);
}

void bgprocFreeze() {
    STUBBED();
}

void FindSlamOrigin(GameObject_s *, NUVEC *, NUVEC *);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);

void AddSlamDebris(GameObject_s *object) {
    NUVEC position;
    FindSlamOrigin(object, &position, NULL);
    f32 radius = 0.6f;
    f32 strength = 0.3f;
    i32 flags = 0x17;
    i32 damage = 1;
    if ((object->apiobj.flags_low & 0x80) != 0 && Cheat_IsOn(0x16)) {
        radius = 1.0f;
        strength = 0.5f;
        flags = 0x37;
        damage = 2;
    }
    EXPLOSION *explosion = AddExplosion(&position, radius, strength, object, object->slam_debris_effect, flags);
    if (explosion != NULL)
        explosion->field_0x32 = damage;
}

void CloakMovement(GameObject_s *object) {
    const u8 index = object->field_0x1089;
    if (index > 2) {
        return;
    }
    GAMECHARACTERDATA_s *character = object->apiobj.character_data->game_character;
    if (character->cloak_joint == -1) {
        return;
    }
    u8 *state = reinterpret_cast<u8 *>(object) + index * 0x34;
    f32 &value = *reinterpret_cast<f32 *>(state + 0xf50);
    f32 phase = NuFsqrt((value - character->field_0x60) / (character->field_0x5c - character->field_0x60));
    f32 high_weight;
    f32 low_weight;
    if (LEGOACT_FALL != -1) {
        i16 animation;
        if (object->apiobj.anim_packet.blending == 0) {
            animation = object->apiobj.anim_packet.animation_index;
        } else {
            animation = object->apiobj.anim_packet.blend_animation_b;
        }
        if (animation == LEGOACT_FALL) {
            phase = FRAMETIME + FRAMETIME + phase;
            if (phase <= 1.0f) {
                high_weight = phase * phase;
                low_weight = 1.0f - high_weight;
            } else {
                high_weight = 1.0f;
                low_weight = 0.0f;
            }
            goto apply_weights;
        }
    }

    high_weight = 0.0f;
    phase = phase - (FRAMETIME + FRAMETIME);
    low_weight = 1.0f;
    if (0.0f <= phase) {
        high_weight = phase * phase;
        low_weight = 1.0f - high_weight;
    }

apply_weights:
    const u8 current_index = object->field_0x1089;
    const u32 state_index = current_index;
    state = reinterpret_cast<u8 *>(object) + state_index * 0x34;
    CHARACTERDATA *character_data = object->apiobj.character_data;
    state[0xf78] = static_cast<u8>(character_data->game_character->cloak_joint);
    GAMECHARACTERDATA_s *current_character = character_data->game_character;
    const f32 interpolated_value =
        high_weight * current_character->field_0x5c + low_weight * current_character->field_0x60;
    *reinterpret_cast<f32 *>(state + 0xf50) = interpolated_value;
    if (interpolated_value == 0.0f) {
        state[0xf79] = 0;
    } else {
        state[0xf79] = 0x21;
        if (current_character->field_0x60 <= current_character->field_0x5c) {
            *reinterpret_cast<i16 *>(state + 0xf70) = static_cast<i16>(current_character->field_0x5c * 32768.0f);
            *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(current_character->field_0x60 * 32768.0f);
        } else {
            *reinterpret_cast<i16 *>(state + 0xf70) = static_cast<i16>(current_character->field_0x60 * 32768.0f);
            *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(current_character->field_0x5c * 32768.0f);
        }
    }
    object->field_0x1089 = current_index + 1;
}

static inline void TexQuadSubmit3D(f32 x, f32 y, i32 colour, i32 u, i32 v) {
    TexQuadVertex *vertex = static_cast<TexQuadVertex *>(g_NuPrim_StreamBufferPtr->void_ptr);
    if (g_NuPrim_NeedsOverbrightening != 0) {
        vertex->colour = colour;
    } else {
        vertex->colour = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    }
    if (g_NuPrim_NeedsHalfUVs != 0) {
        vertex->half_uv[0] = u != 0 ? 0x3c00 : 0;
        vertex->half_uv[1] = v != 0 ? 0x3c00 : 0;
    } else {
        vertex->uv[0] = static_cast<f32>(u);
        vertex->uv[1] = static_cast<f32>(v);
    }
    vertex->x = x;
    vertex->y = y;
    vertex->z = 0.0f;
    g_NuPrim_StreamBufferPtr->u8_ptr += sizeof(TexQuadVertex);
    ++g_NuPrim_VertexCount;
}

void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material) {
    NuPrim3DBegin(1, 7, material, reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(&matrix)));
    TexQuadSubmit3D(-0.5f, -0.5f, colour, 0, 0);
    TexQuadSubmit3D(0.5f, -0.5f, colour, 1, 0);
    TexQuadSubmit3D(-0.5f, 0.5f, colour, 0, 1);
    TexQuadSubmit3D(0.5f, 0.5f, colour, 1, 1);
    NuPrim3DEnd();
}

void CheckResetBits() {
    if ((ResetBits & RESETBIT_CLEAR_LEVEL_PROGRESS) != 0 && WORLD->current_level->area_level_index != -1) {
        ClearLevelProgress(WORLD->current_level->area_level_index, WORLD);
    }

    Cheats_Reset();
    if (WORLD->level_progress != NULL) {
        ResetScene(WORLD->current_gscn, reinterpret_cast<SCENEPROGRESS_s *>(WORLD->level_progress));
        WORLD->level_progress->flags |= 1;
    }

    GizmoBlowupVisibilityOverrides(WORLD);
    texanimbits = 0;
    SetTexAnimSignals();
    SetTexAnimSignals();
    CutScenes_Reset(WORLD);

    if (NOSOUND == 0) {
        if ((ResetBits & RESETBIT_USE_CUSTOMISER_SETUP) == 0) {
            InitGameMode();
        }
        if (NOSOUND == 0) {
            Customiser_SetUpCharacterData(CharacterCustomiser);
        }
    }

    Surfaces_Reset();
    ResetStreaks();
    Bolts_Reset();
    Batarangs_Reset();
    Detonators_Reset();
    ResetParts();
    ResetExplosions();
    ShoveObjectSysReset();
    Panel_Clear();
    GameCam_Reset(GameCam);
    ResetGameMessages();
    Tag_ResetTransfers();
    Hint_Reset();
    Hint_CancelCurrent();
    Teleports_Reset(WORLD);
    TrafficAnimSys_Reset(WORLD->trafficanim_sys);
    Pulses_Reset(WORLD->pulses_sys);
    NuSound3StopRumble();
    ResetTimer(&GameTimer, 0.0f);
    ResetTimer(&GamePlayTimer, 0.0f);
    ResetTimer(&PauseTimer, 0.0f);
    ResetTimer(&JoinInTimer, 0.0f);
    if ((ResetBits & RESETBIT_CLEAR_LEVEL_PROGRESS) != 0 && MissionSys != NULL) {
        ResetTimer(&MissionSys->timer, 0.0f);
    }
    Hint_SetHintFromId(-1, 0, 0);
    ResetRepeatSfx();
    DEFAULT_MOVE_RANGE = 0.0f;
    alert_obj = NULL;
    alert_timer = 0.0f;
    drop_back_in_timer = 0.0f;
    ResetRippleSet(ripples);
    Grabber_Reset(WORLD);
    Faders_Reset(WORLD);
    chattersfxwait = 3.0f;
    last_chatter_sfx = -1;
    tieonsfxwait = 1.0f;
    tieoffsfxwait = 1.0f;
    FalconDebKey[0] = -1;
    FalconDebKey[1] = -1;
    LevelNameMul = 0.0f;
    LevelNameTime = LevelChange != 0 ? 3.0f : 0.0f;
    memset(&lev_rtldata, 0, sizeof(lev_rtldata));
    if (VehicleArea != 0) {
        if (WORLD->current_level == ASTEROIDCHASED_LDATA || WORLD->current_level == DEATHSTAR2BATTLEA_LDATA) {
            TargetDist_Near2 = 6.25f;
            TargetDist_Mid2 = 100.0f;
            TargetDeg_Near = 0x38e3;
            TargetDeg_Mid = 0x31c7;
            TargetDeg_Far = 0x2aaa;
        } else {
            TargetDist_Near2 = 6.25f;
            TargetDist_Mid2 = 100.0f;
            TargetDeg_Near = 0x2aaa;
            TargetDeg_Mid = 0x1c71;
            TargetDeg_Far = 0x0e38;
        }
    } else {
        TargetDist_Near2 = 0.25f;
        TargetDist_Mid2 = 1.0f;
        TargetDeg_Near = 0x2aaa;
        TargetDeg_Mid = 0x2000;
        TargetDeg_Far = 0x1555;
    }
    party_under_cover = 0;
    nbaddies_can_see_players = 0;
    if (reset_restart != 0) {
        LevTime[0] = 0.0f;
        LevTime[1] = 0.0f;
        BonusCoinTotal = 0;
    }

    const i32 progress_index = WORLD->current_level->area_level_index;
    if (WORLD->api_object_sys != NULL)
        WORLD->api_object_sys->flags_210 &= ~1;
    if ((ResetBits & RESETBIT_CLEAR_LEVEL_PROGRESS) != 0) {
        GizmoSysClearLevelProgress(WORLD, progress_index);
    }

    GameAnimSys_ReStoreProgress(WORLD->game_anim_sys, progress_index);
    GizmoSysReset(WORLD->gizmo_sys, WORLD, progress_index);

    if ((ResetBits & RESETBIT_REINITIALISE_LEVEL) != 0) {
        DrawBossHitPoints(NULL);
        ResetForceBack();
        ClearAICreatures();
        GameAISysReset(WORLD->ai_sys);
        ReStoreStatusTakeOverObjectSys(1);
        AIScriptInitConditions(WORLD->ai_sys);

        for (i32 player_index = 0; player_index < 8; ++player_index) {
            if (Player[player_index] == NULL)
                continue;
            InitPlayerAI(Player[player_index]);
            char script_name[32];
            if (FreePlay != 0) {
                if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("Freeplay"), 0, 1, 0) != NULL)
                    strcpy(script_name, "Freeplay");
                else
                    goto party_script;
            } else if (Mission_Active(NULL) != NULL) {
                if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("Mission"), 0, 1, 0) != NULL)
                    strcpy(script_name, "Mission");
                else
                    goto party_script;
            } else {
                if (AIScriptFind(WORLD->ai_sys, Player[player_index]->apiobj.character_data->file, 0, 1, 0) != NULL) {
                    sprintf(script_name, Player[player_index]->apiobj.character_data->file);
                } else {
                    if ((Player[player_index]->apiobj.character_data->model_flags & 8) != 0)
                        strcpy(script_name, "Jedi");
                    else if ((Player[player_index]->apiobj.character_data->model_flags & 0x80) != 0)
                        strcpy(script_name, "Blaster");
                    else
                        strcpy(script_name, "NoWeapon");
                    if (AIScriptFind(WORLD->ai_sys, script_name, 0, 1, 0) == NULL) {
                    party_script:
                        if (AIScriptFind(WORLD->ai_sys, const_cast<char *>("party"), 0, 1, 0) != NULL)
                            strcpy(script_name, "party");
                        else
                            strcpy(script_name, "GeneralParty");
                    }
                }
            }
            AIScriptProcessorInit(WORLD->ai_sys, &Player[player_index]->ai,
                                  reinterpret_cast<AISCRIPTPROCESS *>(&Player[player_index]->ai), NULL, script_name,
                                  NULL, 1, NULL, NULL);
            if (FreePlay == 0 && Mission_Active(NULL) == NULL && Player[player_index]->field_0xcc0 == NULL &&
                AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&Player[player_index]->ai),
                                                 const_cast<char *>("InActive")) != 0) {
                Player[player_index]->apiobj.flags_high &= ~0x10;
            }
        }
    } else {
        ReStoreStatusTakeOverObjectSys(0);
    }

    if (makebaddiesforgetinresetbits != 0)
        MakeBaddiesForgetAboutParty(0);
    if ((ResetBits & 2) != 0) {
        for (i32 i = 0; i < 8; ++i) {
            if (Player[i] != NULL)
                Player[i]->field_0xe38 = 4;
        }
    }
    if ((ResetBits & 4) != 0) {
        for (i32 i = 0; i < 8; ++i) {
            if (Player[i] != NULL) {
                if (Player[i]->apiobj.player_controlled)
                    Player[i]->spawn_protection_timer = 2.5f;
                else
                    Player[i]->spawn_protection_timer = 0.0f;
            }
        }
    }
    ResetRadios();
    SpecialMiniKits_Reset(WORLD);
    SuperCounters_FixUpGizmos(WORLD);
    AITriggerSetSysReset(WORLD->ai_trigger_set_sys);
    AITriggerSysAutoSetUp(WORLD, WORLD->ai_trigger_set_sys);
    if ((ResetBits & 0x10) != 0) {
        for (i32 i = 0; i < 8; ++i) {
            if (Player[i] != NULL)
                ResetPlayer(Player[i], 1, NULL, 0);
        }
        VehicleAreaRememberSpeed = GetVehicleAreaRememberSpeed();
    }

    LevHSpecialExists = 0;
    for (i32 i = 0; i < 88; ++i) {
        if (NuSpecialExistsFn(&LevHSpecial[i]) != 0) {
            LevHSpecialExists |= static_cast<u64>(1) << (i & 63);
        }
    }
    WORLD->field_0x50d0 = 0;
    if (WORLD->current_level->reset_fn != NULL) {
        WORLD->current_level->reset_fn(WORLD);
    }
    CharPlatforms_Reset(WORLD->char_platform_sys);
    SetSoundFadeDist(WORLD, &Game.options_save);
    TempOptions.field3_0x3 = Game.options_save.field3_0x3;
    TempOptions.field4_0x4 = Game.options_save.field4_0x4;
    TempOptions.field5_0x5 = Game.options_save.field5_0x5;

    if (WORLD->level_progress != NULL && (WORLD->level_progress->flags & 2) == 0) {
        nugspline_s *spline = reinterpret_cast<nugspline_s *>(WORLD->portal_places[1]);
        if (spline != NULL && gone_through_door_to_new_mode == 0 && gone_through_door_to_new_level == 0 &&
            come_from_an_editor == 0) {
            GameCameraMakeMiniCut(spline, 0.0f, 2.0f, 0.0f, 1.0f, 0, 1);
        }
    }
    gone_through_door_to_new_mode = 0;
    come_from_an_editor = 0;
    if (newmode_cutinfo != NULL) {
        NewCutScene(newmode_cutinfo, WORLD->cutscene_sys, NULL, 0);
        newmode_cutinfo = NULL;
    }

    if (BonusArea != 0)
        Cheats_TurnOff(1);
    BonusCoinTarget = TotalLevelCoinTally(WORLD, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    if (BonusCoinTarget > 1000000u || (BonusArea != 0 && VehicleArea != 0 && BonusCoinTarget != 1000000u))
        BonusCoinTarget = 1000000u;
    if (netclient != 0)
        BonusCoinTarget = 1000000u;

    // Reset requests are edge-triggered. Leaving the bits set would rebuild the
    // level's AI and gizmo state again on every frame through Batman().
    ResetBits = 0;
    reset_reimport = 0;
    reset_restart = 0;
    CutScene_StartAudio();
    if (NOSOUND == 0)
        WORLD->reset_flags = 1;
    LookAtBoth = 0;
    oneAtOnce_SetNumAttackers(1);
    if (WORLD->level_progress != NULL) {
        if ((WORLD->level_progress->flags & 2) != 0)
            WORLD->field_0x5174 = (WORLD->level_progress->flags >> 2) & 1;
        ResetGizFlow(WORLD->giz_flow, &WORLD->level_progress->giz_flow_progress);
    } else {
        ResetGizFlow(WORLD->giz_flow, NULL);
    }
    EffectOffProgress_Reset(WORLD->level_progress);
    if (HUB_ADATA != NULL && WORLD->area == HUB_ADATA) {
        Tag_SetMode(3);
    } else {
        Tag_SetMode(1);
    }
    WeaponInOut_NoAIJediSfx = WORLD->area != NULL && WORLD->area == JEDI_ADATA ? 1 : 0;
    bonusmodearcade = 0;
    if (WORLD->area != NULL && (WORLD->area->flags & 0x10) != 0) {
        if (AreaGlobals.values.field_0x14 < AreaGlobals.values.field_0x0c) {
            AreaGlobals.values.field_0x14 = AreaGlobals.values.field_0x0c < 11 ? AreaGlobals.values.field_0x0c : 10;
        }
    }
    arcade_placed_stud_total = ((GizmoPickups_TotalScore(WORLD) * 85u) / 100u) / 1000u * 1000u;
}

void bgProcAbortAll() {
    STUBBED();
}

void bgprocUnFreeze() {
    STUBBED();
}

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern i16 id_LANDSPEEDER;
void PodDust(WORLDINFO_s *, GameObject_s *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

void AddSurfaceDebris(GameObject_s *object) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (object->field_0x7a5 == 0x2b)
        return;
    i32 layer = static_cast<i8>(object->apiobj.field_0x27f);
    if (layer != -1 && (TerLayer[layer].flags & 1) != 0)
        return;
    f32 surface_y = object->apiobj.field_0x218;
    if (surface_y == 2000000.0f)
        return;
    if ((PODRACE_ADATA != NULL && world->area == PODRACE_ADATA) || world->current_level == PODSPRINTA_LDATA) {
        PodDust(world, object);
        return;
    }
    i32 effect;
    f32 rate;
    f32 minimum_rate;
    f32 height;
    if (object->id == id_LANDSPEEDER) {
        effect = 102;
        rate = 20.0f;
        minimum_rate = 0.0f;
        height = 0.0f;
    } else if (WORLD->area != NULL && WORLD->area == HOTHBATTLE_ADATA) {
        effect = 86;
        rate = 100.0f;
        minimum_rate = 10.0f;
        height = 3.0f;
    } else if ((BONUS_GUNSHIP_ADATA != NULL && world->area == BONUS_GUNSHIP_ADATA) ||
               (GUNSHIP_ADATA != NULL && world->area == GUNSHIP_ADATA)) {
        effect = 141;
        rate = 75.0f;
        minimum_rate = 0.0f;
        height = 0.0f;
    } else {
        return;
    }
    f32 speed = object->apiobj.horizontal_velocity_magnitude;
    f32 run_speed = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->run_speed;
    if (world->debris_sys->entries[effect].effect == -1 || run_speed <= 0.0f)
        return;
    f32 height_scale = 1.0f;
    if (height > 0.0f) {
        height_scale = 1.0f - (object->apiobj.collision_min.y - surface_y) * (1.0f / height);
        if (height_scale <= 0.0f)
            return;
        if (height_scale > 1.0f)
            height_scale = 1.0f;
    }
    f32 emission_rate = (speed / run_speed) * rate;
    if (emission_rate < minimum_rate)
        emission_rate = minimum_rate;
    emission_rate *= height_scale;
    NUVEC current = {object->apiobj.position.x, surface_y, object->apiobj.position.z};
    NUVEC previous;
    if (object->apiobj.field_0x214 == 2000000.0f) {
        previous = current;
    } else {
        previous.x = object->apiobj.start_position.x;
        previous.y = object->apiobj.field_0x214;
        previous.z = object->apiobj.start_position.z;
    }
    i32 count = ParticlesPerSecond(emission_rate, FRAMETIME);
    if (count <= 0)
        return;
    NUVEC delta = {current.x - previous.x, current.y - previous.y, current.z - previous.z};
    do {
        f32 fraction = qrand() * (1.0f / 65535.0f);
        NUVEC position = {previous.x + delta.x * fraction, previous.y + delta.y * fraction,
                          previous.z + delta.z * fraction};
        AddVariableShotDebrisEffect(world->debris_sys->entries[effect].effect, &position, 1, 0, 0);
    } while (--count != 0);
}

void bgprocIsFreezing() {
    STUBBED();
}

extern "C" void DebFree(i32 *);
void DebFreeWithoutKey(debkeydatatype_s *key) {
    i32 handle = key->allocation_index;
    DebFree(&handle);
}

i32 CannotKill(GameObject_s *object);
extern "C" i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);
extern "C" i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);
extern "C" i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

void DebrisKillPlayers() {
    DebrisPreCheckCollisions(&GameCam->pos, 50.0f);
    for (i32 player_index = 0; player_index < 8; ++player_index) {
        GameObject_s *player = Player[player_index];
        if (player == NULL || (player->apiobj.flags_high & 0x10) == 0 || player->apiobj.field_0x287 != 0 ||
            player->field_0x1024 > 0.0f || player->spawn_protection_timer > 0.0f || (player->field_0xefe & 0x40) != 0 ||
            CannotKill(player) != 0 || Player_HasInvincibility(player) != 0 ||
            (player->apiobj.character_data->game_character->flags_090 & 0x04008000) != 0) {
            continue;
        }
        if (DebrisCollisionCheckScaleY(&player->apiobj.collision_position, player->apiobj.collision_radius,
                                       player->collision_y_scale) != -1) {
            ObjHitObj(NULL, player, 1, 0, 0, 1);
        }
        if (DebrisTorusCollisionCheckScaleY(&player->apiobj.collision_position, player->apiobj.collision_radius,
                                            player->collision_y_scale) != -1) {
            ObjHitObj(NULL, player, 1, 0, 0, 1);
        }
    }
}

void RndrUnfilledCircle(float, float, float, float, float, i32, float, float, numtl_s *) {
    STUBBED();
}

void DebrisProcessSpheres(uv1deb *data, float time, debinftype *effect, debkeydatatype_s *key, i32 finite) {
    if (!(time >= key->sphere_next_time))
        return;
    if (key->sphere_skip_count > 0) {
        --key->sphere_skip_count;
        return;
    }
    dma_particle_s *particle = reinterpret_cast<dma_particle_s *>(data);
    i32 index = key->field_2c8;
    key->process_spheres[index].position.x = particle->position.x + key->position.x;
    key->process_spheres[index].position.y = particle->position.y + key->position.y;
    key->process_spheres[index].position.z = particle->position.z + key->position.z;
    key->process_spheres[index].time = time;
    key->process_spheres[index].momentum = particle->momentum;
    if (++key->field_2c8 >= static_cast<i8>(effect->process_spheres))
        key->field_2c8 = 0;
    if (finite != 0)
        key->sphere_skip_count = key->field_2c8;
    key->sphere_next_time =
        time + effect->particle_lifetime / static_cast<f32>(static_cast<i8>(effect->process_spheres));
}

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40
static char nudl_html_buf[0x1000]; // flush threshold leaves room for formatted output
static char *nudl_html_cursor;     // original @0xb9d720-rel
static char *nudl_html_end;        // original @0xb9d730-rel
extern "C" {
    NUFILE hfh;
    char unknown[8] = "unknown";
}

void NuHtmlFlush(i32 force) {
    if ((nudl_html_cursor > nudl_html_end) | force) {
        NuFileWriteString(hfh, nudl_html_buf);
        nudl_html_cursor = nudl_html_buf;
        nudl_html_end = nudl_html_buf + 0xc00;
    }
}

extern "C" void NuHtmlBegin(void *file) {
    hfh = static_cast<NUFILE>(reinterpret_cast<usize>(file));
    nudl_html_cursor = nudl_html_buf;
    nudl_html_end = nudl_html_buf + 0xc00;
}

extern "C" void NuHtmlWrite(const char *text, ...) {
    if (text == NULL || text[0] == '\0') {
        text = unknown;
    }
    va_list ap;
    va_start(ap, text);
    vsprintf(nudl_html_cursor, text, ap);
    va_end(ap);
    nudl_html_cursor += NuStrLen(nudl_html_cursor);
    NuHtmlFlush(0);
}

extern "C" void NuHtmlHeading1(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#CFCFE5><tr><td><font face=arial size=+3> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(nudl_html_cursor, fmt, ap);
    va_end(ap);
    nudl_html_cursor += NuStrLen(nudl_html_cursor);
    NuHtmlWrite("</font></table>\n");
}

extern "C" void NuHtmlBanner(void) {
    NuHtmlWrite("<table   width=100%c height=20 bgcolor=#FF0000><tr><td><font face=arial size=+2></font></table>", '%');
}

extern "C" void NuHtmlHeading2(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#DFDFE5><tr><td><font face=arial size=+2> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(nudl_html_cursor, fmt, ap);
    va_end(ap);
    nudl_html_cursor += NuStrLen(nudl_html_cursor);
    NuHtmlWrite("</font></table>\n");
}

extern "C" void NuHtmlHeading3(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#FFDFE5><tr><td><font face=arial size=+0> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(nudl_html_cursor, fmt, ap);
    va_end(ap);
    nudl_html_cursor += NuStrLen(nudl_html_cursor);
    NuHtmlWrite("</font></table>\n");
}

extern "C" void NuHtmlEnd(void) {
    NuHtmlFlush(1);
    hfh = 0;
}

void NuHtmlTitle(char *title) {
    NuHtmlWrite("<HR><Center><B>");
    NuHtmlWrite(title);
    NuHtmlWrite("</B></Center><HR>");
    NuHtmlWrite("<P><P>");
}

extern "C" void NuHtmlBitmap(char *filename, i32 width, i32 height, char *caption, char *source) {
    char tag[256];
    NuHtmlWrite("<P>");
    if (height != 0 && width != 0) {
        sprintf(tag, "<IMG ALIGN=\"left\" HEIGHT=\"%d\" WIDTH=\"%d\" SRC=\"%s\">", height, width, filename);
    } else {
        sprintf(tag, "<IMG ALIGN=\"left\" SRC=\"%s\">", filename);
    }
    NuHtmlWrite(tag);
    if (caption != NULL) {
        NuHtmlWrite(caption);
    }
    NuHtmlWrite("<BR CLEAR=\"all\"> <P>&nbsp;<P>");
    if (source != NULL && NuStrCmp(filename, source) != 0) {
        NuFileCopy(filename, source);
    }
}

void NuHtmlGraphArray(char **strings) {
    char *text = *strings++;
    while (text != NULL) {
        NuHtmlWrite(text);
        text = *strings++;
    }
}

void AddChunkToRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack) {
    chunk->previous = NULL;
    chunk->next = NULL;

    particlechunkrendertype_s *current = *stack;
    if (current != NULL) {
        const u16 priority = static_cast<u16>(chunk->render_priority);
        const u16 current_priority = static_cast<u16>(current->render_priority);
        if (priority <= current_priority &&
            (priority != current_priority || current->effect->particle_type < chunk->effect->particle_type)) {
            particlechunkrendertype_s *next = current->next;
            while (next != NULL) {
                const u16 next_priority = static_cast<u16>(next->render_priority);
                if (next_priority <= priority &&
                    (priority != next_priority || chunk->effect->particle_type <= next->effect->particle_type)) {
                    break;
                }
                current = next;
                next = next->next;
            }
            current->next = chunk;
            chunk->previous = current;
            chunk->next = next;
            if (next != NULL) {
                next->previous = chunk;
            }
            return;
        }
        chunk->next = current;
        current->previous = chunk;
    }
    *stack = chunk;
}

void AddChunkControlToStack(debris_chunk_control_s *control, debris_chunk_control_s **stack) {
    debris_chunk_control_s *current = *stack;
    while (current != NULL && current->expiry_time < control->expiry_time) {
        stack = &current->next;
        current = current->next;
    }
    control->next = current;
    *stack = control;
}

extern "C" debkeydatatype_s *debris_keystack;

void AddDebrisEffectToStack(debkeydatatype_s *key) {
    if (key == NULL) {
        return;
    }
    if (debris_keystack != NULL) {
        debris_keystack->next = key;
    }
    key->previous = debris_keystack;
    debris_keystack = key;
}

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern f32 globaltime;
    extern f32 panelglobaltime;
}
void DebrisGetControlStackLock(void);
void DebrisReleaseControlStackLock(void);
void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

void DebFreeChunksInstantly(i32 *handle) {
    debkeydatatype_s *key = &debkeydata[*handle];
    if (key->effect_index == 0 || key->allocated_chunk_count == 0) {
        return;
    }
    debinftype *effect = debtab[key->effect_index];

    DebrisGetControlStackLock();
    for (i32 i = 0; i < key->allocated_chunk_count; ++i) {
        debris_chunk_control_s *control = freechunkcontrols[freechunkcontrolsptr++];
        control->particle_chunk = key->particle_chunks[i];
        control->active = effect->particle_type == 7 ? 9 : 2;
        control->owner = NULL;
        const bool panel_time = effect->time_group == 4;
        control->expiry_time = panel_time ? panelglobaltime : globaltime;
        AddChunkControlToStack(control, &debris_chunk_control_stack[panel_time ? 1 : 0]);
    }
    DebrisReleaseControlStackLock();

    const i32 render_chunk_count = debrischunks + debrischunksglass;
    for (i32 i = 0; i < render_chunk_count; ++i) {
        particlechunkrendertype_s *render_chunk = &ParticleChunkToRender[i];
        if (render_chunk->particle_chunk == key->particle_chunks[0]) {
            if (key->field_2f6 != 0) {
                RemoveChunkFromRenderStack(render_chunk, &ParticleChunkRenderStack[effect->time_group]);
            }
            render_chunk->particle_chunk = NULL;
            render_chunk->effect = NULL;
            render_chunk->key = NULL;
            break;
        }
    }

    for (i32 i = 0; i < key->allocated_chunk_count; ++i) {
        key->particle_chunks[i] = NULL;
    }
    key->particle_count = 0;
    key->allocated_chunk_count = 0;
    key->previous_particle_count = 0;
    key->previous_allocated_chunk_count = 0;
    key->controlled_chunk_count = 0;
    key->field_18a = 0;
}

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern i16 *freedebkeys;
    extern i32 freedebkeyptr;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];
    extern f32 globaltime;
    extern f32 panelglobaltime;
    extern f32 timeincrement;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
}

extern "C" void DebReAlloc2(debkeydatatype_s *);
extern "C" void DebReAlloc(debkeydatatype_s *, i32);
void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);
void DebrisReleaseControlStackLock(void);

void DebrisProcessAllocation() {
    for (debkeydatatype_s *key = debris_keystack; key != NULL; key = key->previous) {
        if (key->previous_particle_count != key->particle_count) {
            DebReAlloc2(key);
        }
    }
}

VARIPTR *DisplayListRenderBuffer() {
    return &rndrstream_free;
}

static i32 control_stack_lock;

void DebrisGetControlStackLock() {
    while (control_stack_lock != 0) {
        NuThreadSleep(1);
    }
    control_stack_lock = 1;
}

static particlechunkrendertype_s *FindParticleRenderChunk(dma_particle_chunk_s *particle_chunk) {
    const i32 count = debrischunks + debrischunksglass;
    for (i32 i = 0; i < count; ++i) {
        if (ParticleChunkToRender[i].particle_chunk == particle_chunk) {
            return &ParticleChunkToRender[i];
        }
    }
    return NULL;
}

static void ReleaseChunkControl(debris_chunk_control_s *control) {
    --freechunkcontrolsptr;
    freechunkcontrols[freechunkcontrolsptr] = control;
    control->particle_chunk = NULL;
}

i32 SolveQuadratic(f32, f32, f32, f32 *, f32 *);
void DebrisProcessControlChunks(i32 panel_time) {
    const f32 now = panel_time != 0 ? panelglobaltime : globaltime;
    debris_chunk_control_s **stack = &debris_chunk_control_stack[panel_time];

    DebrisGetControlStackLock();
    while (*stack != NULL && (*stack)->expiry_time <= now) {
        debris_chunk_control_s *control = *stack;
        *stack = control->next;
        control->next = NULL;

        if (control->active == 5) {
            debinftype *effect = debtab[control->effect_index];
            dma_particle_s *particle = &control->particle_chunk->particles[control->particle_index];
            const f32 time = control->collision_time;
            const f32 old_y = particle->momentum.y;
            const f32 gravity_velocity = 2.0f * (time * effect->field_0a0);
            const f32 new_y = -control->restitution * (old_y + gravity_velocity) - gravity_velocity;
            particle->momentum.y = new_y;
            particle->position.y -= time * new_y - time * old_y;
            f32 first, second;
            if (SolveQuadratic(effect->field_0a0, new_y, particle->position.y - control->collision_plane, &first,
                               &second) != 0) {
                const f32 next_time = first > second ? first : second;
                if (effect->particle_lifetime > next_time) {
                    const f32 interval = next_time - control->collision_time;
                    control->collision_time = next_time;
                    if (interval > 0.02f) {
                        control->expiry_time += interval;
                        AddChunkControlToStack(control, stack);
                        continue;
                    }
                }
            }
            ReleaseChunkControl(control);
            continue;
        }

        if (control->active == 6) {
            NUVEC normal = {1.0f, 0.0f, 0.0f};
            dma_particle_s *particle = &control->particle_chunk->particles[control->particle_index];
            NuVecRotateY(&normal, &normal, control->rotation_y);
            const f32 old_x = particle->momentum.x;
            const f32 old_z = particle->momentum.z;
            const f32 impulse = -(1.0f + control->restitution) * (old_x * normal.x + old_z * normal.z);
            const f32 new_x = impulse * normal.x + old_x;
            const f32 new_z = impulse * normal.z + old_z;
            particle->momentum.x = new_x;
            particle->momentum.z = new_z;
            particle->position.x -= new_x * control->collision_time - old_x * control->collision_time;
            particle->position.z -= new_z * control->collision_time - old_z * control->collision_time;
            ReleaseChunkControl(control);
            continue;
        }

        if (control->active == 0 || control->active == 7) {
            particlechunkrendertype_s *render = FindParticleRenderChunk(control->particle_chunk);
            if (render != NULL) {
                RemoveChunkFromRenderStack(render, &ParticleChunkRenderStack[render->effect->time_group]);
                render->particle_chunk = NULL;
                render->effect = NULL;
                render->key = NULL;
            }
            control->expiry_time += 0.1f;
            control->active = control->active == 7 ? 9 : 2;
            AddChunkControlToStack(control, stack);
            continue;
        }

        if (control->active == 1) {
            debkeydatatype_s *key = control->owner;
            debinftype *effect = debtab[key->effect_index];
            i32 chunk_index = 0;
            for (i32 i = 0; i < key->allocated_chunk_count; ++i) {
                if (key->particle_chunks[i] == control->particle_chunk) {
                    chunk_index = i;
                    break;
                }
            }

            const i32 old_chunk_count = key->allocated_chunk_count;
            for (i32 i = chunk_index; i + 1 < old_chunk_count; ++i) {
                key->particle_chunks[i] = key->particle_chunks[i + 1];
            }
            key->particle_chunks[old_chunk_count - 1] = NULL;
            --key->previous_allocated_chunk_count;
            --key->controlled_chunk_count;
            --key->allocated_chunk_count;

            if (key->allocated_chunk_count == 0) {
                key->particle_count = 0;
                key->previous_particle_count = 0;
                for (i32 slot = 0; slot < 8; ++slot) {
                    const i16 key_index = effect->particle_keys[slot];
                    if (key_index != -1 && &debkeydata[key_index] == key) {
                        key->effect_index = 0;
                        key->allocation_index = -1;
                        --freedebkeyptr;
                        freedebkeys[freedebkeyptr] = key_index;
                        effect->particle_keys[slot] = -1;
                    }
                }
                key->particle_chunks[0] = NULL;
            } else {
                const i32 particles_per_chunk = effect->particle_type == 7 ? 12 : 32;
                key->particle_count -= static_cast<i16>(particles_per_chunk);
                key->previous_particle_count -= static_cast<i16>(particles_per_chunk);
                if ((chunk_index + 1) * particles_per_chunk < key->field_18a) {
                    key->field_18a -= static_cast<i16>(particles_per_chunk);
                }
                LinkDmaParticalSets(key->particle_chunks, key->allocated_chunk_count);
            }

            if (chunk_index == 0) {
                particlechunkrendertype_s *render_chunk = FindParticleRenderChunk(control->particle_chunk);
                if (render_chunk != NULL) {
                    render_chunk->particle_chunk = key->particle_chunks[0];
                    if (key->allocated_chunk_count == 0) {
                        RemoveChunkFromRenderStack(render_chunk, &ParticleChunkRenderStack[effect->time_group]);
                        render_chunk->effect = NULL;
                        render_chunk->key = NULL;
                    }
                }
            }

            control->expiry_time += 0.1f;
            control->active = effect->particle_type == 7 ? 9 : 2;
            AddChunkControlToStack(control, stack);
            continue;
        }

        if (control->active == 2 || control->active == 9) {
            LinkDmaParticalSets(&control->particle_chunk, 1);
            control->active = control->active == 9 ? 8 : 3;
            control->expiry_time += 0.1f;
            AddChunkControlToStack(control, stack);
            continue;
        }

        if (control->active == 3) {
            --freedebchkptr;
            freedebchunks[freedebchkptr] = control->particle_chunk;
            for (dma_particle_s &particle : control->particle_chunk->particles) {
                particle.start_time = 10000000000.0f;
                particle.inverse_lifetime = 128.0f;
            }
            ReleaseChunkControl(control);
            continue;
        }

        if (control->active == 8) {
            --freedebchkptrg;
            freedebchunksglass[freedebchkptrg] = control->particle_chunk;
            for (i32 i = 0; i < 12; ++i) {
                control->particle_chunk->particles[i].start_time = 10000000000.0f;
                control->particle_chunk->particles[i].inverse_lifetime = 128.0f;
            }
            ReleaseChunkControl(control);
            continue;
        }

        // Unrecognized control states are removed from the active stack in the original.
    }
    DebrisReleaseControlStackLock();
}

extern "C" {
    extern i32 EDPP_MAX_TYPES;
    void edppDeleteEffect(i32);
}

void DebrisCleanUpDmaDebTypeTables() {
    static i32 count1;
    static i32 count2 = 1;
    if (++count1 > 5) {
        count1 = 0;
        debinftype *effect = debtab[count2];
        if (effect != NULL && effect->native_data != NULL) {
            f32 now = effect->time_group == 4 ? panelglobaltime : globaltime;
            if (now > effect->last_render_time + 5.0f) {
                DmaDebTypes[--freeDmaDebType] = effect->native_data;
                debtab[count2]->native_data = NULL;
                if (debtab[count2]->scale != 1.0f)
                    edppDeleteEffect(count2);
            }
        }
        if (++count2 >= EDPP_MAX_TYPES)
            count2 = 1;
    }
}

void DebrisReleaseControlStackLock() {
    control_stack_lock = 0;
}

void xxxNuDisplayListUpdateSpecial(nuhspecial_s *) {
    STUBBED();
}

static inline __attribute__((always_inline)) f32 DebrisInterpolateFloatKeys(const debris_float_key_s *keys, f32 time) {
    i32 first;
    i32 second;
    if (time >= keys[0].time && time <= keys[1].time) {
        first = 0;
        second = 1;
    } else if (time >= keys[1].time && time <= keys[2].time) {
        first = 1;
        second = 2;
    } else if (time >= keys[2].time && time <= keys[3].time) {
        first = 2;
        second = 3;
    } else if (time >= keys[3].time && time <= keys[4].time) {
        first = 3;
        second = 4;
    } else if (time >= keys[4].time && time <= keys[5].time) {
        first = 4;
        second = 5;
    } else if (time >= keys[5].time && time <= keys[6].time) {
        first = 5;
        second = 6;
    } else if (time >= keys[6].time && time <= keys[7].time) {
        first = 6;
        second = 7;
    } else {
        return 0.0f;
    }
    return keys[first].value + ((time - keys[first].time) / (keys[second].time - keys[first].time)) *
                                   (keys[second].value - keys[first].value);
}

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern f32 globaltime;
    extern NUVEC debris_collide_pt;
}

i32 DebrisSingleCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags) {
    debkeydatatype_s *key = &debkeydata[key_index];
    const i32 effect_index = key->effect_index;
    if (static_cast<u16>(effect_index + 1) <= 1)
        return 0;
    debinftype *effect = debtab[effect_index];
    if (effect == NULL || (effect->field_2f2 & flags) == 0)
        return 0;
    const i32 sphere_count = static_cast<i8>(effect->process_spheres);
    if (sphere_count <= 0)
        return 0;

    for (i32 i = 0; i < sphere_count; ++i) {
        debris_process_sphere_s *sphere = &key->process_spheres[i];
        f32 age = globaltime - sphere->time;
        if (age < 0.0f || age > effect->particle_lifetime)
            continue;

        debris_collide_pt.x = sphere->position.x + sphere->momentum.x * age;
        f32 acceleration = age * age;
        acceleration *= effect->field_0a0;
        debris_collide_pt.y = sphere->position.y + sphere->momentum.y * age + acceleration;
        debris_collide_pt.z = sphere->position.z + sphere->momentum.z * age;

        f32 effect_radius = DebrisInterpolateFloatKeys(effect->collision_keys, age / effect->particle_lifetime);
        if (effect_radius <= 0.0f)
            continue;

        f32 dx = position->x - debris_collide_pt.x;
        f32 dy = position->y - debris_collide_pt.y;
        f32 dz = position->z - debris_collide_pt.z;
        f32 combined_radius = radius + effect_radius;
        if (y_scale != 1.0f)
            dy *= combined_radius / (radius * y_scale + effect_radius);
        if (combined_radius * combined_radius > dx * dx + dy * dy + dz * dz)
            return 1;
    }
    return 0;
}

i32 DebrisSingleTorusCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags) {
    debkeydatatype_s *key = &debkeydata[key_index];
    const i32 effect_index = key->effect_index;
    if (static_cast<u16>(effect_index + 1) <= 1)
        return 0;
    debinftype *effect = debtab[effect_index];
    if (effect == NULL || (effect->field_2f2 & flags) == 0 || effect->torus_lifetime == 0.0f)
        return 0;

    f32 age =
        globaltime > key->emission_time ? globaltime - key->emission_time : globaltime - key->previous_emission_time;
    if (age <= 0.0f || age >= effect->torus_lifetime)
        return 0;
    f32 normalised_time = age / effect->torus_lifetime;
    f32 ring_radius = DebrisInterpolateFloatKeys(effect->torus_keys1, normalised_time) * effect->torus_radius1;
    f32 horizontal_radius = DebrisInterpolateFloatKeys(effect->torus_keys2, normalised_time) * effect->torus_radius2;
    f32 vertical_radius = DebrisInterpolateFloatKeys(effect->torus_keys3, normalised_time) * effect->torus_radius2;

    debris_collide_pt.x = position->x - key->position.x;
    debris_collide_pt.y = 0.0f;
    debris_collide_pt.z = position->z - key->position.z;
    NuVecNorm(&debris_collide_pt, &debris_collide_pt);
    NuVecScale(&debris_collide_pt, &debris_collide_pt, ring_radius);
    debris_collide_pt.x += key->position.x;
    debris_collide_pt.y += key->position.y;
    debris_collide_pt.z += key->position.z;

    f32 dx = position->x - debris_collide_pt.x;
    f32 dy = position->y - debris_collide_pt.y;
    f32 dz = position->z - debris_collide_pt.z;
    f32 combined_radius = radius + horizontal_radius;
    if (horizontal_radius != vertical_radius || y_scale != 1.0f)
        dy *= combined_radius / (radius * y_scale + vertical_radius);
    return combined_radius * combined_radius > dx * dx + dy * dy + dz * dz;
}

void unref(unsigned char *, unsigned char *) {
    STUBBED();
}

void TBRESET() {
    TBGAMECOUNT = 0;
    TBDRAWCOUNT = 0;
    TBPLAYERCOUNT = 0;
    TBAICOUNT = 0;
}

void TBOPENFN(char *, i32) {
    STUBBED();
}

void RndrArrow(float, float, float, i32, i32) {
    STUBBED();
}

void TBCLOSEFN(char *, i32) {
    STUBBED();
}
