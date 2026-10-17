// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#if defined(__SSE__)
#include <xmmintrin.h>
#endif
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "legoapi/misc/supportall.h"
#include "legoapi/render/core/screen.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/android/nuptl_android.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/actions/combat/hits.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include "decomp.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/bgproc.h"
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
#include "legoapi/render/core/rtl.h"
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
#include "nu2api/nu3d/nupostparams.h"

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
// The reference defines these with dynamic initialization in this unit.
MemoryManager theMemoryManager;
#include "legoapi/misc/supportall_types.h"
BIGBUFFER_s BigBuffer;
void *theThingManager = NULL;

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
extern u32 arcade_placed_stud_total;
extern f32 DEFAULT_MOVE_RANGE;
extern f32 drop_back_in_timer;
extern i32 last_chatter_sfx;
extern i32 FalconDebKey[2];
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
extern "C" ripple_set_s *ripples;
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

DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

static TIMINGBAR_LABEL GameTB[12];
static TIMINGBAR_LABEL PlayerTB[12];
static TIMINGBAR_LABEL AITB[12];
static TIMINGBAR_LABEL DrawTB[12];
extern i32 app_tbgameset;
extern i32 app_tbplayerset;
extern i32 app_tbaiset;
extern i32 app_tbdrawset;

static inline void TBSTART(i32 slot, i32 type) {
    i32 set;
    switch (type) {
        case 2:
            set = app_tbgameset;
            break;
        case 3:
            set = app_tbplayerset;
            break;
        case 4:
            set = app_tbaiset;
            break;
        case 5:
            set = app_tbdrawset;
            break;
        default:
            return;
    }
    _NuTimeBarSlotBegin(set, slot, NULL);
}

static inline void TBEND(i32 slot, i32 type) {
    i32 set;
    switch (type) {
        case 2:
            set = app_tbgameset;
            break;
        case 3:
            set = app_tbplayerset;
            break;
        case 4:
            set = app_tbaiset;
            break;
        case 5:
            set = app_tbdrawset;
            break;
        default:
            return;
    }
    _NuTimeBarSlotEnd(set, slot);
}

extern f32 TargetDist_Near2;
extern f32 TargetDist_Mid2;
extern u16 TargetDeg_Near;
extern u16 TargetDeg_Mid;
extern u16 TargetDeg_Far;
extern i32 party_under_cover;
extern i32 makebaddiesforgetinresetbits;
void MakeBaddiesForgetAboutParty(i32);
extern i32 nbaddies_can_see_players;
extern i32 reset_reimport;

void CatchUpCode(GameObject_s *object, f32 divisor, f32 maximum, i32 mode);

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

i32 SuperWeirdo(GameObject_s *object);

void bgProcClose() {
}

extern "C" void edrtlCalculateBurnoutEx(burnset_s *set, NuBloomParameters *parameters, NUVEC *camera_position,
                                        f32 frame_time);

void BurnoutApply(i32 paused);

void bgprocFreeze() {
}

void FindSlamOrigin(GameObject_s *, NUVEC *, NUVEC *);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);

void AddSlamDebris(GameObject_s *object);

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
    f32 low_weight;
    f32 high_weight;
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
    if (interpolated_value != 0.0f) {
        state[0xf79] = 0x21;
        if (current_character->field_0x60 <= current_character->field_0x5c) {
            *reinterpret_cast<i16 *>(state + 0xf70) = static_cast<i16>(current_character->field_0x5c * 32768.0f);
            *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(current_character->field_0x60 * 32768.0f);
        } else {
            *reinterpret_cast<i16 *>(state + 0xf70) = static_cast<i16>(current_character->field_0x60 * 32768.0f);
            *reinterpret_cast<i16 *>(state + 0xf76) = static_cast<i16>(current_character->field_0x5c * 32768.0f);
        }
    } else {
        state[0xf79] = 0;
    }
    object->field_0x1089 = current_index + 1;
}

static inline void TexQuadSubmit3D(f32 x, f32 y, i32 colour, i32 u, i32 v) {
    VARIPTR *stream = g_NuPrim_StreamBufferPtr;
    TexQuadVertex *vertex = static_cast<TexQuadVertex *>(stream->void_ptr);
    if (g_NuPrim_NeedsOverbrightening == 0) {
        vertex->colour = ((colour >> 1) & 0x7f7f7f) | (colour & 0xff000000);
    } else {
        vertex->colour = colour;
    }
    if (g_NuPrim_NeedsHalfUVs == 0) {
        vertex->uv[0] = static_cast<f32>(u);
        vertex->uv[1] = static_cast<f32>(v);
    } else {
        vertex->half_uv[0] = u != 0 ? 0x3c00 : 0;
        vertex->half_uv[1] = v != 0 ? 0x3c00 : 0;
    }
    vertex->x = x;
    vertex->y = y;
    vertex->z = 0.0f;
    stream->u8_ptr += sizeof(TexQuadVertex);
    ++g_NuPrim_VertexCount;
}

void CheckResetBits();

void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material) {
    NuPrim3DBegin(1, 7, material, reinterpret_cast<NUMTX *>(const_cast<VuMtx *>(&matrix)));
    TexQuadSubmit3D(-0.5f, -0.5f, colour, 0, 0);
    TexQuadSubmit3D(0.5f, -0.5f, colour, 1, 0);
    TexQuadSubmit3D(-0.5f, 0.5f, colour, 0, 1);
    TexQuadSubmit3D(0.5f, 0.5f, colour, 1, 1);
    NuPrim3DEnd();
}

void bgProcAbortAll() {
}

void bgprocUnFreeze() {
}

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern "C" i16 id_LANDSPEEDER;
void PodDust(WORLDINFO_s *, GameObject_s *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 bgprocIsFreezing() {
    i32 freezing = 1;
    if (bgproc_frozen == 0)
        freezing = bgproc_freeze != 0;
    return freezing;
}

void AddSurfaceDebris(GameObject_s *object);

extern "C" void DebFree(i32 *);
void DebFreeWithoutKey(debkeydatatype_s *key);

i32 CannotKill(GameObject_s *object);
extern "C" i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);
extern "C" i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);
extern "C" i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

void DebrisKillPlayers();

i32 RndrUnfilledCircle(f32 x, f32 y, f32 radius, f32 border_width, f32 aspect, i32 colour, f32 progress, f32 z,
                       numtl_s *material) {
    NuPrim2DBegin(1, 7, material);

    radius *= 0.5f;
    const f32 inner_radius = radius - border_width * 0.5f;

    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 1.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * x, static_cast<f32>(PS2_VREZ_H) * (y - radius), z);

    NuRndrPrimSetColour(colour);
    NuRndrPrimUV(0.0f, 0.0f);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * x, static_cast<f32>(PS2_VREZ_H) * (y - inner_radius), z);

    const f32 segments = progress * 360.0f;
    // Retail CVTTSS2SI returns integer-indefinite for unordered/out-of-range
    // inputs. The touch widgets actually pass packed colours as progress;
    // the intrinsic keeps that no-segment result well defined.
