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

// The reference defines these with dynamic initialization in this unit.
MemoryManager theMemoryManager;
#include "legoapi/misc/supportall_types.h"
BIGBUFFER_s BigBuffer;
void *theThingManager = NULL;

void ClearLevelProgress(i32, WORLDINFO_s *);
u32 TotalLevelCoinTally(WORLDINFO_s *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *, u32 *);
extern i32 bonusmodearcade;
extern f32 DEFAULT_MOVE_RANGE;
extern f32 drop_back_in_timer;
extern i32 last_chatter_sfx;
void InitPlayerAI(GameObject_s *object);
extern GAMECAMERA_s *GameCam;

struct AIROW_s;
struct nunativegscene_s;
struct SHOPINPUT;


DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

extern i32 app_tbgameset;
extern i32 app_tbplayerset;
extern i32 app_tbaiset;
extern i32 app_tbdrawset;



void RndrTexQuad(f32 x, f32 y, f32 width, f32 height, i32 colour, numtl_s *material, i32 angle);

i32 SuperWeirdo(GameObject_s *object);

void bgProcClose() {
}

extern "C" void edrtlCalculateBurnoutEx(burnset_s *set, NuBloomParameters *parameters, NUVEC *camera_position,
                                        f32 frame_time);

void bgprocFreeze();

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


void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material);

void bgProcAbortAll();

void bgprocUnFreeze();

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 bgprocIsFreezing();

void DebFreeWithoutKey(debkeydatatype_s *key);

i32 RndrUnfilledCircle(f32 x, f32 y, f32 radius, f32 border_width, f32 aspect, i32 colour, f32 progress, f32 z,
                       numtl_s *material);

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

extern "C" {
    extern debinftype **debtab;
}

extern "C" {
    extern debinftype **debtab;
    extern f32 debris_thinning_level;
    extern i32 forced_debris_thinning;
}

extern "C" void DebReAlloc(debkeydatatype_s *, i32);

VARIPTR *DisplayListRenderBuffer() {
    return &rndrstream_free;
}

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

extern "C" {
    extern debinftype **debtab;
}

void TBRESET();

void TBOPENFN(char *name, i32 type);

void RndrArrow(float x, float y, float scale, i32 angle, i32 colour);

void TBCLOSEFN(char *name, i32 type);

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

BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
void PartCollide_3D(PART_s *);
extern i32 lightning_type;
extern i16 id_YODA, id_YODAGHOST, id_GAMORREANGUARD, id_JANGOFETT;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;
extern i32 VehicleArea;

extern "C" {
    extern NUVEC plr_lastpos;
    extern i16 id_GRIEVOUS;
    extern i16 id_BODYGUARD;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_JAWA;
    extern i16 id_UGNAUGHT;
    extern i16 id_ATAT;
    extern i16 id_GONKDROID;
    extern i16 id_LANDSPEEDER, id_MILLENNIUMFALCON;
    void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisPosOrientationMtx(i32, NUMTX *);
    void DebFreeInstantly(i32 *);
    void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
}

// Written by ThingManager's ctor (original global @0x124f2e0, .bss).
extern void *theThingManager;

void legoSetMusicVolume(float);
void AnimatePlayer(GameObject_s *object);
void InitSurfaceInfo(GameObject_s *object);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
APIOBJECT *GameAPIOBJECTFromObjID(u8 object_id);
void LevelScriptReStoreProgress(WORLDINFO_s *world, LEVELSCRIPTPROCESS_s *process);

extern i32 LEGO_AIPATHCNX_BLOCKAGE;

extern "C" i32 AISysSetLevelPath(AISYS_s *system, char *path_name);

extern GAMEFOG_STATE GameFog;

#include "legoapi/items/objects/gameobjects_types.h"

// Enabled in the target data image.  Ordinary background characters are
// staggered across frames; special movement states opt back into full-rate
// processing through the forced-update path.

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

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

f32 GameShadow(GameObject_s *object, nuvec_s *position, f32 probe_height, i32 terrain_mask);

extern i32 Paused;
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

extern FadeSystem FadeSys;
extern i32 pause_rndr_on;

extern "C" i32 NewRayCastScaleYMask(NUVEC *, NUVEC *, f32, f32, i32, u32);

i32 GameRayCast(NUVEC *position, NUVEC *displacement, f32 radius, i32 mask);

extern i32 active_neutral_count;
extern AREADATA *VADER_ADATA;
extern i32 active_baddy_count;

#if defined(__i386__)
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone, regparm(2), sseregparm))
#else
#define GAMEOBJECTS_CLONE_ATTR2_SSE __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, which takes the other object's x and z by address.

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

}

extern f32 OFFSCREEN_CATCHUP_TIME;

i32 TryToTeleportToNextNode(GameObject_s *object, AIPATHNODE_s *node, i32 tightrope);

void SetSpecialMove(GameObject_s *object, AIPATHNODE_s *target, AIPATHNODE_s *node, char move);

void ClearSpecialMove(GameObject_s *object);

void GameAISysInit();

void GameAISysReset(AISYS_s *system);

i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);

void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);

void GameObjectOrigin(GameObject_s *object);

void PortalGameObject(GameObject_s *object, i32 enabled, i32 update_room, i16 room, nugscn_s *scene);

void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface);

i32 Game_IgnoreInput();

extern i16 id_GONKDROID;
u32 GameAI_TotalScore();

void GameAnimSys_Update(GAMEANIMSYS_s *system);

u16 SeekRot(u16, u16, f32);
i32 RotDiff(u16, u16);
extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER, id_ATST;

f32 GameGetMusicVolume(OPTIONSSAVE_s *options);

f32 GameSetMusicVolume(OPTIONSSAVE_s *options);

CABLE_s *GameObjOwnsAnyCables(GameObject_s *object);

i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);

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

i32 NoLayerKill(GameObject_s *object);

GameObject_s *FindGameObject(i32 character_id, u32 required_flags, i32 alive_only, i32 vehicle_only,
                             i32 non_level_only);

void DieRumble(GameObject_s *);

void RemoveGameObject(GameObject_s *obj, i32);

extern i32 nethost;
void TakeOverGameObject(GameObject_s *, GameObject_s *, i32, i32);

extern f32 lightning_sizew[2];
extern f32 lightning_sizel[2];
extern f32 lightning_sizewab[2];
extern f32 lightning_endw[2];
extern u32 lightning_col[2];
extern AREADATA *PODSPRINT_ADATA;
extern i16 id_ANAKINSNEWPOD, id_ANAKINSNEWPODGREEN, id_SEBULBASPOD;
void Player_ToggleCharacter(GameObject_s *, i32, i32);

GameObject_s *AddDynamicCreature(i32 model, nuvec_s *position, i32 angle, char *script_name, AIPATHINFO_s *path_info,
                                 AIGROUP_s *group, i32 set_on_surface, nugspline_s *spline, nuvec_s *spline_offset,
                                 i32 spline_mode, i32 creature_set);

GameObject_s *GetNamedGameObject(AISYS_s *system, char *name);

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

i32 GameObjectNearFloor(GameObject_s *object, f32 height, f32 *distance);

// ==== src/legoapi/misc/memory.cpp: all remaining content ====
#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/android/nuptl_android.h"

#include <string.h>

extern "C" {

    extern debinftype **debtab;

void DebFreeInstantly(i32 *handle);

void DebReAlloc(debkeydatatype_s *key, i32 particle_count);

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