#if defined(__SSE__)
    const i32 segment_count = _mm_cvtt_ss2si(_mm_set_ss(segments));
#else
    const i32 segment_count =
        segments >= -2147483648.0f && segments < 2147483648.0f ? static_cast<i32>(segments) : INT_MIN;
#endif
    if (segment_count >= 0) {
        f32 angle = 0.0f;
        for (i32 segment = 0; segment <= segment_count; ++segment, angle += 0.017455555f) {
            const f32 sine = NuSinf(angle);
            const f32 negative_cosine = -NuCosf(angle);
            const f32 outer_x = radius * sine * aspect + x;
            const f32 outer_y = radius * negative_cosine + y;
            const f32 inner_x = inner_radius * sine * aspect + x;
            const f32 inner_y = inner_radius * negative_cosine + y;

            NuRndrPrimSetColour(colour);
            NuRndrPrimUV(0.0f, 1.0f);
            NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * outer_x, static_cast<f32>(PS2_VREZ_H) * outer_y, z);

            NuRndrPrimSetColour(colour);
            NuRndrPrimUV(0.0f, 0.0f);
            NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * inner_x, static_cast<f32>(PS2_VREZ_H) * inner_y, z);
        }
    }

    NuPrim2DEnd();
    return 1;
}

void DebrisProcessSpheres(uv1deb *data, float time, debinftype *effect, debkeydatatype_s *key, i32 finite);

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40
static char buffer[0x1000]; // flush threshold leaves room for formatted output
static char *txt;           // original @0xb9d720-rel
static char *flush;         // original @0xb9d730-rel
extern "C" {
    NUFILE hfh;
    char unknown[8] = "unknown";
}

void NuHtmlFlush(i32 force) {
    if ((txt > flush) | force) {
        NuFileWriteString(hfh, buffer);
        txt = buffer;
        flush = buffer + 0xc00;
    }
}

extern "C" void NuHtmlBegin(void *file) {
    hfh = static_cast<NUFILE>(reinterpret_cast<usize>(file));
    txt = buffer;
    flush = buffer + 0xc00;
}

extern "C" void NuHtmlWrite(const char *text, ...) {
    if (text == NULL || text[0] == '\0') {
        text = unknown;
    }
    va_list ap;
    va_start(ap, text);
    vsprintf(txt, text, ap);
    va_end(ap);
    txt += NuStrLen(txt);
    NuHtmlFlush(0);
}

extern "C" void NuHtmlHeading1(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#CFCFE5><tr><td><font face=arial size=+3> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(txt, fmt, ap);
    va_end(ap);
    txt += NuStrLen(txt);
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
    vsprintf(txt, fmt, ap);
    va_end(ap);
    txt += NuStrLen(txt);
    NuHtmlWrite("</font></table>\n");
}

extern "C" void NuHtmlHeading3(const char *fmt, ...) {
    if (fmt == NULL || fmt[0] == '\0') {
        fmt = unknown;
    }
    NuHtmlWrite("<table width=100%c bgcolor=#FFDFE5><tr><td><font face=arial size=+0> ", '%');
    va_list ap;
    va_start(ap, fmt);
    vsprintf(txt, fmt, ap);
    va_end(ap);
    txt += NuStrLen(txt);
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
    if (!(source != NULL && NuStrCmp(filename, source) != 0)) {
        return;
    }
    NuFileCopy(filename, source);
}

void NuHtmlGraphArray(char **strings) {
    char *text = *strings++;
    while (text != NULL) {
        NuHtmlWrite(text);
        text = *strings++;
    }
}

void AddChunkToRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

void AddChunkControlToStack(debris_chunk_control_s *control, debris_chunk_control_s **stack);

extern "C" debkeydatatype_s *debris_keystack;

void AddDebrisEffectToStack(debkeydatatype_s *key);

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
void RemoveAnyChunkControls(i32 *);
void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

void DebFreeChunksInstantly(i32 *handle);

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

void DebrisProcessAllocation();

VARIPTR *DisplayListRenderBuffer() {
    return &rndrstream_free;
}


void DebrisGetControlStackLock();



i32 SolveQuadratic(f32, f32, f32, f32 *, f32 *);
void DebrisProcessControlChunks(i32 panel_time);

extern "C" {
    extern i32 EDPP_MAX_TYPES;
    void edppDeleteEffect(i32);
}

void DebrisCleanUpDmaDebTypeTables();

void xxxNuDisplayListUpdateSpecial(nuhspecial_s *special) {
    NUDLDLISTSCENE *scene = special->scene->display_list;
    NUDISPLAYSPECIAL *display = special->display_special;
    NUMTX_ALIGNED16 matrix = *NuSpecialGetDrawMtx(special);
    NUVEC corners[8] = {
        {display->bounds_min.x, display->bounds_min.y, display->bounds_min.z},
        {display->bounds_max.x, display->bounds_max.y, display->bounds_max.z},
        {display->bounds_min.x, display->bounds_min.y, display->bounds_max.z},
        {display->bounds_max.x, display->bounds_max.y, display->bounds_min.z},
        {display->bounds_min.x, display->bounds_max.y, display->bounds_min.z},
        {display->bounds_min.x, display->bounds_max.y, display->bounds_max.z},
        {display->bounds_max.x, display->bounds_min.y, display->bounds_min.z},
        {display->bounds_max.x, display->bounds_min.y, display->bounds_max.z},
    };
    NuVecMtxTransform(&corners[0], &corners[0], &matrix);
    NuVecMtxTransform(&corners[1], &corners[1], &matrix);
    NuVecMtxTransform(&corners[2], &corners[2], &matrix);
    NuVecMtxTransform(&corners[3], &corners[3], &matrix);
    NuVecMtxTransform(&corners[4], &corners[4], &matrix);
    NuVecMtxTransform(&corners[5], &corners[5], &matrix);
    NuVecMtxTransform(&corners[6], &corners[6], &matrix);
    NuVecMtxTransform(&corners[7], &corners[7], &matrix);

    NUVEC minimum;
    NUVEC maximum;
    NuVecMin(&minimum, &corners[0], &corners[1]);
    NuVecMin(&minimum, &minimum, &corners[2]);
    NuVecMin(&minimum, &minimum, &corners[3]);
    NuVecMin(&minimum, &minimum, &corners[4]);
    NuVecMin(&minimum, &minimum, &corners[5]);
    NuVecMin(&minimum, &minimum, &corners[6]);
    NuVecMin(&minimum, &minimum, &corners[7]);
    NuVecMax(&maximum, &corners[0], &corners[1]);
    NuVecMax(&maximum, &maximum, &corners[2]);
    NuVecMax(&maximum, &maximum, &corners[3]);
    NuVecMax(&maximum, &maximum, &corners[4]);
    NuVecMax(&maximum, &maximum, &corners[5]);
    NuVecMax(&maximum, &maximum, &corners[6]);
    NuVecMax(&maximum, &maximum, &corners[7]);

    if ((scene->render_buffer & NUDL_SCENE_RENDER_FLAG_CENTER_EXTENT_BOUNDS) != 0) {
        NUVEC extent;
        extent.y = (maximum.y - minimum.y) * 0.5f;
        extent.x = (maximum.x - minimum.x) * 0.5f;
        extent.z = (maximum.z - minimum.z) * 0.5f;
        const f32 radius = NuFsqrt(extent.x + extent.y + extent.z);
        NUCLIPBOUNDS *bounds = &scene->clip_bounds[display->instance_ix];
        bounds->center.x = (maximum.x + minimum.x) * 0.5f;
        bounds->center.y = (maximum.y + minimum.y) * 0.5f;
        bounds->center.z = (maximum.z + minimum.z) * 0.5f;
        bounds->center_w = radius;
        bounds->extent = extent;
    } else {
        NUCLIPBOUNDS *bounds = &scene->clip_bounds[display->instance_ix];
        bounds->center = minimum;
        bounds->extent = maximum;
        // Retail copies uninitialized local W components in min/max mode.
        // They are not clipping coordinates; retain the destination padding,
        // as the active NuDisplayListUpdateSpecial implementation does.
    }
    DisplayListUpdateSpecialTransformPS(special, &matrix);
}

void DebrisReleaseControlStackLock();

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern f32 globaltime;
    extern NUVEC debris_collide_pt;
}

i32 DebrisSingleCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags);

i32 DebrisSingleTorusCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags);

void TBRESET() {
    TBGAMECOUNT = 0;
    TBDRAWCOUNT = 0;
    TBPLAYERCOUNT = 0;
    TBAICOUNT = 0;
}

void TBOPENFN(char *name, i32 type) {
    TIMINGBAR_LABEL *labels;
    i32 *count;
    switch (type) {
        case 2:
            labels = GameTB;
            count = &TBGAMECOUNT;
            break;
        case 3:
            labels = PlayerTB;
            count = &TBPLAYERCOUNT;
            break;
        case 4:
            labels = AITB;
            count = &TBAICOUNT;
            break;
        case 5:
            labels = DrawTB;
            count = &TBDRAWCOUNT;
            break;
        default:
            return;
    }
    const i32 existing_count = *count;
    for (i32 index = 0; index < existing_count; ++index) {
        if (NuStrCmp(name, labels[index].name) == 0) {
            TBSTART(index, type);
            return;
        }
    }
    if (!(*count < 12)) {
        return;
    }
    char label[256];
    NuStrCpy(label, name);
    i32 length = NuStrLen(label);
    if (!(length > 0)) {
        return;
    }
    if (length > 10)
        label[10] = '\0';
    NuStrCpy(labels[*count].name, label);
    labels[*count].type = static_cast<u8>(type);
    TBSTART(*count, type);
    ++*count;
}

void RndrArrow(float x, float y, float scale, i32 angle, i32 colour) {
    NUVEC points[4] = {};
    points[0].x = -1.0f;
    points[0].y = -1.0f;
    points[1].y = 1.0f;
    points[2].y = -0.5f;
    points[3].x = 1.0f;
    points[3].y = -1.0f;
    const f32 aspect = GetAspectRatio();
    NuVecRotateZ(&points[0], &points[0], angle);
    NuVecRotateZ(&points[1], &points[1], angle);
    NuVecRotateZ(&points[2], &points[2], angle);
    NuVecRotateZ(&points[3], &points[3], angle);
    points[0].x *= scale;
    points[0].y *= scale;
    points[1].x *= scale;
    points[1].y *= scale;
    points[2].x *= scale;
    points[2].y *= scale;
    points[3].x *= scale;
    points[3].y *= scale;
    points[0].x *= aspect;
    points[1].x *= aspect;
    points[2].x *= aspect;
    points[3].x *= aspect;
    points[0].x += x;
    points[0].y += y;
    points[1].x += x;
    points[1].y += y;
    points[2].x += x;
    points[2].y += y;
    points[3].x += x;
    points[3].y += y;
    NuPrim2DBegin(1, 5, NULL);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[0].x, static_cast<f32>(PS2_VREZ_H) * points[0].y, 0.0f);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[1].x, static_cast<f32>(PS2_VREZ_H) * points[1].y, 0.0f);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[2].x, static_cast<f32>(PS2_VREZ_H) * points[2].y, 0.0f);
    NuRndrPrimSetColour(colour);
    NuPrim2DAddXYZ(static_cast<f32>(PS2_VREZ_W) * points[3].x, static_cast<f32>(PS2_VREZ_H) * points[3].y, 0.0f);
    NuPrim2DEnd();
}

void TBCLOSEFN(char *name, i32 type) {
    TIMINGBAR_LABEL *label;
    switch (type) {
        case 2:
            label = GameTB;
            for (i32 index = 0, count = TBGAMECOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
        case 3:
            label = PlayerTB;
            for (i32 index = 0, count = TBPLAYERCOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
        case 4:
            label = AITB;
            for (i32 index = 0, count = TBAICOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
        case 5:
            label = DrawTB;
            for (i32 index = 0, count = TBDRAWCOUNT; index < count; ++index, ++label) {
                if (NuStrCmp(name, label->name) == 0) {
                    TBEND(index, type);
                    return;
                }
            }
            break;
    }
}

// ==== src/legoapi/items/objects/gameobjects.cpp: declarations ====
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/items/objects/objectsall.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/actions/character/streaks.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/audio/audio.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/screens/arcade.h"
#include "decomp.h"
#include "gameapi/ai/aisys/aisys.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/world/mission.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/world/world.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/gizmos/trigger/gizspecial.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/door/spinner.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/store.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/core/input/timing.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nu3d/nucamera.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"

#include <stdio.h>
#include <string.h>

void SetObjAsHeadTarget(GameObject_s *, GameObject_s *, i8, f32, f32, f32);
void SetBallooningHeight(GameObject_s *, f32);
void GameObjectSetCanUse(GameObject_s *, void *, u8, u8, f32);
i32 Suit_GetIndex(SUIT_s *);
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
void Bolt_Shoot(GameObject_s *, i32, i32);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
void PartUpdate_Basketball(PART_s *);
void PartImpact_Basketball(PART_s *);
void AddSlamDebris(GameObject_s *);
void Batarang_Release(GameObject_s *, i32);
void ThermalDetonator_Throw(GameObject_s *);
void BobaRocket_Kill(PART_s *, i32);
void BobaRocket_Move(PART_s *, f32);
void BobaRocket_Deflect(PART_s *);
void PartCollide_3D(PART_s *);
void ConfigureComplexShadow(GameObject_s *);
f32 DropInOutScale(GameObject_s *);
extern f32 rocket_speed;
extern "C" f32 sabrerubwait;
NUVEC *GetZapOrigin(GameObject_s *);
void PlaySabreSfx(char *, GameObject_s *, NUVEC *, i32);
extern i32 testlaser_type;
extern i32 lightning_type;
extern f32 testlaser_endw;
extern f32 testlaser_sizewab;
extern f32 testlaser_sizel;
extern f32 testlaser_sizew;
extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;
extern i16 id_MOUSEDROID;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;
extern i32 VehicleArea;
extern i32 TERRAINMASK_NONWEAPON, TERRAINMASK_NONDROID;
extern f32 DEFENDTIME;
extern f32 jump_stuck_time;
extern i32 teleport_all_freeplay_modes;
extern i32 drop_in_teleport;


extern "C" {
    void AddToAIGroup(AIGROUP_s *group, APIOBJECT_s *object);
    extern NUVEC plr_lastpos;
    extern i16 id_BAT;
    extern i16 id_SNAKE;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_BASKETCANNON;
    extern i16 id_R2Q5;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    extern i32 FalconDebKey[2];
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).
extern void *theThingManager;
extern void oneAtOnce_MaintainArray();

void legoSetMusicVolume(float);
void MovePlayer(GameObject_s *object);
void AnimatePlayer(GameObject_s *object);
void TerrainPlayer(GameObject_s *object);
void KeepOnScreen(GameObject_s *object);
void SetPlayer();
void InitPlayerAI(GameObject_s *object);
void ResetPlayerMoves(GameObject_s *object);
void MovePlayerSpline(GameObject_s *object);
void GetTopBot(GameObject_s *object);
void ResetRumble(RUMBLEPACKET *packet);
void InitSurfaceInfo(GameObject_s *object);
i32 TightRope_SnapTo(GameObject_s *object, NUVEC *position);
void Player_ClearContext(GameObject_s *object, i32 mode);
void Player_ResetContexts(PLAYERPACKET_s *packet);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
void StarWars_GameAISysInit();
void GameAISysSetGame();
void ClearAICreatures();
void CollideGameObjects(WORLDINFO_s *world);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
i32 TagCode(GameObject_s *source, GameObject_s *target, i32 takeover, i32 blend, i32 mode);
extern i32 do_player_tag;
extern f32 player_tag_timer;
extern GameObject_s *player_tag_from;
extern GameObject_s *player_tag_to;
APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);
i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special);
void AIPathCnxControlSysReset(AIPATHCNXCONTROLSYS_s *system);
void AIPathCnxHelperSysReset(WORLDINFO_s *world, AIPATHCNXHELPERSYS_s *system);
void InitAICreatures(AISYS_s *system);
void ResetAICreatures(AISYS_s *system);
void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process);
void GizmoSysAddGizmos(GIZMOSYS_s *gizmo_sys, GIZFLOW_s *giz_flow, void *world);
void UpdateCoinPacket(COINPACKET_s *packet, i32 active, i32 player_index);
void ResetCoinPacket(COINPACKET_s *packet);
GIZMOPICKUP_s *GizmoPickups_Collide(WORLDINFO_s *world, GameObject_s *object, i32 arg);

f32 Condition_InHubArea(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);
void *Condition_InHubAreaInit(AISYS_s *, char *, AISCRIPT_s *);

extern i32 LEGO_AIPATHCNX_BLOCKAGE;

extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

extern GAMEFOG_STATE GameFog;
extern f32 dof_fstop;


#include "legoapi/items/objects/gameobjects_types.h"






// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.
extern i32 timebase_updates;


extern TERRSET *CurTerr;


















































extern "C" i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *cutscene);












extern "C" f32 NuAnimEndFrameOld(void *animation);

extern i32 Hub_GetRandomCharType();
extern u8 hub_custodians_finished_loading;
extern "C" {
extern i32 nbaddies_can_see_players;
}














































i32 CanFightLikeAJedi(GameObject_s *object);












































extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

extern f32 default_path_heighttol;
extern u8 default_activate_difficulty;
extern u8 default_min_n_respawns;
extern u8 default_max_n_respawns;
extern f32 default_min_t_respawn;
extern f32 default_max_t_respawn;

extern CHARACTERNAMEFN * LevelCharacterNameFn;
extern CHARACTERNAMEFN * SpecialRouteCharacterNameFn;
extern CHARACTERGLOBALIDFN * LevelCharacterGlobalIDFn;
extern GLOBALCHARACTERNAMEFN * GlobalCharacterNameFn;
extern CHARACTERHGOBJFN * GlobalCharacterHGobjFn;
extern CHARACTERRENDERFN * GlobalCharacterRenderFn;
extern CHARACTERGOALSPEEDFN * GetCharacterGoalSpeedFn;
extern CHARACTERTYPEIDFN * LevelCharacterTypeIDFn;

extern AICHARACTERTYPEID * GlobalCharacterTypeIDFn;
extern AISPECIALROUTECHARACTERTYPEID * SpecialRouteCharacterTypeIDFn;
extern AICHARACTERDISTANCE * GetViewRangeFn;
extern AICHARACTERDISTANCE * GetHearDistanceFn;
extern AICHARACTERDISTANCE * GetMaxViewHeightFn;
extern AICHARACTERDISTANCE * GetMinViewHeightFn;
extern GAMEAILOAD * GameAILoadFn;
extern AIACTIONPARSESPEED * AIActionParseSpeedFn;
extern AIBIGJUMPTODESTINATION * AIBigJumpToDestinationFn;
extern AIRESPAWNONPATH * AIRespawnOnPathFn;
extern AICLEARCREATURES * ClearAICreaturesFn;
extern APIOBJECTFROMOBJID * APIOBJECTFromObjIDFn;
extern AIFINDALTERNATIVESPECIALOBJECT * FindAlternativeSpecialObjectFn;
extern AIGETNAMEDAPIOBJECT * GetNamedAPIObjectFn;
extern AIGETCREATUREORIGIN * GetAICreatureOriginFn;
}


// The special-route list reserves the first ten ids for suit characters.  The
// remaining ids enumerate the current story list while omitting variants that
// are represented by those dedicated routes.






















extern i32 TimingBarSet;
extern "C" i32 SHADOWCALLS;
extern u32 LAYER_HOVERIGNORE;

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask);

extern "C" f32 MainRenderTime;
extern "C" f32 MainRenderTargetTime;
extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;
extern void (*BackDrop_AlphaFn)(f32 *alpha);
extern void BackDrop_UpdateColours(i32 instant);
extern i32 Paused;
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

void UpdateGameMessages();
extern "C" i32 DoubleScore;
extern FadeSystem FadeSys;
extern f32 LevelNameTime, LevelNameMul;
extern i32 pause_rndr_on;
extern "C" TIMER BonusTimer;
extern "C" i32 screendump;

void GameTiming(WORLDINFO_s *world, float *game_time);

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);
extern "C" i32 RAYCASTCALLS;

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask);

extern f32 draw_attention_distance;
extern f32 party_follow_offsets[8];
extern i32 active_neutral_count;
extern "C" i32 party_under_cover;
extern "C" i32 party_cant_be_under_cover;
extern AREADATA *VADER_ADATA;
extern GameObject_s *alert_obj;
void AITriggerSetSysProcess(AITRIGGERSETSYS_s *system);
i32 Hub_InMenu();
extern i32 test_new_interaction;
extern i32 debug_ignore_players;
extern i32 WALKSTEALTH;
extern i32 active_goody_count;
extern i32 active_baddy_count;
extern f32 timetogetlos;
void GameCreatureOpponentSelection(AISYS_s *, i32, APIOBJECT **, i32, APIOBJECT **, i32, APIOBJECT **, u64, f32);
void GizTurrets_OpponentSelection(GIZTURRETSYS_s *, i32, APIOBJECT **, i32, APIOBJECT **);

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.
static GAMEOBJECTS_CLONE_ATTR2_SSE void TestWalkAround(APIOBJECT *object, f32 *other_x, f32 *other_z,
                                                       NUVEC *difference, f32 radius)
    __asm__("_ZL14TestWalkAroundP11APIOBJECT_sS0_P7nuvec_sf.isra.10");


void GameAIProcess();

extern "C" {
void InitFn_LevelCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_SpecialRouteCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_LevelCharacterName(CHARACTERNAMEFN *function);

void InitFn_SpecialRouteCharacterName(CHARACTERNAMEFN *function);

void InitFn_LevelCharacterGlobalID(CHARACTERGLOBALIDFN *function);

void InitFn_GlobalCharacterTypeID(CHARACTERTYPEIDFN *function);

void InitFn_GlobalCharacterName(GLOBALCHARACTERNAMEFN *function);

void InitFn_GlobalCharacterRender(CHARACTERRENDERFN *function);

void InitFn_GlobalCharacterHGobj(CHARACTERHGOBJFN *function);

void InitFn_ClearAICreatures(AICLEARCREATURES *function);

void InitFn_GetCharacterGoalSpeedFn(CHARACTERGOALSPEEDFN *function);

void InitFn_GetViewRange(CHARACTERDISTANCEFN *function);

void InitFn_GetHearDistance(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMaxViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GlobalGetMinViewHeight(CHARACTERDISTANCEFN *function);

void InitFn_GameAILoad(GAMEAILOAD *function);

void InitFn_AIActionParseSpeed(AIACTIONPARSESPEED *function);

void InitFn_AIRespawnOnPath(AIRESPAWNONPATH *function);

void InitFn_AIBigJumpToDestination(AIBIGJUMPTODESTINATION *function);

void InitFn_FindAlternativeSpecialObjectFn(AIFINDALTERNATIVESPECIALOBJECT *function);

void InitFn_APIOBJECTFromObjIDFn(APIOBJECTFROMOBJID *function);

void InitFn_GetNamedAPIObject(AIGETNAMEDAPIOBJECT *function);

void InitFn_GetAICreatureOrigin(AIGETCREATUREORIGIN *function);

void SetScriptErrorLevel(SCRIPT_ERROR_LEVEL level);
}

extern f32 OFFSCREEN_CATCHUP_TIME;
extern f32 drop_back_in_timer;

i32 TryToTeleportToNextNode(GameObject_s *object, AIPATHNODE_s *node, i32 tightrope);

void SetSpecialMove(GameObject_s *object, AIPATHNODE_s *target, AIPATHNODE_s *node, char move);

void ClearSpecialMove(GameObject_s *object);

void GameAISysInit();

void GameFog_Reset();

void Game_KillPart(PART_s *part, i32 reason);

void GameAISysReset(AISYS_s *system);

extern "C" i32 party_under_cover;
extern "C" i32 party_cant_be_under_cover;
extern GameObject_s *alert_obj;
extern f32 alert_timer;
extern AREADATA *VADER_ADATA;

void MakeBaddiesForgetAboutParty(i32 check_hostility);

void GameAttackInit();

extern "C" void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);

void GameFog_Update(WORLDINFO_s *world);

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);

void GameAISysSetGame();

void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);

void GameObjectOrigin(GameObject_s *object);

void PortalGameObject(GameObject_s *object, i32 enabled, i32 update_room, i16 room, nugscn_s *scene);

void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface);

i32 Game_IgnoreInput();

extern i16 id_GONKDROID;
u32 GameAI_TotalScore();

void GameDrawMenuEntry(MENU_s *menu, char *text);

void GameAnimSys_Update(GAMEANIMSYS_s *system);

extern CABLE_s cables[8];
CABLE_s *GameObjIsCableTied(GameObject_s *object);

void FindAnglesZX(NUVEC *, u16 *, u16 *);
f32 GetVehicleHoverHeight(GameObject_s *, f32 *);
u16 SeekRot(u16, u16, f32);
i32 RotDiff(u16, u16);
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

f32 GameGetMusicVolume(OPTIONSSAVE_s *options);

f32 GameSetMusicVolume(OPTIONSSAVE_s *options);

void GameAISysStartFrame(AISYS_s *system);

void GameDisplaySettings(LEVELDATADISPLAY *display, i32 *background_colours);

extern u8 grapple_attach_frames;
void GameObjectSetCanUse(GameObject_s *object, void *target, unsigned char action, unsigned char, float parameter);

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object);

void GameObjectDimensionsExtra_LSW(GameObject_s *object);

void GameObjectDimensions(GameObject_s *object);

void GameAntiNodeData_Init(GAMEANTINODEDATA_s *data, nuhspecial_s *special);

void GameAntiNodeData_Read(GAMEANTINODEDATA_s *data);

APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);

i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern i16 id_ANAKINJEDISCARRED;

void GameBlowUpBlownUpFn_LSW(GIZMOBLOWUP_s *blowup);

void GameLoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append, VARIPTR *buf, VARIPTR *buf_end, i32 area_models,
                             i32 area);

void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *);

extern GameObject_s *alert_obj;
extern NUVEC alert_pos;
extern f32 alert_timer;
extern "C" {
extern i32 party_under_cover;
}
i32 ZapTarget(GameObject_s *object);

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float);

void GameObjectDimensionsExtra_LSW(GameObject_s *object);

i32 AnakinGreenSabre(GameObject_s *object);
extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
i32 CannotKill(GameObject_s *object);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *, NUVEC *, i32, f32, NUVEC *, NUVEC *, BOLT_s *, u32, u8 *);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);
extern f32 ForceThrowSpeed, ForceThrowGravity;
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);


void GameObjectStuffAfterAnimation();

void GameMsg_DrawAdjustNewPos_CoinToTotal(GAMEMESSAGE_s *message);




















// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

static eduimenu_s *edTimingMenu;
static u32 EdAttr[] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};







extern "C" {
extern ed_module_s edTimingDesc;
}




































extern CUSTOMISER *CharacterCustomiser;
extern CUSTOMPIECE *Customiser_FindPieceByName(CUSTOMISER *, char *, i32 *, i32 *);
extern void Customiser_Set100PercentPieces(CUSTOMISER *);


i32 NoLayerKill(GameObject_s *object);



void InitExtraList();

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only);

extern "C" void NuSound3ClearLoopHold(NUVEC *, i32);
void Grapple_RemoveDynamic(void *);
void Shards_HandleLostObj(WORLDINFO_s *, GameObject_s *);
void DestroySnakeBody(GameObject_s *);
void BlowUpSnakeBody(GameObject_s *);
void PlayDieSfx(GameObject_s *);
void DieRumble(GameObject_s *);
void GetTakeOverPos(GameObject_s *, NUVEC *);
void SetToLastSafePos(GameObject_s *);
void ReleaseLever(GameObject_s *);
void Batarangs_CheckLostData(void *);
i32 SpecialMove_ReleaseVictim(GameObject_s *);
void ReleaseEat(GameObject_s *);
void ReleasePush(GameObject_s *);
void ReleaseForce(GameObject_s *, i32);
void LoseHelmet(GameObject_s *, i32, i32);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);
extern EXPLOSION Explosion[8];
extern i32 speeder_hitpoints_lost;
extern i16 id_MOUSEDROID;

void ConstantRumble(GameObject_s *object, f32 strength, f32 phase);

void PowerUp_Update(GameObject_s *object);


extern GameObject_s * player_tag_to;
extern GameObject_s * player_tag_from;
extern f32 player_tag_timer;
extern i32 do_player_tag;
i32 TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, NUVEC *);
extern char *ASCII_DOWN;



void ScaleGameObject(GameObject_s *object);

void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32);

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);

extern i32 nethost;
extern i16 id_PKDROID;
void ResetAICreature(GameObject_s *, AISYS_s *);
void SpawnCreatureFromCrate(GameObject_s *, f32, f32);
void SetToLastSafePos(GameObject_s *);
GameObject_s *GetOtherActivePlayer(GameObject_s *);
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

extern "C" i32 adaptivedifficulty[3];
extern i8 (*adtab)[4];
extern f32 Hub_PadSpeed[2];
extern u16 Hub_PadAngle[2];
extern GameObject_s * CarWashHack;
extern f32 AIFireIntervalMul;
extern f32 GhostLightMul;
extern f32 GhostLightTargetMul;
extern f32 CurrentSpeedOverride;
extern f32 lightning_sizew[2];
extern f32 lightning_sizel[2];
extern f32 lightning_sizewab[2];
extern f32 lightning_endw[2];
extern u32 lightning_col[2];
extern f32 CurrentSpeed;
extern f32 BaseCurrentSpeed;
extern AREADATA *PODSPRINT_ADATA;
extern i16 id_ANAKINSNEWPOD, id_ANAKINSNEWPODGREEN, id_SEBULBASPOD;
extern "C" f32 tieonsfxwait, tieoffsfxwait;
void MovePlayer_NETWORK(GameObject_s *);
void UpdateRumble(RUMBLEPACKET *);
void Player_ToggleCharacter(GameObject_s *, i32, i32);
void AveragePlayerCurrentSpeedMul();
void RegenerateHearts(GameObject_s *);
void AddSurfaceRipples(GameObject_s *);
void AddSurfaceDebris(GameObject_s *);

void GameAntinode_Update(GAMEANTINODESYS_s *system);

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set);

GameObject_s *GetNamedGameObject(AISYS_s *system, char *name);

void TakeOverGameObject2(GameObject_s *, GameObject_s *, i32);


void RegisterTakeOverObject(GameObject_s *object);


void DeactivateGameObject(GameObject_s *object);

i32 EquivalentObject_Find(WORLDINFO_s *world, nuhspecial_s *special);

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance);

void GameObjectToCameraDistances();

extern "C" {
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
}
void RemoveChunkControlFromStack(debris_chunk_control_s *, debris_chunk_control_s **);
void RemoveAnyChunkControls(i32 *chunk);

void RemoveChunkFromRenderStack(particlechunkrendertype_s *chunk, particlechunkrendertype_s **stack);

void RemoveChunkControlFromStack(debris_chunk_control_s *control, debris_chunk_control_s **stack);

extern "C" debkeydatatype_s *debris_keystack;

void RemoveDebrisEffectFromStack(debkeydatatype_s *key);

// ==== src/legoapi/misc/memory.cpp: all remaining content ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/android/nuptl_android.h"

#include <string.h>



void DebFreeChunksInstantly(i32 *);
void RemoveDebrisEffectFromStack(debkeydatatype_s *);
void RemoveAnyChunkControls(i32 *);
void AddChunkControlToStack(debris_chunk_control_s *, debris_chunk_control_s **);
void DebrisGetControlStackLock();
void DebrisReleaseControlStackLock();
void RemoveChunkFromRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

extern "C" {

    extern debkeydatatype_s *debkeydata;
    extern i16 *freedebkeys;
    extern i32 freedebkeyptr;
    extern i32 maxdebkeys;
    extern debinftype **debtab;
    extern i32 debrischunks;
    extern i32 debrischunksglass;
    extern i32 freedebchkptr;
    extern i32 freedebchkptrg;
    extern dma_particle_chunk_s **freedebchunks;
    extern dma_particle_chunk_s **freedebchunksglass;
    extern particlechunkrendertype_s *ParticleChunkToRender;
    extern particlechunkrendertype_s *ParticleChunkRenderStack[5];

i32 DebAlloc(void);

    extern debris_chunk_control_s **freechunkcontrols;
    extern i32 freechunkcontrolsptr;
    extern debris_chunk_control_s *debris_chunk_control_stack[2];
    extern f32 globaltime, panelglobaltime;

void DebFree(i32 *handle);

void DebFreeAllChunksInstantly(void);

void DebFreeInstantly(i32 *handle);

void DebFreeOrphansInstantly(debinftype *effect);

    void DebReAlloc2(debkeydatatype_s *key);

void DebReAlloc(debkeydatatype_s *key, i32 particle_count);

    extern "C++" void AddChunkToRenderStack(particlechunkrendertype_s *, particlechunkrendertype_s **);

void DebReAlloc2(debkeydatatype_s *key);

} // extern "C"

// ==== functions in original address order ====

// BaseThing defaults @0x424bf0 (dtor) and 0x425990..0x425a20 (interface
// defaults); RemoveDependancies returns 1, the rest are no-ops. GetName holds
// a 0 slot in the original base vtable (pure) — see basething.h.
BaseThing::~BaseThing() {
}

// ThingManager::AddThing @0x424c10. Appends at count; the pending
// AddThingAfterThis reservation (field_0x14) is folded into the index and
// cleared here.
void ThingManager::AddThing(BaseThing *thing) {
    i32 index = this->count;
    if (thing != NULL) {
        if (index < this->max_things) {
            this->things[index] = thing;
            index = index + 1;
        }
    }
    index = index + this->field_0x14;
    this->field_0x14 = 0;
    this->count = index;
}

// ThingManager::AddThingAfterThis @0x424c40. Reserves the slot after the
// current tail: bumps field_0x14 and stores the thing at count+field_0x14;
// the next AddThing folds the reservation into count.
void ThingManager::AddThingAfterThis(BaseThing *thing) {
    if (thing != NULL) {
        i32 index = this->field_0x14 + 1;
        this->field_0x14 = index;
        index = index + this->count;
        if (index < this->max_things) {
            this->things[index] = thing;
        }
    }
}

void ThingManager::RemoveTemporaryThings() {
    i32 i = this->count - 1;
    if (i >= this->permanent_count) {
        do {
            delete this->things[i];
            this->things[i] = NULL;
            --i;
        } while (i >= this->permanent_count);
    }
    this->count = this->permanent_count;
}

i32 ThingManager::RemoveDependanciesThings(ThingRemoveData *data) {
    i32 removed = 1;
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_REMOVE_DEPENDANCIES) != 0) {
            continue;
        }
        removed &= this->things[i]->RemoveDependancies(data);
    }
    return removed;
}

void ThingManager::EnterLevelThings(ThingLevelData *data) {
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_ENTER_LEVEL) != 0) {
            continue;
        }
        this->things[i]->EnterLevel(data);
    }
}

void ThingManager::ExitLevelThings(ThingLevelData *data) {
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_EXIT_LEVEL) != 0) {
            continue;
        }
        this->things[i]->ExitLevel(data);
    }
}

void ThingManager::edTimingInit() {
    static_cast<ThingManager *>(theThingManager)->ed_timing_state = 0;
}

void ThingManager::edTimingRender() {
    eduiMenuRender(edTimingMenu);
}

i32 ThingManager::edTimingProc(float delta_time, nupad_s *pad) {
    NuFntSet(0);
    NuFntScale(12, 12);
    eduiMenuProcess(edTimingMenu, delta_time, pad);
    return edTimingMenu == NULL ? 2 : 0;
}

void ThingManager::edTimingEnter() {
    edTimingMenu = eduiMenuCreate(30, 30, 250, 320, NULL, cbEdTrackCancel, "Timings");
    if (edTimingMenu != NULL) {
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(0, EdAttr, 0, 1, cbEdTimingSelect, "Off"));
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(1, EdAttr, 0, 2, cbEdTimingSelect, "System"));
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(2, EdAttr, 0, 3, cbEdTimingSelect, "No Things"));
        eduiMenuAddItem(edTimingMenu, eduiItemSelCreate(3, EdAttr, 0, 4, cbEdTimingSelect, "All Things"));
        for (i32 i = 0; i < static_cast<ThingManager *>(theThingManager)->count; ++i) {
            eduiMenuAddItem(
                edTimingMenu,
                eduiItemToggleCreate(
                    i + 4, EdAttr, static_cast<ThingManager *>(theThingManager)->things[i]->profiling_0xc, i + 5,
                    cbEdTimingSelect,
                    const_cast<char *>(static_cast<ThingManager *>(theThingManager)->things[i]->GetName())));
        }
    }
}

void ThingManager::cbEdTrackCancel(eduimenu_s *menu, eduimenu_s *) {
    eduiMenuDestroy(menu);
    edTimingMenu = NULL;
}

void ThingManager::cbEdTimingSelect(eduimenu_s *menu, eduiitem_s *item, u32) {
    ThingManager *manager = static_cast<ThingManager *>(theThingManager);
    u32 selection = item->data;
    manager->ed_timing_state = selection;
    manager->field_0x1c = selection - 3;
    if (selection <= 3) {
        i32 enabled = selection == 3;
        for (i32 i = 0; i < manager->count; ++i) {
            manager->things[i]->profiling_0xc = enabled;
        }
        if (!(selection <= 1)) {
            i32 index = 0;
            for (eduiitem_s *entry = menu->first; entry != NULL; entry = entry->next) {
                if (index < 3) {
                    entry->highlighted = 0;
                } else {
                    entry->highlighted = enabled;
                }
                ++index;
            }
        } else {
            eduiMenuDestroy(menu);
            edTimingMenu = NULL;
        }
        NuTimeBarEnable(item->data != 0);
    } else {
        manager->things[selection - 4]->profiling_0xc ^= 1;
    }
}

void ThingManager::EffectsThings(ThingRenderData *data) {
    const char *name = "Fx";
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] == NULL || (this->things[i]->flags & THING_FLAG_SKIP_EFFECTS) != 0) {
            continue;
        }
        if (__builtin_expect(this->things[i]->profiling_0xc != 0, 0)) {
            _NuTimeBarSlotBegin(this->timebar, 5, name);
        }
        this->things[i]->Effects(data);
        if (this->things[i]->profiling_0xc != 0) {
            _NuTimeBarSlotEnd(this->timebar, 5);
        }
    }
}

// ThingManager::DisplayThings @0x4252c0. Single pass over Display,
// bracketed with timebar slot 3 ("Dis"). PanelRender uses this pass for the
// display-layer things that render on top of the gameplay panel.
void ThingManager::DisplayThings(ThingRenderData *data) {
    const char *name = "Dis"; // timebar slot name @0x5734db

    for (i32 i = 0; i < this->count; ++i) {
        BaseThing *thing = this->things[i];
        if (thing != NULL && (thing->flags & THING_FLAG_SKIP_DISPLAY) == 0) {
            if (__builtin_expect(thing->profiling_0xc != 0, 0)) {
                _NuTimeBarSlotBegin(this->timebar, 3, name);
                thing = this->things[i];
            }
            thing->Display(data);
            thing = this->things[i];
            if (thing->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 3);
            }
        }
    }
}

// ThingManager::RenderThings @0x425390. Single pass over Render,
// bracketed with timebar slot 1 ("Rnd").
void ThingManager::RenderThings(ThingRenderData *data) {
    const char *name = "Rnd"; // timebar slot name @0x5734df

    for (i32 i = 0; i < this->count; ++i) {
        BaseThing *thing = this->things[i];
        if (thing != NULL && (thing->flags & THING_FLAG_SKIP_RENDER) == 0) {
            if (__builtin_expect(thing->profiling_0xc != 0, 0)) {
                _NuTimeBarSlotBegin(this->timebar, 1, name);
                thing = this->things[i];
            }
            thing->Render(data);
            thing = this->things[i];
            if (thing->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 1);
            }
        }
    }
}

// ThingManager::ProcessThings @0x425460. Pass 1 always runs
// ProcessEvenWhenPaused first; then, per ThingProcessData.paused, either
// Process or ProcessOnlyWhenPaused. Each pass has its own opt-out flag. The count
// is re-read every iteration because thing Process calls may add things.
// Profiling flags select which things receive NuTimeBarSlotBegin/End calls.
void ThingManager::ProcessThings(ThingProcessData *data) {
    const char *name = "PROC";
    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] != NULL && (this->things[i]->flags & THING_FLAG_SKIP_PROCESS_EVEN_WHEN_PAUSED) == 0) {
            if (__builtin_expect(this->things[i]->profiling_0xc != 0, 0)) {
                _NuTimeBarSlotBegin(this->timebar, 0, name);
            }
            this->things[i]->ProcessEvenWhenPaused(data);
            if (this->things[i]->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 0);
            }
        }
    }
    if (data->paused == 0) {
        for (i32 i = 0; i < this->count; ++i) {
            if (this->things[i] != NULL && (this->things[i]->flags & THING_FLAG_SKIP_PROCESS) == 0) {
                if (__builtin_expect(this->things[i]->profiling_0xc != 0, 0)) {
                    _NuTimeBarSlotBegin(this->timebar, 0, name);
                }
                this->things[i]->Process(data);
                if (this->things[i]->profiling_0xc != 0) {
                    _NuTimeBarSlotEnd(this->timebar, 0);
                }
            }
        }
    } else {
        for (i32 i = 0; i < this->count; ++i) {
            if (this->things[i] != NULL && (this->things[i]->flags & THING_FLAG_SKIP_PROCESS_ONLY_WHEN_PAUSED) == 0) {
                if (__builtin_expect(this->things[i]->profiling_0xc != 0, 0)) {
                    _NuTimeBarSlotBegin(this->timebar, 0, name);
                }
                this->things[i]->ProcessOnlyWhenPaused(data);
                if (this->things[i]->profiling_0xc != 0) {
                    _NuTimeBarSlotEnd(this->timebar, 0);
                }
            }
        }
    }
}

void ThingManager::ResetThings(ThingResetData *data) {
    const char *name = "Res";

    for (i32 i = 0; i < this->count; ++i) {
        if (this->things[i] != NULL && (this->things[i]->flags & 8) == 0) {
            if (__builtin_expect(this->things[i]->profiling_0xc != 0, 0)) {
                _NuTimeBarSlotBegin(this->timebar, 4, name);
            }
            this->things[i]->Reset(data);
            if (this->things[i]->profiling_0xc != 0) {
                _NuTimeBarSlotEnd(this->timebar, 4);
            }
        }
    }
}

// MemoryManager::AllocPool @0x425740.
//
// The manager is initialized by LoadPermData. Its pointer-bearing fields use
// native pointer width while retaining the original layout in the 32-bit build.
// Pools are 32-byte classes; a freed block is pushed on its class list and
// recycled by the pop path below (memset of the requested size only), while a
// fresh carve rounds up to the full 32-byte block and updates the counters.
void *MemoryManager::AllocPool(u32 size, i32 zero) {
    const i32 adjusted_size =
        static_cast<i32>(size - 1) < 0 ? static_cast<i32>(size + 0x1e) : static_cast<i32>(size - 1);
    const i32 size_class = adjusted_size >> 5;

    void **free_head = &free_lists[size_class];
    void *item = *free_head;
    if (item != NULL) {
        *free_head = *reinterpret_cast<void **>(item);
        if (zero != 0) {
            memset(item, 0, size);
        }
        return item;
    }

    const usize block = static_cast<usize>(size_class + 1) << 5;
    if (*end_cell - *cursor_cell <= block) {
        return NULL;
    }
    const usize aligned = ALIGN(*cursor_cell, 0x10);
    *cursor_cell = aligned;
    item = reinterpret_cast<void *>(aligned);
    *cursor_cell = aligned + block;
    memset(item, 0, block);
    allocated += block;
    remaining -= block;
    high_water = *cursor_cell;
    return item;
}

// MemoryManager::FreePool @0x425810: pushes the block onto its size class's
// free list (the next AllocPool of that class pops it, first word = next).
void MemoryManager::FreePool(void *item, u32 size) {
    const i32 adjusted_size =
        static_cast<i32>(size - 1) < 0 ? static_cast<i32>(size + 0x1e) : static_cast<i32>(size - 1);
    const i32 size_class = adjusted_size >> 5;

    void **free_head = &free_lists[size_class];
    *reinterpret_cast<void **>(item) = *free_head;
    *free_head = item;
}

// BaseThing::BaseThing @0x425840 zeroes the data fields after the vptr.
BaseThing::BaseThing() {
    this->field_0x4 = 0;
    this->flags = 0;
    this->profiling_0xc = 0;
}

// ThingManager::ThingManager @0x425870. Stores the manager in theThingManager
// and carves the thing-pointer array from the
// theMemoryManager linear pool. On allocation failure the array is NULL — the
// manager then simply never accepts things (AddThing's count < max check).
ThingManager::ThingManager(i32 max_things) {
    const usize need = static_cast<usize>(max_things) * sizeof(*things);

    BaseThing **array = NULL;
    if (*theMemoryManager.end_cell - *theMemoryManager.cursor_cell > need) {
        const usize aligned = ALIGN(*theMemoryManager.cursor_cell, 0x10);
        *theMemoryManager.cursor_cell = aligned + need;
        array = reinterpret_cast<BaseThing **>(aligned);
        memset(array, 0, need);
        theMemoryManager.allocated += need;
        theMemoryManager.remaining -= need;
        theMemoryManager.high_water = *theMemoryManager.cursor_cell;
    }
    this->things = array;
    this->max_things = max_things;
    // Profiling sets are a deferred subsystem; the handle is only ever passed
    // to the NuTimeBarSlotBegin/End stubs, so NULL behaves like the original
    // with profiling disabled.
    this->timebar = NuTimeBarCreateSet(0);
    theThingManager = this;
}

// ThingManager::EnableActions @0x425930. Finds the first thing whose 0x4 id
// matches and sets (invert==0) or clears (invert!=0) the given flags bits.
void ThingManager::EnableActions(i32 id, i32 flags, i32 invert) {
    i32 count = this->count;
    if (count <= 0) {
        return;
    }
    for (i32 i = 0; i < count; i++) {
        BaseThing *thing = this->things[i];
        if (thing == NULL) {
            continue;
        }
        if (thing->field_0x4 == (u32)id) {
            if (invert != 0) {
                thing->flags &= ~(u32)flags;
            } else {
                thing->flags |= (u32)flags;
            }
            return;
        }
    }
}

