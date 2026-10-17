// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "legoapi/ai/game/creature.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/audio/sfx.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void RemoveGameObject(GameObject_s *object, i32 immediate);

void ClearAICreatures();

void StoreProgressAICharacter(LEVEL_PROGRESS_s *progress);

void AICreatureResumeScript(GameObject_s *object);

extern "C" {
    extern NUVEC plr_lastpos;
    AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);
    void AddToAIGroup(AIGROUP *group, APIOBJECT *object);
    void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
    void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);
    void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 route, i32 surface_flags);
    void ResetAnimPacket(ANIMPACKET_s *packet, i16 animation);
    void SetAnimTimeRandom(CHARACTERMODEL_s *model, ANIMPACKET_s *packet);
}

void InitPlayerAI(GameObject_s *object);
void ResetPlayerMoves(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 context);
void Player_ResetContexts(PLAYERPACKET_s *packet);
void InitSurfaceInfo(GameObject_s *object);
i32 SetObjOnSurface(GameObject_s *object, i32 mode);
#include "legoapi/ai/game/creature_types.h"


void InitAICreatures(AISYS_s *system);

void ResetAICreature(GameObject_s *object, AISYS_s *system);

void ResetAICreatures(AISYS_s *system);

void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
void NewRumbleAllPlayers(f32, f32, i32, i32);
i32 ObjHitObj_Flags(GameObject_s *);

extern "C" HOTHBATTLE_MELEE_s melee;
i32 OnOrInsidePlane(NUVEC *point, NUVEC *plane_point, NUVEC *plane_normal, NUVEC *corrected_point, f32 normal_offset,
                    f32 *distance);

i32 SpawnMeleeCreatureType(i32 type);

extern GameObject_s * alert_obj;
extern NUVEC alert_pos;
extern f32 alert_timer;
extern f32 alert_time;

void AlertSurroundingCreatures(GameObject_s *object, nuvec_s *position);

// ==== src/legoapi/render/core/render.cpp: declarations ====
#include "decomp.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/render/core/render.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/gamemessages.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/arcade.h"
#include "legoapi/render/light/fade_material.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/cutscenes/cutscenes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void DrawSubItems();

extern "C" void edbriStartAllPages(void);

#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"

extern "C" void AiRndrLine3dDbg(f32, f32, f32, f32, f32, f32, u32);

extern "C" void edDrawCross(NUVEC *centre, u8 red, u8 green, u8 blue);

extern "C" void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                              f32 upper_y, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32, i32 segments);

extern "C" void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32,
                                             i32 segments);

extern "C" void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);

extern "C" void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32);

extern "C" void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);

#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/world/level.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world_shared.h"
#include "globals.h"
#include "nu2api/nu3d/nutexanm.h"
struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/android/nurndr_android.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/numath.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

extern i32 VehicleArea;
extern i32 GAMEDEMO;
extern f32 FORCEGLOWTIME;
extern i16 tHINTS;
extern i16 tCHARACTERS;
extern i16 tEXTRAS;
extern i16 tGOLDBRICKS;
extern i16 tSTORYCLIPS;
extern i16 tENTERCODE;
i32 AnakinGreenSabre(GameObject_s *object);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern f32 HUB_EPISODESUBTITLEY;
extern f32 HUB_EPISODETITLEY;
i32 draw_para = 1;

void DrawGameObjectsDraw(i32 pass);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();
static void DrawParaphernalia(GameObject_s *object);

struct GAMEMESSAGE_s;
struct HINT_s;
struct MENU_s;
struct NuBloomParameters;
struct STATUSPACKET_s;
struct TouchHolder;
struct _vum_s;
struct _vuv_s;
struct nuhspecial_s;
struct ripple_node_s;
struct ripple_set_s;
struct VuVec;

extern nuhspecial_s *(*GameMsg_GetExtraObjFn)(GAMEMESSAGE_s *);

extern "C" i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" void NuRndrGradRect2di(i32, i32, i32, i32, i32 *, numtl_s *);
extern "C" void NuRndrRect2di(i32, i32, i32, i32, i32, numtl_s *);
extern "C" void NuCameraUnlock(void);
extern "C" void NuCameraRelock(void);
extern char *apiGameName;
extern char *apitxt_EMPTY;
extern char *apitxt_PRESENT;
extern char *apitxt_AUTOSAVE_WARNING;
extern char *apitxt_LOADING;
extern char *apitxt_SAVING;
extern char *apitxt_NODATAAVAILABLE;
extern char *apitxt_OK;
extern char *apitxt_GAMELOADED;
extern char *apitxt_GAMELOADING;
extern char *apitxt_FAILEDTOLOAD;
extern char *apitxt_CORRUPTLOAD;
extern char *ASCII_DOWN;
extern char *ASCII_UP;
extern i16 tCURRENTGAME;
extern i16 tEMPTY;
extern i16 tGAME;
extern i16 tNOSPACE;
extern f32 MENUTEXTSCALE;
extern f32 MENUBOTY;
extern f32 AUTOSAVEICONY;
extern f32 AUTOSAVEICONX;
extern f32 AUTOSAVEICONSIZE;
extern f32 MenuAlpha;
extern i32 MenuA;
extern f32 memcard_loadmessage_delay;
extern f32 memcard_loadresult_delay;
extern u8 MENUNORMALR;
extern u8 MENUNORMALG;
extern u8 MENUNORMALB;
extern u8 MENUENTRYR, MENUENTRYG, MENUENTRYB;
extern u8 MENUFLASH0R;
extern u8 MENUFLASH0G;
extern u8 MENUFLASH0B;
extern u8 MENUFLASH1R;
extern u8 MENUFLASH1G;
extern u8 MENUFLASH1B;
extern f32 menu_pulse;
extern f32 menu_pulsate;
extern i32 menu_flash;
extern f32 text3d_height;
extern f32 text3d_width;
extern FadeSystem FadeSys;
extern f32 cointotaltime;
extern "C" f32 MainRenderTime;
extern "C" i32 editor_active;
extern i32 Paused;
extern "C" i32 screendump;
extern f32 pause_fade;
extern "C" i32 PANELOFF;
extern i32 noscenespecials;
extern void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);
extern void SetLevelLights(void *set, f32 scale);
extern f32 ICONX;
extern f32 ICONSIZE;
extern f32 DROPINALPHA;
extern f32 statstime;
f32 GetAspectRatio();
void Hint_Draw(i32 player_index);
extern i32 CutScenePlayer_CanStart(i32 cutscene_id);

// Camera zoom state
extern f32 CameraZoom;

// Graphics loading flags
extern i32 RemoveDirectionalMaps;
extern i32 RemoveNormalMaps;

extern NUVIDEORESHEADER g_VideoResHeader;

extern "C" {
    i32 NuDisplayListRndrSpecial(nuhspecial_s *special, NUMTX *mtx, i32 skinned, NUMTX *skin_mtx,
                                 DEFORMERWEIGHTSARRAY *blend_values);
    void *NuVisiEvaluate(NUGSCN *scene, void *visibility_context);
}

void SetCameraZoom(f32 zoom);

extern "C" void NuGScnUpdate(NUGSCN *gscn, f32 frame_delta);

// --- NuGScn graphics-data reader ---
// NuReadGraphicsData is a C++ static function in the original (GCC clones it,
// hence the `.isra.NNN` suffix in the ROM symbol table).

extern "C" void NuSevereWarning(const char *format, ...);

// Retail only keeps the IPA-SRA clone, which drops the unused fourth argument.
#if defined(__i386__)
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define NU_READ_GRAPHICS_DATA_ATTR __attribute__((noinline, noclone))
#endif
static NU_READ_GRAPHICS_DATA_ATTR NUGSCN *NuReadGraphicsData(VARIPTR *buf, VARIPTR *buf_end, char *path,
                                                             char *scene_data)
    __asm__("_ZL18NuReadGraphicsDataP9variptr_uS0_PcS1_S1_.isra.104");

// --- Extern "C": NuGScn functions have C linkage in original ---
extern "C" {
NUGSCN *NuGScnReadForMultiRender(VARIPTR *buf, VARIPTR buf_end, char *path, i32 render_count);

NUGSCN *NuGScnRead(VARIPTR *buf, VARIPTR buf_end, char *path);
void NuGScnReadFromMemory(NUGSCN *scene);
NUGSCN *NuGHGFixup(NUGSCN *scene, void *);
nuhgobj_s *NuGHGRead(char *path, VARIPTR *buf, VARIPTR buf_end);
} // extern "C"

extern i32 solid_cable;
extern i32 nsegments_drawn;
extern f32 slack_factor;
extern f32 cable_slack;
extern f32 nsegments_per_unit;
extern CABLE_s cables[8];
extern f32 tow_length;
extern NUMTL *SolidMtl3D;

void DrawCables();

void DrawRipple(ripple_node_s *node);

void DrawAreaBox(nuvec_s *position, nuvec_s *size, i32 rotation, i32 colour);

void DrawBox_Now(_vuv_s *, _vuv_s *, i32, i32);

extern "C" {
extern f32 aiEditor_DrawYOffset;
extern f32 aiEditor_LocatorWidth;
    void AiRndrLine3d(NURND_VERTEX3D *, NUMTL *, NUMTX *);
}

void DrawLocator(nuvec_s *position, float radius, i32 rotation, i32 colour);

void Draw_LOADED();

i32 Draw3DObject(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                 u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order);

void DrawCharIcon(i32 character_id, float x, float y, float z, float scale, i32 frame_object_id, float character_alpha,
                  float frame_alpha, i32 draw_character, nuhspecial_s *override_special);

extern const u8 HintRGB[5][3];

void DrawLine_Now(_vuv_s *, _vuv_s *, i32, i32);

void DrawParallax(nuhspecial_s *special);

void DrawQuestion(nuvec_s *position, float scale_value, float y_push);

void DrawRectRGBA(float x, float y, float width, float height, u32 colour, numtl_s *material, i32 alignment,
                  float scale);

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float unused, float y_push, u16 x_rot,
              u16 y_rot, u16 z_rot);
// These rotations are also used by the shop translation unit. Keep them local
// so the compiler can inline the matrix arithmetic at each call site.
static inline void ShopRotateX(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m01 = m->m01;
    f32 m11 = m->m11;
    f32 m21 = m->m21;
    f32 m31 = m->m31;

    m->m01 = m01 * cosx - m->m02 * sinx;
    m->m02 = m01 * sinx + m->m02 * cosx;
    m->m11 = m11 * cosx - m->m12 * sinx;
    m->m12 = m11 * sinx + m->m12 * cosx;
    m->m21 = m21 * cosx - m->m22 * sinx;
    m->m22 = m21 * sinx + m->m22 * cosx;
    m->m31 = m31 * cosx - m->m32 * sinx;
    m->m32 = m31 * sinx + m->m32 * cosx;
}

static inline void ShopRotateY(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m10 = m->m10;
    f32 m20 = m->m20;
    f32 m30 = m->m30;

    m->m00 = m00 * cosx + m->m02 * sinx;
    m->m02 = m->m02 * cosx - m00 * sinx;
    m->m10 = m10 * cosx + m->m12 * sinx;
    m->m12 = m->m12 * cosx - m10 * sinx;
    m->m20 = m20 * cosx + m->m22 * sinx;
    m->m22 = m->m22 * cosx - m20 * sinx;
    m->m30 = m30 * cosx + m->m32 * sinx;
    m->m32 = m->m32 * cosx - m30 * sinx;
}

static inline void ShopRotateZ(NUMTX *m, NUANG a) {
    f32 cosx = NU_COS_LUT(a);
    f32 sinx = NU_SIN_LUT(a);
    f32 m00 = m->m00;
    f32 m10 = m->m10;
    f32 m20 = m->m20;
    f32 m30 = m->m30;

    m->m00 = m00 * cosx - m->m01 * sinx;
    m->m01 = m00 * sinx + m->m01 * cosx;
    m->m10 = m10 * cosx - m->m11 * sinx;
    m->m11 = m10 * sinx + m->m11 * cosx;
    m->m20 = m20 * cosx - m->m21 * sinx;
    m->m21 = m20 * sinx + m->m21 * cosx;
    m->m30 = m30 * cosx - m->m31 * sinx;
    m->m31 = m30 * sinx + m->m31 * cosx;
}
void Draw_LOADING();

#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nupostparams.h"

#define ALPHA_GRID_VERTEX(vx, vy)                                                                                      \
    do {                                                                                                               \
        direction.x = (vx);                                                                                            \
        direction.y = -(vy) * camera.aspect;                                                                           \
        direction.z = adjacent;                                                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        NuVecMtxTransform(&direction, &direction, &camera.mtx);                                                        \
        NuVecNorm(&direction, &direction);                                                                             \
        brightness = direction.y * 0.5f + 0.5f;                                                                        \
        if (brightness <= near_angle)                                                                                  \
            brightness = near_scale * parameters->intensity;                                                           \
        else if (brightness >= far_angle)                                                                              \
            brightness = far_scale * parameters->intensity;                                                            \
        else                                                                                                           \
            brightness = ((brightness - near_angle) * scale_delta / angle_delta + near_scale) * parameters->intensity; \
        if (parameters->directional) {                                                                                 \
            f32 angle = (i16)(0x4000 - NuASin(NuVecDot(&parameters->direction, &direction))) * 0.0054931640625f;       \
            if (angle < parameters->direction_near_angle) {                                                            \
                brightness += 128.0f * parameters->direction_far_scale;                                                \
            } else if (!(angle > parameters->direction_far_angle)) {                                                   \
                f32 blend = NuPowFast((angle - parameters->direction_near_angle) /                                     \
                                          (parameters->direction_far_angle - parameters->direction_near_angle),        \
                                      parameters->direction_bias);                                                     \
                brightness +=                                                                                          \
                    128.0f * ((1.0f - blend) * (parameters->direction_far_scale - parameters->direction_near_scale) +  \
                              parameters->direction_near_scale);                                                       \
            }                                                                                                          \
        }                                                                                                              \
        u32 colour = (static_cast<u32>(static_cast<u8>(128)) << 24) |                                                  \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 16) |                   \
                     (static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness)))) << 8) |                    \
                     static_cast<u32>(static_cast<u8>(MIN(255.0f, MAX(0.0f, brightness))));                            \
        if (!g_NuPrim_NeedsOverbrightening)                                                                            \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] =                                                                     \
                ((static_cast<i32>(colour) >> 1) & 0x7f7f7f) | (colour & 0xff000000);                                  \
        else                                                                                                           \
            g_NuPrim_StreamBufferPtr->u32_ptr[3] = colour;                                                             \
        NuPrim2DAddXYZ((vx), (vy), 0.0f);                                                                              \
    } while (0)

void DrawAlphaGrid(i32 rows, i32 cols, NuBloomParameters *parameters);

void DrawArrow_Now(_vum_s *, float, i32, i32);

extern i16 tNONEWBESTTIME, tNEWBESTTIME;
void Text_MakeTime(f32, i32, i32, i32, char *);

void DrawBonusTime(STATUSPACKET_s *packet, float position, i32 alpha);

void DrawCross_Now(_vuv_s *, float, i32, i32);

void DrawPauseFade();


void DrawRippleSet(ripple_set_s *set);



void ClearScreen();



void DrawAlphaImage(i32 rows, i32 cols, numtl_s *material, i32 use_pixel_offsets, NuBloomParameters *parameters);

void DrawBezierLine(VuVec &start, VuVec &start_control, VuVec &end, VuVec &end_control, numtl_s *material, i32 colour);

void DrawBoxMtx_Now(_vum_s *, _vuv_s *, i32, i32);

void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);

void DrawCutBorders(i32 widescreen);

void DrawExplosions();

void DrawItemMenu2D();

void DrawStatusText(char *text, u16 angle, float x, float y, float scale, u32 colour, i32 alignment);

i32 Draw3DObjectMtx(WORLDINFO_s *world, i32 object_index, numtx_s *mtx);

void DrawGameObjects();

extern nuhspecial_s green_light;
extern nuhspecial_s painttargetcolour[3];
extern nuhspecial_s paintlights[3];

void DrawPaintLights();

void DrawStatusIcons(STATUSPACKET_s *status, float y, float alpha);

void DrawTouchPrompt(char *prompt, char *unused_label, bool hovered, bool large);

void Draw_LOADFAILED();

void DrawAreaCylinder(nuvec_s *centre, nuvec_s *size, i32 colour);

void DrawCameraTarget(nuvec_s *);

void DrawGameMessages();

extern f32 KITPOSX;
extern f32 KITPOSY;
extern f32 KITPOS2X;
extern f32 KITPOS2Y;
extern f32 PANEL_MINIKITSCALE;
extern f32 PANEL_REDBRICKSCALE;
extern f32 PANEL_MINIKITY;
extern f32 PANEL_MINIKITCOUNTSCALE;
extern f32 PANEL_MINIKITCOUNTY;
// DrawPanel HUD globals referenced by the cantina-bar-patrons WIP. Initial
// red-brick positions come from the Game init values in legogame/game.cpp.
extern i32 DRAWBGLOAD;
extern u16 PowerUp_PanelYRot[2];
extern f32 PowerUp_PanelPosMul[2];
extern f32 POWERUPOBJSIZE;
extern f32 REDBRICKPOSX;
extern f32 REDBRICKPOSY;
extern f32 REDBRICKPOS2X;
extern f32 REDBRICKPOS2Y;
extern i32 Arcade_Points[2];
extern f32 BOSSICONY;
extern i32 FPSDISPLAY;
extern i32 ShowPlayerCoordinate;
extern i32 LEGOOBJ_CHARKIT;
extern i32 LEGOOBJ_MINIKIT;
i32 DrawPanel3DObjectNoAlpha(f32, f32, f32, f32, f32, f32, u16, u16, u16, nuhspecial_s *, i32);
extern "C" void Text3D(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8);

void DrawMiniKitCount(float position, float scale, i32 count, i32 maximum);

void DrawStatusBG_LSW(STATUSPACKET_s *status);

void Draw_LOADCORRUPT();

i32 Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, nuvec_s *position, u16 x_rotation, u16 y_rotation,
                      u16 z_rotation, float scale_x, float scale_y, float scale_z, i32 rotate_order, float alpha);

void DrawCameraTarget2(nuvec_s *);

i32 DrawPanel3DObject(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                      u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order, float alpha);

extern STATUSMINIKITPART_s KitPart[10];
extern i32 currentminikit;
extern f32 slideseek;

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);
void DrawStatusMiniKit(float x, float y, float z, float built_scale, float new_scale, i32 count, STATUSPACKET_s *packet,
                       float slide);

extern i16 tUNKNOWN, tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern "C" void PlaySfx(char *, NUVEC *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);

void DrawSubItemMenu2D();

void DrawSubItemMenu3D();

void Draw_NOMEMORYCARD();

void DrawFadeScreenWipe();


void DrawForceGlowSprite(nuvec_s *position, float radius, i32 model, float alpha, GameObject_s *);

void DrawGameObjectsDraw(i32);

void Draw_AUTOSAVECANCEL();

void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, float alpha);

void Draw_AUTOSAVEWARNING();

void Draw_NODATAAVAILABLE();

extern i32 dco_locatorposonly;
extern i32 dco_id;
extern i32 dco_reflectaxis;
extern i32 dco_wearinghat;
extern u16 dco_prerotatez;
extern f32 dco_reflectcoord;
extern GAMECHARACTERDATA_s * dco_gcdata;
extern CHARACTERMODEL_s * dco_cmodel;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);

extern f32 DropInOutScale(GameObject_s *object);
extern void ApplyExtraRotation(GameObject_s *object, NUMTX *matrix);
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern "C" {
    extern i16 id_ATST;
    extern i16 id_ATST_LOWRES;
    extern i16 id_ATAT;
}
u8 CharClipToBlobShadows;

static inline void DrawSetRotationY(NUMTX *matrix, NUANG angle) {
    const f32 cosine = NU_COS_LUT(angle);
    const f32 sine = NU_SIN_LUT(angle);
    NUMTX value = {cosine, 0.0f, -sine, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sine, 0.0f, cosine, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    *matrix = value;
}

i32 DrawGameObjectsProcess();

void DrawStatusTextFraction(i32 current, i32 total, float x, float y, u16 angle, float scale, u32 colour,
                            float animation, float animation_duration);

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);

i32 DrawPanel3DObjectMtxNoAlpha(nuhspecial_s *special, numtx_s *matrix);

extern void DrawMenuEntry(MENU_s *, char *);
void Draw_OK(MENU_s *menu);

void DrawAABox(_vuv_s *position, _vuv_s *size, i32 colour);

void DrawArrow(nuhspecial_s *special, float scale_value);

void DrawCross(nuvec_s *centre, float radius, numtl_s *material, i32 colour);

extern f32 SwipeDecalRenderer_MaxWidth;
extern f32 SwipeDecalRenderer_MinWidth;


#define SWIPE_PRIM_UV(U, V)                                                                                            \
    do {                                                                                                               \
        PrimVertexRaw *vertex = static_cast<PrimVertexRaw *>(g_NuPrim_StreamBufferPtr->void_ptr);                      \
        if (!g_NuPrim_NeedsHalfUVs) {                                                                                  \
            vertex->float_uv[0] = (U);                                                                                 \
            vertex->float_uv[1] = (V);                                                                                 \
        } else {                                                                                                       \
            vertex->half_uv[0] = NuRndrFloatToHalf(U);                                                                 \
            vertex->half_uv[1] = NuRndrFloatToHalf(V);                                                                 \
        }                                                                                                              \
    } while (false)




extern i32 GetMenuID(void);


static void DrawWeapon_SetSabreObjects(GameObject_s *object, i32 red, i32 green, i32 blue, i32 purple, i32 *models,
                                       i32 *hilt);

static void DrawWeapons(GameObject_s *object, i32 reflection, f32 weapon_scale);

static void DrawCharacterAttachments(GameObject_s *object, NUMTX *joint_matrices);

void CharScene_Draw(WORLDINFO_s *, i32, NUMTX *, NUMTX *);
void CharMiniKit_Draw(i32, NUMTX *, i32, f32, f32);
void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, NUMTX *);
i32 Batarang_GetObjectFromCharID(i32);
void Grapple_DrawLine(GameObject_s *);
void Transform_DrawTarget(NUVEC *, f32, f32);
extern i16 id_ANAKINJEDISCARRED, id_WEIRDO1, id_WEIRDO2, id_CATAPULT, id_CHEWBACCA, id_WOOKIEE, id_TWOFACE;
extern i32 PickUpFlickerTest, PickUpFlickerFrames, PickupFlickerFrame;

static void DrawParaphernalia(GameObject_s *object);

#include "legoapi/legoapi_types.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nurndr.h"

extern i32 qrand(void);
extern f32 SeekLinearF(f32, f32, f32);




extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;


extern i32 backdrop_black;

extern void (*BackDrop_AlphaFn)(float *);


void BackDrop_Init(char *path, variptr_u *buf, variptr_u *buf_end);

void BackDrop_Dump();

void BackDrop_Update(float dt);

void BackDrop_ResetColours();

void BackDrop_UpdateColours(i32 instant);

void BackDrop_Draw(float alpha, i32 flags);

extern "C" {
    extern nurenderscene_s currentScene;
void DisplaySceneRndrSpecials(NUDLDLISTSCENE *scene, i32, void *visibility_context);
}

// ==== src/legoapi/characters/core/characters.cpp: declarations ====
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
    i32 hub_character_ready = -1;
}

extern APICHARACTERMODELLIST_s PermModelList[1];

#include "legoapi/characters/core/characters_types.h"

void LSW_SetIndy(i32);

void HairMovement(GameObject_s *object);

void HeadMovement(GameObject_s *object);

nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id);

void CharScene_Draw(WORLDINFO_s *world, i32 character_id, numtx_s *matrix, numtx_s *reflection_matrix);

i32 CharIDFromName(char *name);

CHARACTERDATA *CDataFromName(char *name);

i32 RedirectAnim(char *directory, ANIMREDIRECT *redirects, ANIMLIST_s *animation_list, char *path);

void CharScenes_Init(variptr_u *buf, variptr_u *);

void CharScenes_LevelLoad(WORLDINFO *world);

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

static u32 LayerBit(u8 layer) {
    return 1u << layer;
}

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

void UpdateCharacterIDs();

void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *list, variptr_u *buf, variptr_u buf_end);

void CharScenes_AreaDump();

void SetMoveAndAnimateFunctions(u32 model_flag_mask, u32 model_flag_value, u32 game_flag_mask, u32 game_flag_value,
                                i32 movement_type, void *move_function, void *animate_function, void *draw_function);

void DeactivateGameObject(GameObject_s *);

void DeactivateCharacter(char *name);

void LoadSingleCharacter(bgprocinfo_s *);

void CharScenes_LevelDump(WORLDINFO_s *world);

void UpdateCharacterLoad();

extern "C" VARIPTR characterbuffer_base;
extern i32 CHARACTERBUFFERSIZE;
extern i32 Area;
extern i32 last_area;

void ResetCharacterBuffer(i32 force_reset);

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
extern f32 COLLECTION_Y_STATUS;

void CollectCharcters_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void CollectCharcters_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet);

nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id);

void PostAnimate_ASTROMECH(GameObject_s *object);

AILOCATOR_s *LocalGetRandomLocator(AILOCATOR_s **locators, i32 count, f32 clip_radius, NUVEC *position,
                                   f32 max_distance, i32 outside_camera, f32 max_delta_y, f32 min_delta_y);

AILOCATOR_s *LocalGetNearestLocator(AILOCATOR_s **locators, i32 count, f32 clip_radius, NUVEC *position,
                                    f32 max_distance, i32 outside_camera, f32 max_delta_y, f32 min_delta_y);

i32 newCharactersCollected(STATUSPACKET_s *);

void CollectCharcters_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

i32 SetProtocolDroidFallAnim(GameObject_s *object);

void CollectCharactersOff_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void CollectCharactersOff_Skip(STATUS_STAGE_s *, STATUSPACKET_s *);

void ScaleGameObject(GameObject_s *obj);

void SetGameObjectCharacterData(GameObject_s *obj);

void CollectCharactersOff_Update(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, float elapsed);

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

// ==== src/legoapi/render/core/terrain.cpp: declarations ====
#include "decomp.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/render/core/gameliball.h"

#include <stdio.h>
#include <new>

#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/nuplatform/nudevicespecs.hpp"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuvec4.h"

#include <limits.h>
#include <math.h>
#include <string.h>

struct tertype;
struct terrsitu_s;
struct PLATSKININFO;
struct GAMECAMERA_s;
struct pushblock_s;
void Surface_Deflect(NUVEC *normal, NUVEC *movement, NUVEC *result, i32 mode);
extern i16 id_ATST;
extern i16 id_SPEEDERBIKE;
void GetSurfaceInfo(GameObject_s *object, i32 update_surface, f32 shadow_height);
f32 FindReflectionNoPlatforms(NUVEC *position);
extern i32 TimingBarSet;
void TBOPENFN(char *, i32);
void TBCLOSEFN(char *, i32);
i32 NOTERRAINSTOP;
extern GameObject_s *CarWashHack;
GameObject_s *CharPlatform_FindObjFromPlatID(CHARPLATFORMSYS_s *system, i32 platform_id);
void StartFlatten(GameObject_s *source, GameObject_s *target);
void AddWaterSplash(GameObject_s *object, NUVEC *position);
i32 CannotKill(GameObject_s *object);
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *PODRACE_ADATA;
void NewRumble(nupad_s *pad, f32 strength, i32 mode);
void NewBuzz(nupad_s *pad, f32 duration, i32 mode);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 delay, i32 mode);
i32 NoLayerKill(GameObject_s *object);
void ClearLastSafeTakeOver(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 mode);
void PlayDieSfx(GameObject_s *object);
i32 StartBigJump(GameObject_s *, NUVEC *, i32, f32, f32, i32, i8);
extern "C" i16 id_SNAKE, id_TRACTOR;
extern AREADATA_s *DAGOBAH_ADATA;
extern f32 MiscTime;
void TakeOverCode(GameObject_s *object, i32 tag_pressed);
void ApplyGravity(GameObject_s *object, f32 *gravity, f32 hover_height, f32 seek_rate, f32 *ground_height);
void InstantKillParts(GameObject_s *object, i32 mode, f32 delay);
i32 IntersectWater(GameObject_s *object);
extern "C" void NewTerrPlatformsOff();

i32 testshadowfix = 1;

namespace {
#include "legoapi/render/core/terrain_types.h"
    DECOMP_ASSERT(sizeof(TERRAIN_SCENE_OBJECT) == 0x50, "TERRAIN_SCENE_OBJECT ABI");

    DECOMP_ASSERT(sizeof(TERRAIN_DISPLAY_OBJECT) == 0xd0, "TERRAIN_DISPLAY_OBJECT ABI");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, transform) == 0xb8, "TERRAIN_DISPLAY_OBJECT transform offset");
    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_OBJECT, scene_object_index) == 0xc0,
                  "TERRAIN_DISPLAY_OBJECT scene_object_index offset");

    DECOMP_ASSERT(offsetof(TERRAIN_DISPLAY_SCENE, object_count) == 0x6c, "TERRAIN_DISPLAY_SCENE object_count offset");

    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_count) == 0x1c, "TERRAIN_GSCENE object_count offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, object_map_present) == 0x58, "TERRAIN_GSCENE object_map_present offset");
    DECOMP_ASSERT(offsetof(TERRAIN_GSCENE, display_scene) == 0x110, "TERRAIN_GSCENE display_scene offset");

} // namespace

extern "C" void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);

extern "C" void SphereDrawEx(NUVEC *centre, f32 radius, f32 vertical_scale, i32 colour);

extern "C" void SphereDraw(NUVEC *centre, f32 radius, f32 vertical_scale);
extern NUVEC TerrWallNorm;

i16 InsidePolLines(f32 point_x, f32 point_y, f32 point_z, f32 edge_a_x, f32 edge_a_y, f32 edge_a_z, f32 edge_b_x,
                   f32 edge_b_y, f32 edge_b_z, NUVEC *normal);

void ScanWallSplineTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);
void TerrainMoveImpactData();
void TerrainImpactNorm();
void Tag_Check(GameObject_s *object);
void BigJumpCode(GameObject_s *object);
f32 GameShadow(GameObject_s *object, NUVEC *position, f32 probe_height, i32 terrain_mask);
void GetSurfaceInfo(GameObject_s *object, i32 update_surface, f32 shadow_height);
extern "C" i32 NewShadowOnPlatform();
extern i32 LEGO_AIPATHCNX_BLOCKAGE;
extern i32 LEGO_AIPATHCNX_FULLTERRAIN;
extern i32 LEGO_AIPATHCNX_WALLSHUFFLE;

void StorePlatImpact();

extern "C" void PlatImpactInfo(NUVEC *normal, i32 *material0, i32 *material1);

namespace {



    static TerrainScanBounds TerrainHandleBounds(const NUVEC &position, const NUVEC &movement, f32 radius);




    // Retail scan callers contain the scale-and-normal reconstruction directly.
#define TERRAIN_SCALE_SCAN_SHAPE(candidate_arg, group_arg, writer_arg)                                                 \
    do {                                                                                                               \
        TERRAIN_SHAPE *&scale_candidate = (candidate_arg);                                                             \
        const TERRAIN_GROUP &scale_group = (group_arg);                                                                \
        TerrainScanWriter *scale_writer = (writer_arg);                                                                \
        if (TerI->object_scale != 1.0f) {                                                                              \
            TERRAIN_SHAPE *source = scale_candidate;                                                                   \
            scale_candidate = &ScaleTerrain[scale_writer->scaled_shape_count];                                         \
            scale_candidate->material[0] = source->material[0];                                                        \
            scale_candidate->material[1] = source->material[1];                                                        \
            scale_candidate->flags = source->flags;                                                                    \
            scale_candidate->normal_flags = source->normal_flags;                                                      \
                                                                                                                       \
            for (i32 vector_index = 0; vector_index < 3; ++vector_index) {                                             \
                scale_candidate->vectors[vector_index].x = source->vectors[vector_index].x;                            \
                scale_candidate->vectors[vector_index].z = source->vectors[vector_index].z;                            \
                scale_candidate->vectors[vector_index].y =                                                             \
                    (source->vectors[vector_index].y + scale_group.origin.y) * TerI->inverse_object_scale -            \
                    scale_group.origin.y;                                                                              \
            }                                                                                                          \
                                                                                                                       \
            if (source->normals[1].y < 65535.0f) {                                                                     \
                scale_candidate->vectors[3].x = source->vectors[3].x;                                                  \
                scale_candidate->vectors[3].z = source->vectors[3].z;                                                  \
                scale_candidate->vectors[3].y =                                                                        \
                    (source->vectors[3].y + scale_group.origin.y) * TerI->inverse_object_scale - scale_group.origin.y; \
                                                                                                                       \
                const f32 length = NuFsqrt(source->normals[1].x * source->normals[1].x +                               \
                                           source->normals[1].y * source->normals[1].y * TerI->object_scale_sq +       \
                                           source->normals[1].z * source->normals[1].z);                               \
                const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                      \
                scale_candidate->normals[1].x = source->normals[1].x * inverse_length;                                 \
                scale_candidate->normals[1].y = source->normals[1].y * TerI->object_scale * inverse_length;            \
                scale_candidate->normals[1].z = source->normals[1].z * inverse_length;                                 \
            } else {                                                                                                   \
                scale_candidate->normals[1].y = 65536.0f;                                                              \
            }                                                                                                          \
                                                                                                                       \
            const f32 length = NuFsqrt(source->normals[0].x * source->normals[0].x +                                   \
                                       source->normals[0].y * source->normals[0].y * TerI->object_scale_sq +           \
                                       source->normals[0].z * source->normals[0].z);                                   \
            const f32 inverse_length = length == 0.0f ? 0.0f : 1.0f / length;                                          \
            scale_candidate->normals[0].x = source->normals[0].x * inverse_length;                                     \
            scale_candidate->normals[0].y = source->normals[0].y * TerI->object_scale * inverse_length;                \
            scale_candidate->normals[0].z = source->normals[0].z * inverse_length;                                     \
            ++scale_writer->scaled_shape_count;                                                                        \
        }                                                                                                              \
    } while (0)


    // Preserve the retail arithmetic ordering and the sweep-diagonal service
    // call, even in the wall-only caller where its result is unused.
#define TERRAIN_GET_SCAN_BOUNDS(bounds_arg, radius_arg, wall_arg)                                                      \
    do {                                                                                                               \
        TerrainScanBounds &scan_bounds = (bounds_arg);                                                                 \
        f32 &bounds_radius = (radius_arg);                                                                             \
        const TerrainQuery_s &scan_query = *TerI;                                                                      \
        if (scan_query.scan_result != 1) {                                                                             \
            const f32 move_length = NuFsqrt(scan_query.movement.x * scan_query.movement.x +                            \
                                            scan_query.movement.y * scan_query.movement.y +                            \
                                            scan_query.movement.z * scan_query.movement.z);                            \
            const TerrainQuery_s &sphere_query = *TerI;                                                                \
            const f32 reach = (wall_arg) ? 0.1f + sphere_query.collision_radius + move_length                          \
                                         : move_length + 0.1f + sphere_query.collision_radius;                         \
            const f32 vertical = reach * sphere_query.object_scale;                                                    \
            scan_bounds.max_x = sphere_query.position.x + 0.05f + reach;                                               \
            scan_bounds.max_z = sphere_query.position.z + 0.05f + reach;                                               \
            scan_bounds.min_x = sphere_query.position.x - 0.05f - reach;                                               \
            scan_bounds.min_z = sphere_query.position.z - 0.05f - reach;                                               \
            scan_bounds.min_y = sphere_query.position.y - 0.05f - vertical;                                            \
            scan_bounds.max_y = sphere_query.position.y + 0.05f + vertical;                                            \
            bounds_radius = reach;                                                                                     \
        } else {                                                                                                       \
            const f32 radius = scan_query.collision_radius;                                                            \
            if (!(scan_query.movement.x > 0.0f)) {                                                                     \
                scan_bounds.min_x = scan_query.position.x + scan_query.movement.x - 0.02f - radius;                    \
                scan_bounds.max_x = scan_query.position.x + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_x = scan_query.position.x - 0.02f - radius;                                            \
                scan_bounds.max_x = scan_query.position.x + scan_query.movement.x + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.y > 0.0f)) {                                                                     \
                scan_bounds.min_y = scan_query.position.y + scan_query.movement.y - 0.02f - radius;                    \
                scan_bounds.max_y = scan_query.position.y + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_y = scan_query.position.y - 0.02f - radius;                                            \
                scan_bounds.max_y = scan_query.position.y + scan_query.movement.y + 0.02f + radius;                    \
            }                                                                                                          \
            if (!(scan_query.movement.z > 0.0f)) {                                                                     \
                scan_bounds.min_z = scan_query.position.z + scan_query.movement.z - 0.02f - radius;                    \
                scan_bounds.max_z = scan_query.position.z + 0.02f + radius;                                            \
            } else {                                                                                                   \
                scan_bounds.min_z = scan_query.position.z - 0.02f - radius;                                            \
                scan_bounds.max_z = scan_query.position.z + scan_query.movement.z + 0.02f + radius;                    \
            }                                                                                                          \
            const f32 dx = scan_bounds.min_x - scan_bounds.max_x;                                                      \
            const f32 dy = scan_bounds.min_y - scan_bounds.max_y;                                                      \
            const f32 dz = scan_bounds.min_z - scan_bounds.max_z;                                                      \
            bounds_radius = NuFsqrt(dx * dx + dy * dy + dz * dz);                                                      \
        }                                                                                                              \
        if (!(wall_arg) && scan_query.object_scale > 1.0f)                                                             \
            bounds_radius += (scan_query.object_scale - 1.0f) * scan_query.collision_radius;                           \
    } while (0)

    // These wall-spline traversals are part of all three retail scan bodies.
#define TERRAIN_COLLECT_WALL_SPLINES(bounds_arg, mask_arg, clear_arg, reset_empty_arg)                                 \
    do {                                                                                                               \
        TerrainScanBounds wall_bounds = (bounds_arg);                                                                  \
        const i32 wall_mask = (mask_arg);                                                                              \
        const bool wall_clear_override = (clear_arg);                                                                  \
        wall_bounds.min_x -= 0.02f;                                                                                    \
        wall_bounds.min_z -= 0.02f;                                                                                    \
        wall_bounds.max_x += 0.02f;                                                                                    \
        wall_bounds.max_z += 0.02f;                                                                                    \
        if ((reset_empty_arg) || (CurTerr->spatial_nodes != NULL && WallSplCount < 64))                                \
            WallSplCount = 0;                                                                                          \
        for (TERRAIN_SPATIAL_NODE *node = CurTerr->spatial_nodes; node != NULL;                                        \
             node = *reinterpret_cast<TERRAIN_SPATIAL_NODE **>(reinterpret_cast<u8 *>(node) - sizeof(void *))) {       \
            const u8 mask = node->field_0x02 >> 8;                                                                     \
            if (mask != 0 && (mask & wall_mask) == 0)                                                                  \
                continue;                                                                                              \
            for (i32 first = 0; first < node->point_count; first += 16) {                                              \
                NUVEC *points = node->points + first;                                                                  \
                i32 end = node->point_count;                                                                           \
                if (points[0].y != 2147483648.0f) {                                                                    \
                    if (!(points[1].y >= wall_bounds.min_x && wall_bounds.max_x >= points[0].y &&                      \
                          points[3].y >= wall_bounds.min_z && wall_bounds.max_z >= points[2].y))                       \
                        continue;                                                                                      \
                    end = MIN(end, first + 16);                                                                        \
                }                                                                                                      \
                for (i32 i = first; i < end; ++i) {                                                                    \
                    NUVEC &a = node->points[i];                                                                        \
                    NUVEC &b = node->points[i + 1];                                                                    \
                    if (!((a.x >= wall_bounds.min_x && wall_bounds.max_x >= b.x) ||                                    \
                          (b.x >= wall_bounds.min_x && wall_bounds.max_x >= a.x)))                                     \
                        continue;                                                                                      \
                    if (!((a.z >= wall_bounds.min_z && wall_bounds.max_z >= b.z) ||                                    \
                          (b.z >= wall_bounds.min_z && wall_bounds.max_z >= a.z)))                                     \
                        continue;                                                                                      \
                    if (WallSplCount < 64) {                                                                           \
                        WallSplList[WallSplCount].position = a;                                                        \
                        WallSplList[WallSplCount + 1].position = b;                                                    \
                        for (i32 p = 0; p < 2; ++p) {                                                                  \
                            u8 *material = WallSplList[WallSplCount + p].material;                                     \
                            material[0] = static_cast<u8>(node->field_0x02);                                           \
                            material[1] = mask;                                                                        \
                            material[2] = 0;                                                                           \
                            material[3] = 0;                                                                           \
                        }                                                                                              \
                        WallSplCount += 2;                                                                             \
                    }                                                                                                  \
                }                                                                                                      \
            }                                                                                                          \
        }                                                                                                              \
        if (wall_clear_override)                                                                                       \
            TerrOverRideScan = NULL;                                                                                   \
    } while (0)

} // namespace

extern i16 debug_index;

// Original 400-pointer table at 0x61e860: 147 named effects, then empty slots.
extern char * debris_name[400];
extern "C" {
extern i32 Grass_Available;
}

extern "C" void TerrainSetImpactData(void *impact_data, i32 *impact_count, i32 maximum_impacts);

void TerrFlush();

extern "C" void TerrTempMemory(void **buffer);

// Constructs the fixed terrain header and its variable-sized group, index and
// platform arrays in the caller's arena. The four bounds arrays and the cell
// assignment array are temporary storage carved backwards from buf_end.
#define IS_SNAN(x) (*(u32 *)&(x) == 0xffc00000)

void NuVecCheckForSNANs(NUVEC *v);


extern "C" void *TerrainInitEx(i32 level_num, void *buf, void *buf_end, i32 options, char *path, void *gscn,
                               i32 scene_id, u32 max_group_indices, u32 max_groups, u32 max_platforms);
void LoadTerrainFile(WORLDINFO *world);
void LoadGrassFile(WORLDINFO *world);
void LoadBridgeFile(WORLDINFO *world);
TERRAIN_TRACK_SLOT *ScanTerrId(void *id);
i32 UnderWater(GameObject_s *object);
i32 CheckCylinder(i32 first_vertex, i32 second_vertex, i32 *vertex_mask, i32 remaining_vertex_mask);

i32 CheckSphere(i32 vertex_index);

i32 CheckSphereTer(NUVEC *position, f32 radius);

i32 HitPoly(f32 primary_start, f32 primary_end, f32 secondary_start, f32 secondary_end, tertype *surface);

void ScanTerrain(i32 scan_type, i32 terrain_mask, i32 scan_flags);

NUVEC TerrainSkin(PLATSKININFO *info, nuvec_s *position, float weight, i32 mode);
void DerotateMovementVector();

void RotateVec(NUVEC *source, NUVEC *destination);

void RotateTerrain(tertype *surface);

void DeRotateTerrain(tertype *surface);

void DeRotatePoint(nuvec_s *point);

namespace {

    DECOMP_ASSERT(sizeof(TERRAIN_IMPACT_RECORD) == 0x1c, "TERRAIN_IMPACT_RECORD ABI");


} // namespace

void TerrainImpact(NUVEC *position, NUVEC *movement, u8 *hit_flags);

// Retail only keeps the IPA-SRA clone, which takes &normal->y.
#if defined(__i386__)
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone))
#endif
static TERRAIN_KILL_PLAYER_ATTR i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y)
    __asm__("_ZL17TerrainKillPlayerP12GameObject_siP7nuvec_s.isra.4");
static i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y);

void TerrainPlayer(GameObject_s *object);


void MakePlayPlanes(GAMECAMERA_s *camera);

DECOMP_ASSERT(sizeof(TERRAIN_DEBUG_RECORD) == 100, "Terrain debug record ABI");

void TerrDrawPlatCol(tertype *terrain, i16 index, i32 colour);




void ScanTerrainPlatform(i32 group_index, i32 terrain_mask);
void ScanTerrainHandel(i32 scan_type, i16 *handle);

extern "C" i32 NewRayCastPlatForm(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                  i32 platform_index, i32 terrain_mask);

extern "C" void *NuScratchAlloc32(i32 size);
extern "C" void NuScratchRelease();
void TerrDraw(tertype *terrain, i16 index);
void TerrDrawPlat(tertype *terrain, i16 index);

extern "C" void DrawHitTerrain(void);

void TerrShowCamTerr();
NUVEC TerrainStaticMtx(PLATSKININFO *info, nuvec_s *position, i32);
extern "C" void NewShapeInit(NUVEC *offset);

void TerrainImpactNorm();
void ScanTerrainPlatform(i32 group_index, i32 terrain_mask);
i32 TerrainBlockOnBlock(WORLDINFO_s *world, pushblock_s *block, nuvec_s *points, float *heights);

void ScanTerrIDRemovePlat(i32 platform_index);
i32 HitWallSpline();

void ScanWallSplineTerrain(i32, i32 terrain_mask, i32);


i32 TerrainImpactPlatform(unsigned char *hit_flags);

void NewTerrStoreAnyInfo();

void TerrainMoveImpactData();
extern "C" void *NuScratchAlloc32(i32 size);
extern "C" void NuScratchRelease();


i32 TerrainPlatformMoveCheck(nuvec_s *, nuvec_s *, i32, i32, i32);

extern "C" void TerrainScanWallSpline(TERRAIN_SPATIAL_NODE *node);

extern "C" void TerrainAddWallSpline(TERRAIN_SPATIAL_NODE *node, TERRSET *terrain);

extern "C" void TerrainRemoveWallSpline(TERRAIN_SPATIAL_NODE *node, TERRSET *terrain);

extern "C" void FullDeflect(NUVEC *normal, NUVEC *movement, NUVEC *result);

void FullDeflectSmallY(NUVEC *normal, NUVEC *movement, NUVEC *result);

i32 FullDeflectTest(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);

void FullDeflectSize(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);

extern "C" void FullReflect(NUVEC *normal, NUVEC *movement, NUVEC *result);

void FullReflectTest(nuvec_s *normal, nuvec_s *movement, nuvec_s *result);


extern "C" void TerrainSetPlatConnectTol(f32 tolerance);

extern "C" void TerrainSetWallDeflectYScale(f32 scale);

i32 PlatformChecks(i32 count, nuvec_s *movement);



i32 TerrainPlatformEmbedded(nuvec_s *movement_delta);
i32 TerrainPlatformMoveCheck(nuvec_s *position, nuvec_s *normal, i32 platform_index, i32 resolve_impact,
                             i32 keep_embedded_disabled);
NUVEC TerCrossProduct(NUVEC *a, NUVEC *b);

void TerrDrawSitu(tertype *terrain, terrsitu_s *situation);

void TerrDrawPlat(tertype *terrain, i16 index);

void TerrDraw(tertype *terrain, i16 index);

void DrawMSitu(i32 index);

extern "C" void terrainpickupinit(char *path, void **buffer);

extern "C" void noterraininit(void);

extern "C" void TerrDrawImpactPol(void);

void DrawWallSpline(float height);

namespace {

    // Target NewScanRot 0x378fb0 uses 0.1f on both horizontal axes when
    // selecting the terrain shapes that can contain the cast point.





} // namespace

void NewScan(nuvec_s *position, i32 terrain_mask, i32 scan_platforms);

void NewScanRot(nuvec_s *position, i32 terrain_mask);

i16 InsideLineF(f32, f32, f32, f32, f32, f32);

namespace {




} // namespace

f32 NewCast(nuvec_s *position, f32 height_above, f32 roof_range);

extern "C" void TerrainWallAng(f32 slope);

extern "C" void ShadowDir(NUVEC *direction);

extern "C" i32 EShadowInfo(void);

extern "C" i32 ShadowRoofInfo(void);

extern "C" i32 EShadowRoofInfo(void);

extern "C" void NewTerrPlatformsOff(void);

extern "C" i32 NewShadowOnPlatform(void);

extern "C" i32 NewShadowOnMSitu(void);

extern "C" void AddCollisionSphere(NUVEC *position, f32 radius);

extern "C" void AddPickupTerr(i32 type, NUVEC *position);



extern "C" i32 AddPickupTerrRot(i32 type, NUMTX *matrix, NUMTX *previous, i32 rotating);

void TerrainSideClamp(NUVEC *axis, NUVEC *position);

i32 TerrShapeSideStep(NUVEC *, NUVEC *, u8 *);

extern "C" void CubeImpact(NUMTX *current, NUMTX *previous, NUVEC *normal, f32 scale, NUVEC *impact);

extern "C" i32 NewTerrainOnAPlatform(void *id);

extern "C" void NewTerrHitInfo(u8 *info);

extern "C" void NewTerrainScaleYMask(NUVEC *position, NUVEC *movement, u8 *hit_flags, i32 object_index, f32 radius,
                                     f32 collision_radius, f32 object_scale, i32 embedded_retry, i32 scan_flags,
                                     i32 terrain_mask);

extern "C" f32 NewRayCastGetTOFI(void);

extern "C" i32 NewRayCastGetImpactTerrainType(void);

extern "C" void NewRayCastGetImpactNormal(NUVEC *normal);

extern "C" f32 NewRayCastGetEmbedDist(void);

extern "C" i32 NewRayCastHitWallSpline(void);

extern "C" i32 NewRayCast(NUVEC *position, NUVEC *movement, f32 radius, i32 scan_flags);

extern "C" i32 NewRayCastScaleYMask(NUVEC *position, NUVEC *movement, f32 radius, f32 scale_y, i32 scan_flags,
                                    u32 terrain_mask);

extern "C" void TerrainPolyEdge(NUVEC *first, NUVEC *second);

extern "C" i32 NewRayCastMask(NUVEC *position, NUVEC *movement, f32 radius, i32 terrain_mask, i32 scan_flags);

extern "C" i32 NewRayCastSet(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon, f32 compare_epsilon,
                             i32 scan_type, i32 scan_flags);

extern "C" i32 NewRayCastSetMask(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                 f32 compare_epsilon, i32 scan_type, i32 terrain_mask, i32 scan_flags);

extern "C" void NewTerrAxisFreedom(TERRAIN_AXIS_FREEDOM_SHAPE *shape, NUVEC *position);

extern "C" i16 NewPlatPickupInst(void *object, i32 type);

extern "C" void ReassignPickupInst(i32 index, i32 type);

extern "C" i32 AddMSituExtraTerrRot(i32 source_index, NUMTX *matrix, NUMTX *previous, i32 rotating, TERRSET *source);

extern "C" void TerrainWallSideSlide(NUVEC *movement, void *id, f32 speed, f32 upward_scale, NUANG angle);

extern "C" i32 NewRayCastSetHandel(NUVEC *position, NUVEC *movement, f32 radius, f32 separation_epsilon,
                                   f32 compare_epsilon, i16 *handle, i32 scan_type);

void ScanTerrainHandel(i32 terrain_mask, i16 *handle);
f32 NewCast(nuvec_s *position, f32 height_above, f32 roof_range);

extern "C" f32 NewShadowHandelEx(NUVEC *position, i32, f32 height_above, f32 height_below, i32 terrain_mask,
                                 i16 *handle);

extern "C" i32 TerrainPlatId(void);

extern "C" i32 TerrainTrackBack(NUVEC *position, NUVEC *direction, f32 radius, f32 backoff, NUVEC *result);

extern "C" i32 TerrainIntensityInfo();

extern "C" i32 TerrainInfo();

extern "C" i32 TerrainInfoExtra();

extern "C" i32 ShadowInfo(void);

extern "C" i32 ShadowIntensityInfo(void);

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.
extern TERRSET *CurTerr;
extern "C" void *NuScratchAlloc32(i32);
extern "C" void NuScratchRelease();

extern "C" void TerrainPlatformOldUpdate(void);

extern "C" i32 PlatformCrush(void);

extern "C" void NewScanInit(void);

namespace {
    static TerrainScanBounds TerrainHandleBounds(const NUVEC &position, const NUVEC &movement, f32 radius);
    static bool TerrainHandleVerticesOverlap(const TerrainScanBounds &bounds, const TERRAIN_SHAPE &shape);
    static void TerrainHandleFilterShapes(TerrainScanWriter *writer, TERRAIN_SHAPE **shapes, i32 count,
                                          const TerrainScanBounds &bounds, i32 terrain_mask, bool rotating);
    static bool TerrainHandleWallOverlap(const TerrainScanBounds &bounds, const TERRAIN_WALL_POINT *wall);


} // namespace

i16 *NewScanHandelFull(nuvec_s *position, nuvec_s *movement, f32 radius, i32 scan_type, i32 terrain_mask);


i16 *NewScanHandelSubset(i16 *subset, nuvec_s *position, nuvec_s *movement, f32 radius, i32 terrain_mask);

extern "C" i16 *NewScanHandel(nuvec_s *position, nuvec_s *movement, f32 radius, i32 scan_type, i16 *subset);

extern "C" void NewRaySetDisablePalt(i32 disabled);

namespace {



} // namespace

void ScanTerrainHandel(i32 terrain_mask, i16 *handle);

extern "C" void TerrainTrackFlush(void);

i32 HitTerrain();

extern "C" f32 NewShadowEx(NUVEC *position, i32, f32 height_above, f32 height_below, i32 terrain_mask);

i32 HitTerrPoly(tertype *surface, i32 group_index);

void PlatformConnect(char *track_id, NUVEC *position_delta, NUVEC *movement_delta, i32 platform_index);

void RayImpact(NUVEC *movement);

// ==== src/legoapi/characters/core/charconfig.cpp: declarations ====
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/world/level.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "globals.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern "C" {
    extern void *perm_debrissys;
    extern u16 SURFACEBITS_DUST;
}










































































































// Retail keeps only the IPA-SRA clones (taking the flags field directly),
// mangled with the original ANIMLIST_s parameter type.
#if defined(__i386__)
#define CC_SET_ANIM_FLAGS_ATTR __attribute__((noinline, noclone, regparm(2)))
// The u32 clone keeps retail's push-based prologue only when size-optimized.
#define CC_SET_ANIM_FLAGS_OS_ATTR __attribute__((noinline, noclone, optimize("Os"), regparm(2)))
#else
#define CC_SET_ANIM_FLAGS_ATTR __attribute__((noinline, noclone))
#define CC_SET_ANIM_FLAGS_OS_ATTR __attribute__((noinline, noclone))
#endif
static CC_SET_ANIM_FLAGS_OS_ATTR void CC_SetAnimFlagsField(NUFPAR *parser, u32 *field, u32 flags)
    __asm__("_ZL15CC_SetAnimFlagsP8nufpar_sP10ANIMLIST_sj.isra.4");

static CC_SET_ANIM_FLAGS_ATTR void CC_SetAnimMiscFlagsField(NUFPAR *parser, u16 *field, u16 flags)
    __asm__("_ZL19CC_SetAnimMiscFlagsP8nufpar_sP10ANIMLIST_st.isra.5");







































































































void CharVariants_Init(CHARVARIANT *variants, i32 count);

i32 LayerFromName(GAMECHARACTERDATA_s *character, char *name);

i32 CharVariant_Find(char *name);

i32 MakeLayerList_Name(CHARACTERMODEL_s *model, i16 *output, u32 mask);

void CharCategories_Init(CHARCATEGORY *categories);

i32 CanWearHatsInFreePlay(i32 character_id);

i32 CharCategory_FindByName(char *name);

i32 CharCategory_IsCategory(GameObject_s *object, i32 index);

void Move_DEFAULT(GameObject_s *);

i32 RandomIDFromFlags(u32 model_flags, u32 game_flags, i32 require_hat, APICHARACTERMODELLIST_s *excluded,
                      i32 exclude_hidden);

extern CHARCONFIG_s charconfig;
extern i32 SecondCharConfigure;
extern i32 configureallcharacters_perm;
extern f32 vehicle_timebase_dist[4];
extern i32 vehicle_timebase_nframes[4];
extern char MiniBuffer[16384];
extern i32 CHARPAK;
extern "C" char ConfigBuffer[0x10000];

// Retail only keeps the clone with strip_comments fixed to 1 (regparm text, filename).
#if defined(__i386__)
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define REDIRECT_TEXT_FILE_ATTR __attribute__((noinline, noclone))
#endif
static REDIRECT_TEXT_FILE_ATTR i32 RedirectTextFile(char *text, char *filename)
    __asm__("_ZL16RedirectTextFilePcS_i.constprop.54");


void CharConfig_ConfigureAll(i32 permanent, NUFPCOMJMP *game_keywords);

void CharConfig_CalculateJumpStats(float jump_speed, float gravity, float *duration, float *height);

void ExtraCharacterFixUpAfterConfig();

// ==== src/legoapi/core/net/netplay.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/net/netplay.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/mission.h"
#include "nu2api/nusound/nusound.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern APICHARACTERMODELLIST_s NetFreePlayModelList[49];
extern i8 net_FreePlayModelCount;
extern u8 net_recievedFreePlayList;

void GetClientMineInfo(nuvec_s **positions, u64 **present, u64 **exploded);

extern void Player_ResetContexts(PLAYERPACKET_s *packet);
extern void CharShadows_Reset(PLAYERPACKET_s *packet);
extern void ResetForceGlow(PLAYERPACKET_s *packet);

void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *);

void FinishLoop_Network();

i32 FinishStatusPacket_LSW(WORLDINFO_s *, STATUSPACKET_s *, i32);

void setObjInNetWaitContext(GameObject_s *object, i32 animation);

// ==== src/legoapi/characters/core/players.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/world.h"
struct HINT_s;

#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/edstubs.h"
#include "gameapi/gui/apimenu.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/gizmos/transport/gizportal.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/characters/motion.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/core/input/timer.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/numath/nutrig.h"

struct MechTouchUIElement;
struct PLAYERITEM_s;
struct PLAYERITEMTYPE_s;
struct PLAYERPACKET_s;
struct SOCKPOSITION_s;
struct TouchHolder;

void CheckForPlayersTurnedOff();

extern "C" NUVEC plr_lastpos;

extern void GetTopBot(GameObject_s *obj);
extern f32 FindGunshipHoverHeight(GameObject_s *obj);
extern f32 GetVehicleSpeedMul(GameObject_s *obj, f32 input);
extern i32 ObjInNarrowSock(GameObject_s *obj, SOCKSYS *socks, i32 level);
extern i32 complexsockposition_forcesock;
extern i32 movegamecamera_forcesock;
extern AREADATA *BONUS_GUNSHIP_ADATA;
extern AREADATA *BATTLEOVERCORUSCANT_ADATA;
extern i32 LevFlag[4];
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern void ResetRumble(RUMBLEPACKET *packet);
extern void CurrentStart(GameObject_s *obj, i32 mode, i32 start);
extern void InitSurfaceInfo(GameObject_s *obj);
extern i32 SetObjOnSurface(GameObject_s *obj, i32 mode);
extern void GizForce_ResetLOS(GameObject_s *obj);
void Arcade_PlayerKilled(i32 player, i32 active);
void ResetPlayerAI(GameObject_s *obj);
void ResetPlayerMoves(GameObject_s *obj);
void SetProtocolDroidDeactivatedAction(GameObject_s *);
i32 TagCharacter(GameObject_s *source, GameObject_s *target, i32 mode);
void NewBuzz(nupad_s *, f32, i32);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);
f32 VehicleTurnOrLoopOffset(GameObject_s *object);
void Player_ClearContext(GameObject_s *object, i32 mode);
void Player_ResetContexts(PLAYERPACKET_s *packet);
extern "C" f32 chattersfxwait;

void Players_Init(void);

// --- Helpers moved from world.cpp ---



void Players_InitPositions(WORLDINFO *world);


extern i8 BoltType_FindIDByName(char *name, WORLDINFO *world);

void PlayerItemTypes_Reset(WORLDINFO *world);

GameObject_s *AddCreature(i32 id, i32 param);


void PreResetCode(GameObject_s *obj);

void PostResetCode(GameObject_s *obj);

void SetHitPoints(GameObject_s *obj, i32 hp);

void RememberPlayerIDs(i32 a, i32 b, i32 c);

// ---- Player start spawn entries ----
extern PLAYERSTARTENTRY PlayerStart[8];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

void *CutScenePlayer_Available(void);

void ChatterSfx(GameObject_s *g, i32 a, float b);

void DrawOffsetCode(GameObject_s *obj, i32 param);

float GetHoverPosY(GameObject_s *obj);

void PlayerItem_Set(PLAYERITEM_s *item, PLAYERITEMTYPE_s *type);

GameObject_s *Player_FindByID(i32 id);

NUVEC *Player_StartPos(GameObject_s *obj);

i32 PlayersDropInOut();

i32 PlayerItem_GotAmmo(PLAYERITEM_s *item);

i32 Players_AveragePos(nuvec_s *position, SOCKPOSITION_s *socket_position);

i32 Players_BothActive();

PLAYERITEMTYPE_s *PlayerItemType_Find(i32 id);

extern void (*Player_ClearContextFn)(GameObject_s *, i32);
void Whip_Release(GameObject_s *);
i32 SpecialMove_ReleaseVictim(GameObject_s *);

void Player_ClearContext(GameObject_s *object, i32 mode);

i32 Player_HasFastBuild(GameObject_s *player);

void PlayerItemTypes_Init(PLAYERITEMTYPE_s *types);

void Player_ResetContexts(PLAYERPACKET_s *packet);

void Player_CopyEssentials(GameObject_s *source, GameObject_s *destination);

i32 Player_HasDeflectBolts(GameObject_s *object);

extern i32 FULLDEBUGTOGGLE;
extern i32 LIFTPLAYER;
extern f32 TOGGLEREPEATTIME;
extern i32 (*Player_ToggleSubCharacterFn)(GameObject_s *, i32, i32);
extern void (*Player_ToggledCharacterFn)(GameObject_s *, i32);
extern FadeSystem FadeSys;
extern i32 LEGOHINT_FREEPLAYTOGGLE;
void Hint_SetComplete(i32 hint_id);
void Move_DEFAULT(GameObject_s *object);
void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);
void GameAudio_PlaySfx(i32 sfx, NUVEC *position, i32 flags, i32 volume);

void Player_ToggleCharacter(GameObject_s *object, i32 direction, i32 sound);

i32 Player_HasInvincibility(GameObject_s *object);

i32 Player_HasDoubleBoltDamage(GameObject_s *object);

void PlayerButton_OnHold_Callback(MechTouchUIElement &element, TouchHolder &);

i32 Player_HasDoubleWeaponDamage(GameObject_s *object);

void PlayerButton_OnLeave_Callback(MechTouchUIElement &element, TouchHolder &holder);

i32 Player_HasDoubleBoltDamage_FromBolt(BOLT_s *bolt);

void PlayerButton_OnClick_Callback_NextButton(MechTouchUIElement &element, TouchHolder &);

extern u32 (*CanPushObstaclesFn)(GameObject_s *);

namespace {

    constexpr f32 kInvalidSurfaceHeight = 2000000.0f;

    void SyncPlayerSpawnPosition(GameObject_s *obj) {
        const NUVEC &position = obj->apiobj.position;

        obj->apiobj.initial_position = position;
        obj->apiobj.collision_position = position;
        plr_lastpos = position;
        obj->apiobj.start_position = position;
    }

} // namespace

void ResetPlayer(GameObject_s *obj, i32 reset_moves, nuvec_s *position, i32 snap_to_surface);

void StarWars_AutoSetAICapabilities(GameObject_s *object);
i32 CanPullLevers(i32 id);
extern f32 DEFAULT_MOVE_RANGE;

void SetFlicker(GameObject_s *object, float duration);

#include "legoapi/characters/core/players_types.h"

static inline PlayerAIFlagBits *PlayerAIFlags(u8 *object, i32 offset) {
    return reinterpret_cast<PlayerAIFlagBits *>(object + offset);
}

void InitPlayerAI(GameObject_s *object);

void ResetPlayerAI(GameObject_s *object);

void StarWars_AutoSetAICapabilities(GameObject_s *object);

i32 MakePlayerList(i32 count);

void ResetPlayerPacket(PLAYERPACKET_s *, CHARACTERDATA_s *);
void ResetCharacterIdle(GameObject_s *, i32, i32);
void SetGameObjectCharacterData(GameObject_s *);
void ResetCoinPacket(COINPACKET_s *);

void ResetPlayerMoves(GameObject_s *object);

void SetToLastSafePos(GameObject_s *object);

i32 AvailableToPlayer(u32 character_flags, i32 weapon_action, i32 context, i32 require_all);

i32 GetNumLocalPlayers();

i32 UnderPlayerControl(GameObject_s *object);

i32 ActivePlayerInRange(nuvec_s *position, float range_squared, float *distance_squared);

GameObject_s *GetOtherActivePlayer(GameObject_s *object);


DECOMP_ASSERT(sizeof(SPECIAL_LAYER_s) == 0xc, "SPECIAL_LAYER_s size");

static SPECIAL_LAYER_s SpecialLayer[] = {
    {&id_BATMAN, "bombbackpack", 0},       {&id_BATMAN, "sonargun", 0},          {&id_BATMAN, "infrared_goggles", 0},
    {&id_BATMAN, "mask_black", 0},         {&id_BATMAN, "mask_blue", 0},         {&id_BATMAN, "mask_red", 0},
    {&id_BATMAN, "cape_black", 0},         {&id_BATMAN, "cape_blue", 0},         {&id_BATMAN, "body_grey_nextgen", 0},
    {&id_BATMAN, "body_grey_high", 0},     {&id_BATMAN, "body_grey_low", 0},     {&id_BATMAN, "body_black_nextgen", 0},
    {&id_BATMAN, "body_black_high", 0},    {&id_BATMAN, "body_black_low", 0},    {&id_BATMAN, "body_blue_nextgen", 0},
    {&id_BATMAN, "body_blue_high", 0},     {&id_BATMAN, "body_blue_low", 0},     {&id_BATMAN, "face_hands_black", 0},
    {&id_BATMAN, "face_hands_blue", 0},    {&id_BATMAN, "face_hands_red", 0},    {&id_BATMAN, "hips_black_nextgen", 0},
    {&id_BATMAN, "hips_black_high", 0},    {&id_BATMAN, "hips_blue_nextgen", 0}, {&id_BATMAN, "hips_blue_high", 0},
    {&id_BATMAN, "hips_red_nextgen", 0},   {&id_BATMAN, "hips_red_high", 0},     {&id_ROBIN, "magnetic_boots", 0},
    {&id_ROBIN, "scuba_gear", 0},          {&id_ROBIN, "hack_pack", 0},          {&id_ROBIN, "vacuum_gun", 0},
    {&id_ROBIN, "nextgen_limbs_green", 0}, {&id_ROBIN, "hires_limbs_green", 0},  {&id_ROBIN, "lowres_limbs_green", 0},
    {&id_ROBIN, "nextgen_limbs_white", 0}, {&id_ROBIN, "hires_limbs_white", 0},  {&id_ROBIN, "lowres_limbs_white", 0},
    {&id_ROBIN, "nextgen_limbs_grey", 0},  {&id_ROBIN, "hires_limbs_grey", 0},   {&id_ROBIN, "lowres_limbs_grey", 0},
    {&id_ROBIN, "nextgen_limbs_blue", 0},  {&id_ROBIN, "hires_limbs_blue", 0},   {&id_ROBIN, "lowres_limbs_blue", 0},
};

i32 Player_HasPurpleForce(GameObject_s *obj);

void FixUpLayers();

u32 AdjustLayerBits(u32 mask, GameObject_s *object);

void SetPlayerGroupPosition(f32 x, f32 y, f32 z);

bool FindNearestPlayerToVec(nuvec_s *position, GameObject_s **nearest_player, float &distance_squared,
                            bool require_character_flags, u32 character_flags);

extern i32 (*LastSafePosExtraFn)(GameObject_s *);
extern void (*GameObjectDimensionsExtraFn)(GameObject_s *);

void CheckForPlayersTurnedOff();

bool FindFurthestPlayerFromVec(nuvec_s *position, GameObject_s **furthest_player, float &distance_squared,
                               bool require_character_flags, u32 character_flags);

void AveragePlayerCurrentSpeedMul();

void SetPlayer();

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

static f32 Condition_OnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_PlayerOnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_EitherPlayerOnForcePlatform(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_OnForcePlatformInit(AISYS_s *, char *name, AISCRIPT_s *);

static void *Condition_ForcePushingInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_ForcePushing(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_EitherPlayerUsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_PlayerUsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_UsingForce(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_UsingForceInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ForceBeingUsed(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_ForceInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ForceAtStart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceAtEnd(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_NumForceObjects(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_NumForceObjectsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_ForceStackComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceStackCompleteInOrder(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ForceFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_ForceCompleteInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ObstacleOpenedByPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ObstacleOpenedByEitherPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *,
                                                  void *argument);

static void *Condition_ObstacleOpenedByPlayerInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ObstacleLockedOpen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ObstacleLockedShut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_GizSpecialInit(AISYS_s *, char *name, AISCRIPT_s *);

static void *Condition_ObstacleInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_ObstacleAtStart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_ObstacleAtEnd(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_OnSpeederBikeInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_OnSpeederBike(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_Player2Active(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_TakenOverInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_TakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *name, void *argument);

static void *Condition_LastLevelInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_LastLevel(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_IsVisibleInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_IsVisible(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_Indy(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PSP(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_CheatProgress(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_ChallengeMode(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_Freeplay(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_MissionMode(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_MissionWon(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_NumInSetAlive(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *, void *argument);

static f32 Condition_IsSetAlive(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *, void *argument);

static void *Condition_IsSetAliveInit(AISYS_s *, char *arg, AISCRIPT_s *);

extern "C" i32 instNuGCutSceneIsFinished(instNUGCUTSCENE_s *cutscene);

static f32 Condition_SockXDistanceToPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_PlayerDeflectingPart(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_CollidingWithOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_OpponentPathPosRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_OpponentToPlayerRange(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_SockDistanceToPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_SockDistanceToOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_FurthestPlayerDistanceAlongSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PlayerDistanceAlongSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PlayerInSock(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_PlayerInSockInit(AISYS_s *, char *name, AISCRIPT_s *);

extern "C" f32 NuAnimEndFrameOld(void *animation);

extern i32 Hub_GetRandomCharType();
extern u8 hub_custodians_finished_loading;
extern "C" {
extern i32 nbaddies_can_see_players;
}

static f32 Condition_NumBaddiesThatCanSeePlayers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_OnSameObjectAsPlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_RandomMapCharsAvailable(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_EitherPlayerWearingHelmet(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_AreaContainsPartyMember(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                             void *argument);

static void *Condition_AreaContainsPartyMemberInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_CharacterTypeExists(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CharacterTypeExistsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_EitherPlayerInMyTriggerArea(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                                 void *);

static f32 Condition_OnDynamicGrapple(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_OnDynamicGrappleInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_AngleAboutMyLocatorToPlayer(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *packet, char *,
                                                 void *argument);

static void *Condition_AngleAboutMyLocatorToPlayerInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_EitherPlayerUsingHatMachine(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

static f32 Condition_EitherPlayerPullingLever(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

static f32 Condition_EitherPlayerUsingPanel(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

static f32 Condition_AreaContainsBaddies(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                         void *argument);

static void *Condition_AreaContainsBaddiesInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_AreaContainsGoodies(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                         void *argument);

static void *Condition_AreaContainsGoodiesInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_PickupBeenTurnedOn(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_PickupBeenTurnedOnInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_AnimationFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_AnimationFinishedInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_RigidAnimFrame(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_RigidAnimFrameInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_FinishedSpline(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_CutSceneFinished(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CutSceneFinishedInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CutSceneStarted(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CutSceneStartedInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CutSceneExists(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_CutSceneExistsInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CutScenePlaying(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_CutScenePlayingInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_Message(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_MessageInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_ScriptParam(AISYS_s *, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *, void *argument);

static void *Condition_ScriptParamInit(AISYS_s *, char *name, AISCRIPT_s *script);

static f32 Condition_Blocking(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_RaceLap(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_MusicOn(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_GotOpponentLOS(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_GotLocatorInSet(AISYS_s *system, AISCRIPTPROCESS_s *process, AIPACKET_s *, char *,
                                     void *argument);

static void *Condition_GotLocatorInSetInit(AISYS_s *system, char *name, AISCRIPT_s *);

i32 CanFightLikeAJedi(GameObject_s *object);

static f32 Condition_CanFightLikeAJedi(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_EitherPlayerPushingSpinner(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_EitherPlayerPushingSpinnerInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_CharacterRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_CharacterRangeInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_MaulShouldRunAway(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_BeenTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_BeenTakenOverInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_LastAttackerRange(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_LastAttackerIsActivePlayer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_CannotReachDestination(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_BeenSpawned(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_ShouldAttackOpponent(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_RespawnLocatorIs(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_RespawnLocatorIsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_BoltsDontGetDeflectedBack(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_HelpWithTriggers(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_DropBackInTimer(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_InMiniCut(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *arg, void *);

static f32 Condition_BigJumpComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_AIOverrideControl(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_AIOverrideControlInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_IsOnScreen(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_IsOnScreenInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_IAmPlayer2(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *);

static f32 Condition_PlayerOnObject(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_OnObjectInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_PlayerOnGround(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_UnderPlayerControl(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static void *Condition_UnderPlayerControlInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_PlayerTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_EitherPlayerTakenOver(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_PartyContainsDroids(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static f32 Condition_OpponentContext(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_InContextInit(AISYS_s *, char *name, AISCRIPT_s *);

static f32 Condition_InContext(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static void *Condition_HitPointsInit(AISYS_s *system, char *name, AISCRIPT_s *);

static f32 Condition_HitPoints(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *packet, char *, void *argument);

static f32 Condition_CurrentHintId(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *);

static void *Condition_HintAvailableInit(AISYS_s *, char *argument, AISCRIPT_s *);

static f32 Condition_HintAvailable(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_HintComplete(AISYS_s *, AISCRIPTPROCESS_s *, AIPACKET_s *, char *, void *argument);

static f32 Condition_EmptyTakeOver(AISYS_s *system, AISCRIPTPROCESS_s *, AIPACKET_s *, char *name, void *);

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

static char *LevelCharacterName(u8 character_index);

static i32 LevelCharacterGlobalID(u8 character_index);

static char *GlobalCharacterName(i32 character_type);

static void *GlobalCharacterHGobj(i32 character_type);

static f32 GetViewRange(i32 character_type);

static f32 GetHearDistance(i32 character_type);

static f32 GetMaxViewHeight(i32 character_type);

static f32 GetMinViewHeight(i32 character_type);

static NUVEC *GetAICreatureOrigin(AISYS *system, AIPACKET *packet);

static APIOBJECT *GetNamedAPIObject(AISYS *system, char *name);

static i32 GlobalCharacterTypeID(char *name);

static i32 GameFindAlternativeSpecialObject(AISYS *, nuhspecial_s *special);

static void GameAILoad(AISYS *system, i32 version, NUGSCN *, VARIPTR *, VARIPTR *);

static void GlobalCharacterRender(NUVEC *position, i16 angle, i32 character, i32, EDCREATURE_s *);

static f32 GetCharacterGoalSpeed(APIOBJECT *object);

static i32 GameAIActionParseSpeed(char *name, u8 *speed);

static char *SpecialRouteCharacterName(u8 route_id);

static i32 LevelCharacterTypeID(char *name);

static u32 AIBigJumpToDestination(APIOBJECT *object, NUVEC *destination);

static u32 AIRespawnOnPath(APIOBJECT *object);

static i32 SpecialRouteCharacterTypeID(char *name);

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

// ==== src/legoapi/characters/motion/camera.cpp: declarations ====
#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "globals.h"
#include <stdlib.h>
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/cutscenes/minicamcut.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"

void Minicam_InitSystem(void);
extern "C" void NuLightSpeedBlurOldCameraPos(const NUMTX *camera);
void GameCam_ResetLookRot(GAMECAMERA_s *camera);
extern i32 newgamecam;
i32 GetMenuID(void);
i32 OnOrInsidePlane(NUVEC *point, NUVEC *plane_point, NUVEC *plane_normal, NUVEC *corrected_point, f32 normal_offset,
                    f32 *distance_out);
void Surface_Deflect(NUVEC *normal, NUVEC *movement, NUVEC *result, i32 mode);
void SpecialMove_Cancel(GameObject_s *object);
void Hint_CancelCurrent(void);
extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *BATTLEOVERCORUSCANT_ADATA;

#include "legoapi/characters/motion/camera_types.h"
extern VIEWCAM_s ViewCam;
// Original data symbol testmovef is float0.005 at 0x617cb0.
extern f32 testmovef;

DECOMP_ASSERT(sizeof(VIEWCAM_s) == 0x28, "ViewCam size");
DECOMP_ASSERT(offsetof(VIEWCAM_s, pitch) == 0x10, "ViewCam pitch offset");
DECOMP_ASSERT(offsetof(VIEWCAM_s, gamepad) == 0x24, "ViewCam gamepad offset");

void GameCam_Blend(GAMECAMERA_s *camera, f32 duration, f32 curve, i32 mode);

void GameCam_Reset(GAMECAMERA_s *camera);

void GameCam_Judder(GAMECAMERA_s *camera, float amount, i32 axis, nuvec_s *source);

void GameCam_HitRoll();

void GameCam_NewShake(GAMECAMERA_s *camera, float amount, float duration, float speed);

void GameCam_HitJudder();

void GameCam_UpdateShake(GAMECAMERA_s *camera, float ambient_amount);

void GameCam_ResetLookRot(GAMECAMERA_s *camera);

void GameCam_UpdateLookRot(GAMECAMERA_s *camera);

void GameCameraMakeMiniCut(nugspline_s *spline, f32 start, f32 end, f32 blend_in, f32 blend_out, i32 borders,
                           i32 hold_until_players_move);

extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern NUVEC ObstacleCamCutPts[2];

void GameCameraMakeMiniCut2(nuvec_s *camera, nuvec_s *target, i32 target_guid, float start, float end, float blend_in,
                            float blend_out, i32 follow_target, i32 follow_camera, i32 borders);

void GameCameraMakeMiniCut3(u32 flags, float distance, i32 pitch, i32 yaw, i32 roll, void *focus, i32 target_guid,
                            nuvec_s *position, float start_time, float blend_in_time, float end_time,
                            float blend_out_time, float blend_time, float hold_time, float max_time, i32 borders,
                            nugspline_s *spline, char mode, char easing);

u16 GameCam_GetAdjustedYRot(GAMECAMERA_s *camera);

extern "C" {

    extern NUVEC *CutoffCameraVec;

f32 CameraEmitterDistance(NUVEC *position);

} // extern "C"

extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL, id_LANDSPEEDER, id_ATST, id_DEWBACK;
extern "C" f32 MainRenderTime;
extern f32 newgamecamtime, cutscenecam_focalLength, CamStopBlend, EMPERORFIGHTA_CAMDYHACK;
extern "C" i32 NewMode;
extern i32 CUTCAMONLY, CUTCAM, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
extern i32 LevFlag[4];
extern NUMTX cutscenecammtx, CutCamMtx;
extern u8 set_cutscenecammtx;
extern NUGSPLINE *hub_minikitviewer_camspl;
extern AREADATA_s *PODRACE_ADATA;
f32 getPodRoll(i32);
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);
f32 PodSprint_InStartCountdown(WORLDINFO_s *);
i32 PodRace_InStartCountdown(WORLDINFO_s *);
extern "C" i32 editor_active;
extern "C" void NuSpeedBlurSetMotionFactors(f32 pan, f32 pull, f32 clamp);
extern "C" void NuLightSpeedBlur(i32 reuse_camera, f32 scale);
i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);
void Minicam_Update();
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
void MakePlayPlanes(GAMECAMERA_s *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern NUVEC CustomisePos[2];
extern f32 HUB_MINIKITVIEWER_CAMDY;
extern f32 PodCamDist;
extern NUVEC ObstacleCamCutPts[2];
extern nugspline_s ObstacleCamCutSpline;
extern i32 ObstacleCamTargetGuid;
extern f32 EMPERORFIGHTA_CAMDYHACK;
extern i32 movegamecamera_forcesock;
extern NUMTX CutCamMtx;
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, path) == 0x40, "Camera context path offset");
DECOMP_ASSERT(offsetof(CAMERA_CONTEXT_PATH_s, flags) == 0x4c, "Camera context flags offset");
NUVEC *Technos_TgtPos(TECHNO_s *);
i32 Grapple_LookAtPos(GameObject_s *, NUVEC *);
void CentreTwoPlayerCamera(NUVEC *, NUVEC *, NUVEC *, NUVEC *);


void MoveGameCamera(GAMECAMERA_s *camera);

void ViewCamDraw();

void KeepOnScreen(GameObject_s *object);

NUVEC *ViewCamGetTgt();

i32 ViewCamGetMode();

void SetDepthOfField();

void SpeedBlur_Apply(WORLDINFO_s *world);

void SpeedBlur_Update();

void ViewCamSetActive(i32 mode, GAMEPAD_s *gamepad);

void KeepPointOnScreen(NUVEC *position, NUVEC *velocity);

void SetCameraMatrices();

GAMEPAD_s *ViewCamGetGamePad();

void CentreTwoPlayerCamera(nuvec_s *center, nuvec_s *player_a, nuvec_s *player_b, nuvec_s *reference);

void KeepVehicleOnScreen(GameObject_s *object, i32 sides, i32 top, i32 bottom);

void do_Pad_flymode_camera(edcam_s *camera, float delta_time, nupad_s *pad);

void InitCameraTargetMaterial();

i32 GoingForwardsAlongNarrowSock(GameObject_s *object);

void GetTopBot(GameObject_s *object);

extern "C" {

extern i32 near_clip_at_cursor;

void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);

void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);

void do_maya_mouse_camera(edcam_s *camera);

void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);

} // extern "C"

// ==== src/legoapi/characters/motion/move.cpp: declarations ====
#include "decomp.h"
#include <stdlib.h>
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/torpedo.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/hatmachine.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/snake.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuqfnt.h"


#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/gizmos/traps/attracto.h"
#include "legoapi/props/objects/signal.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/gizmos/transport/teleport.h"
#include "legoapi/gizmos/door/zipups.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/props/objects/tightrope.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/core/gamemessage.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/render/fx/edsplines.h"
#include "legoapi/render/fx/spline_position.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/gizmo/object/gizmopickup.h"
#include "legoapi/world/level.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nulgtlaser.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

// The retail move.cpp initializer owns these four managed references.
extern MechWeakPtr<MechObjectInterface, 4> nextShootTarget;
extern MechWeakPtr<MechObjectInterface, 4> forceNextAttackOpponent;
extern MechWeakPtr<MechObjectInterface, 4> lungeTarget;
extern MechWeakPtr<MechObjectInterface, 4> BobaRocketTarget;

extern "C" i16 id_R2Q5;

// Original action/context lookup data, with its arrays and pointers in one owner.


extern CHARACTER_CONTEXT_INFO_s * CInfo;
extern ACTIONINFO_s * ActionInfo;
extern EXTRAACTIONDATA_s ExtraActionData[33];

extern AREADATA_s *PODSPRINT_ADATA;
extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *ANAKINSFLIGHT_ADATA;
extern AREADATA_s *DEATHSTARBATTLE_ADATA;
extern "C" i16 id_GRABCONTROL, id_GRABR2CONTROL;

extern float SLAMGRAVITY;
extern float TURNTIME;
float spline_xoffset_speed_roll = 16384.0f;
float spline_xoffset_speed_seek = 4.0f;
float spline_seek_ang = 10.0f;
float spline_seek_pos = 10.0f;

void MovePlayer_DIRECTIONAL(GameObject_s *object);
i32 CanStepBack(GameObject_s *object);
i32 StepBackFromTarget(GameObject_s *object);
i32 ForcePushed_SetTargetMom(GameObject_s *object, f32 *seek_rate);
i32 ForcePushed_YRotation(GameObject_s *object);
static i32 IsAFallAnim(i32 animation);
void MovePlayer_VEHICLEDIRECTIONAL(GameObject_s *object);
i32 MovePlayer_TWIST(GameObject_s *object);
i32 MovePlayer_CIRCLE(GameObject_s *object);
i32 CircleLevel(LEVELDATA_s *level);
f32 ForceTowardsMid(GameObject_s *object);
void MoveInactiveVehicle(GameObject_s *object, i32 mode, GameObject_s **vehicle);
i32 UnderPlayerControl(GameObject_s *object);
void StartTurn(GameObject_s *object);
static void ShootThisFrame(GameObject_s *object, i32 bolt_id, i32 flags);
GIZMOBLOWUP_s *GizmoBlowUp_Target(GameObject_s *, NUVEC *, NUVEC *, f32, f32, i32, i32, i32);
i32 GizmoSys_SetBestBoltTarget(GIZMOSYS *, void *, GameObject_s *, NUVEC *, NUVEC *, f32, f32, i32, i32, i32);
void KeepVehicleOnScreen(GameObject_s *object, i32 sides, i32 top, i32 bottom);
i32 MovePlayer_GUNSHIPIN(GameObject_s *object);
i32 MovePlayer_POD(GameObject_s *object);
void ApplyGravity(GameObject_s *object, float *gravity, float hover_height, float seek_rate, float *ground_height);
float GetVehicleHoverHeight(GameObject_s *object, float *separation_offset);
float FindGunshipHoverHeight(GameObject_s *object);
void TurnCode(GameObject_s *object, i32 mode, GAMEPAD_s *pad);
void DisorientateCode(GameObject_s *object, NUVEC *limit, float range);
float VehicleTurnOrLoopOffset(GameObject_s *object);
void ComboHitFrame(GameObject_s *object, i32 damage);
i32 Grapple_LookAtPos(GameObject_s *object, NUVEC *position);
NUVEC *Technos_TgtPos(TECHNO_s *techno);
void GameCam_UpdateLookRot(GAMECAMERA_s *camera);
void GameCam_ResetLookRot(GAMECAMERA_s *camera);
void GameCam_UpdateShake(GAMECAMERA_s *camera, f32 ambient_amount);
void MakePlayPlanes(GAMECAMERA_s *camera);
u16 SeekRot(u16 current, u16 target, f32 rate);
void SeekVec(NUVEC *result, NUVEC *current, NUVEC *target, f32 rate);
void ShoveSystemCheckGameObject(GameObject_s *object);
i32 GizmoBlowupCheckProximity(WORLDINFO_s *world, GameObject_s *object);
void KeepWeaponOut(GameObject_s *object);
void DropInOutCode(GameObject_s *object);
void TakeHitCode(GameObject_s *object);
void FloatCode(GameObject_s *object);
void SlideCode(GameObject_s *object);
void FlattenCode(GameObject_s *object);
void Hang_MoveCode(GameObject_s *object);
void Ledge_MoveCode(WORLDINFO_s *world, GameObject_s *object);
void LedgeTerrain_MoveCode(GameObject_s *object);
void Climb_MoveCode(GameObject_s *object);
void ForcedBackCode(GameObject_s *object);
void PushCode(GameObject_s *object, i32 allow_grab);
void BackFlipCode(GameObject_s *object);
void TakeOverCode(GameObject_s *object, i32 tag_pressed);
void Glide_MoveCode(GameObject_s *object);
void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 special_animation);
void GizPanel_MoveCode(WORLDINFO_s *world, GameObject_s *object, i32 special_pressed);
void ZipUp_MoveCode(GameObject_s *object, i32 special_pressed);
void Lever_MoveCode(WORLDINFO_s *world, GameObject_s *object);
i32 ThermalDetonator_MoveCode(GameObject_s *object);
void Detonator_MoveCode(GameObject_s *object);
extern "C" void AddVariableShotDebrisEffectTimed1(i32, NUVEC *, i32, f32, i16, i16, NUMTX *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);
void ComboRotateCode(GameObject_s *object, i32 action_held);
void WeaponOutCode(GameObject_s *object);
void WeaponInCode(GameObject_s *object);
void WeaponScalingCode(GameObject_s *object);
void HoldCode(GameObject_s *object);
void SetWeaponOut(GameObject_s *object);
void SetWeaponIn(GameObject_s *object);
void SlowWeaponIn(GameObject_s *object);
void FastWeaponIn(GameObject_s *object, i32 sound);
void SlowWeaponOut(GameObject_s *object);
void FastWeaponOut(GameObject_s *object, i32 sound);
void StartHold(GameObject_s *object);
void BlockSfx(GameObject_s *object);
i32 NewBlockAction(GameObject_s *object);
void MakeJumpReachHeight(GameObject_s *, f32, i32);
void PlayJumpSfx(GameObject_s *, i32);
void FindAnglesZX(NUVEC *, u16 *, u16 *);
extern i32 GrappleSwingMode;
void Hint_SetComplete(i32);
extern i32 (*CanStartHoldFn)(GameObject_s *);
void PlaySabreSfx(char *, GameObject_s *, NUVEC *, i32);
extern "C" f32 *AnimListFrameArray(CHARACTERMODEL_s *, i32);
void HeadMovement(GameObject_s *object);
void CloakMovement(GameObject_s *object);
void HairMovement(GameObject_s *object);
i32 AnakinGreenSabre(GameObject_s *object);
i32 SuperWeirdo(GameObject_s *object);
extern i16 id_IMPERIALGUARD;
extern i16 id_GAMORREANGUARD;
extern i16 id_CAPTAINTARPALS;
extern i16 id_BAT;
extern i16 id_WATTO;
BOLT_s *FindIncomingBolt(GameObject_s *, i32, i32);
PART_s *FindIncomingPart(void *, NUVEC *, f32, u32, f32);
i32 NoLayerKill(GameObject_s *object);
void Punch_Hit(GameObject_s *, GameObject_s *, f32, f32);
void HitRumble(GameObject_s *);
void TakeHitRumble(GameObject_s *, f32);
i32 SpecialMove_Check(GameObject_s *, GameObject_s *);
u32 SpecialMove_GetFlags(i32, u32);
i32 SpecialMove_GetAttackerAction(i32);
i32 SpecialMove_GetVictimAction(i32);
void StartQuickShoot(GameObject_s *, i32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);
extern i32 (*FindSlamOrigin_UseCPosFn)(GameObject_s *);
extern void (*Jump_EndOfLandContextFn)(GameObject_s *);
void ConstantRumble(GameObject_s *object, f32 strength, f32 duration);
extern "C" void PlaySfxAndSetVolume(char *name, NUVEC *position, f32 volume);
i32 GizForce_FindBestForceTarget(GIZFORCESYS_s *, GameObject_s *);
i32 GizForce_StoodOnForce(GIZFORCE_s *, GameObject_s *);
u16 GizForces_AngleToForce(NUVEC *, GIZFORCE_s *);
void SetHeadTarget(GameObject_s *, NUVEC *, i8, f32, f32, f32);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
void GameCam_Blend(GAMECAMERA_s *, f32, f32, i32);
void GameCam_HitRoll(void);
void EndForce(GameObject_s *, i32);
void ResetForceGlow(PLAYERPACKET_s *);
extern f32 FORCEGLOWTIME;
extern f32 ForceThrowGravity;
extern f32 ForceThrowSpeed;
extern f32 MaulF_ForceThrowSpeed;
extern f32 MaulF_ForceThrowGravity;
extern f32 DookuC_ForceThrowSpeed;
extern f32 DookuC_ForceThrowGravity;
PART_s *Part_FindFromHSpecial(nuhspecial_s *);
PART_s *GizForce_Throw(GameObject_s *, GIZFORCE_s *, f32, f32, i32);
extern f32 DEACTIVATEDTIME;
extern i32 ForcePush_SuperMindTrick;
extern i32 ForcePush_SuperPush;
extern i32 ForcePush_Waft;
i32 ZapTarget(GameObject_s *);
NUVEC *GetZapOrigin(GameObject_s *);
i32 CannotKill(GameObject_s *);
i32 FaceOpponent(GameObject_s *object, NUVEC *position);
void SetProtocolDroidDeactivatedAction(GameObject_s *);
void NewBuzz(nupad_s *, f32, i32);
void Arcade_AIKilled(i32);
extern i16 * objhitobj_killparts_yrot;
extern i32 objhitobj_noimpactsfx;
void Player_ClearContext(GameObject_s *, i32);
void Player_ResetContexts(PLAYERPACKET_s *);
void PlayDieSfx(GameObject_s *);
void GameAudio_PlaySfx(i32, NUVEC *, i32, i32);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);
// Retail keeps several of this file's static helpers only as GCC clones; these
// declarations pin the clone names and register conventions.
#if defined(__i386__)
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone, regparm(1)))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define MOVE_CLONE_ATTR1 __attribute__((noinline, noclone))
#define MOVE_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

static MOVE_CLONE_ATTR2 void CommunicateCode(GameObject_s *, i32)
    __asm__("_ZL15CommunicateCodeP12GameObject_sii.constprop.74");
static MOVE_CLONE_ATTR1 void AwkwardShapeCode(GameObject_s *)
    __asm__("_ZL16AwkwardShapeCodeP12GameObject_si.constprop.78");
static void SelectOpponent(GameObject_s *, f32, f32, i32, i32);
static void PunchCode(GameObject_s *, i32, i32, i32, i32, f32);
static i32 ShootCode(GameObject_s *, i32, i32, i32, i32, i32);
static void ForcePushed_MoveCode(GameObject_s *);
static void DeactivatedCode(GameObject_s *);
static void ZapCode(GameObject_s *, i32, i32);
static void ShootThisFrame(GameObject_s *, i32, i32);
static void FireCode(GameObject_s *, i32, i32, f32, i32);
f32 FindGunshipHoverHeight(GameObject_s *);
void Move_VEHICLE(GameObject_s *);
void CableCode(GameObject_s *, i32, f32);
void KillParts(GameObject_s *, i32, i32, i32, f32, i32, u16 *);
extern "C" i16 id_BASKETCANNON;
// Retail keeps only the IPA-SRA clone of this function.
static MOVE_CLONE_ATTR2 void ForceCode(GameObject_s *, i32, i32) __asm__("_ZL9ForceCodeP12GameObject_siii.isra.29");
static inline void SelfDestructCode(GameObject_s *, i32);
void JetPackCode(GameObject_s *, i32, i32, i32);
void ChatterSfx(GameObject_s *, i32, f32);
static void ShieldCode(GameObject_s *);
void KeepWeaponIn(GameObject_s *);
void BigJumpCode(GameObject_s *);
void InstantKillParts(GameObject_s *, i32, f32);
EXPLOSION *Detonate(NUVEC *, u16);
void AddFancyMessage(char *, f32, f32, f32, f32, i32, i32);
extern i16 tCHEAT_SELFDESTRUCT;
extern i16 id_BATTLEDROID, id_BUZZDROID, id_SUPERBATTLEDROID, id_PROBEDROID;
extern i16 id_NAFFDROID1, id_NAFFDROID2, id_NAFFDROID4, id_MOUSEDROID;
static void DodgeCode(GameObject_s *, i32, i32);
void Grapple_MoveCode(GameObject_s *);
void SpecialMove_VictimCode(GameObject_s *);
void Tag_Check(GameObject_s *);
i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32);
i32 PodLevel(AREADATA_s *);
void KeepOnScreen(GameObject_s *);
void UpdateSnakeBody(GameObject_s *);
void TractorBeamCode(GameObject_s *);
void LoopCode(GameObject_s *, i32, i32, GAMEPAD_s *, i32);
void CableCode(GameObject_s *, i32, float);
extern f32 max_cable_length;
void EngineNoiseCode(GameObject_s *, i32);
void Buck_MoveCode(GameObject_s *, i32);
void CatchUpCode(GameObject_s *, f32, f32, i32);
GRAPPLE *Grapple_AddDynamic(void *, i32);
void AddSurfaceRipples(GameObject_s *);
extern i16 id_SNAKE;
extern "C" i16 id_ATAT, id_DODGEM;
extern i16 id_YODA, id_YODAGHOST;
extern i16 id_C3PO, id_TC14;
extern i16 id_DROIDEKA, id_RANCOR;
extern "C" i16 id_ATST, id_MINIATST, id_ATST_LOWRES, id_MINIATAT, id_MINIATTE;
void SecurityDoor_MoveCode(WORLDINFO_s *, GameObject_s *);
void Batarang_MoveCode(GameObject_s *);
bool FireBountyHunterRocket(GameObject_s *);
extern f32 DIEAIRSPEED;
extern f32 DIEAIRJUMPSPEED;
void SetObjAsHeadTarget(GameObject_s *, GameObject_s *, i8, f32, f32, f32);
void KillRumble(GameObject_s *);
void PlayHurtSfx(GameObject_s *);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);
extern i16 id_BATTLEDROIDSECURITY, id_PKDROID, id_PITDROID;
void FindForcePushTarget(GameObject_s *, i32, i32);
void ReleaseForce(GameObject_s *, i32);
void BobaRocket_Move(PART_s *, f32);
void Boulder_Move(PART_s *, f32);
void Boulder_Kill(PART_s *, i32);
extern "C" void NewPartRotation(PART_s *);
void PlayGruntSfx(GameObject_s *);
extern "C" i32 dagobah_training;
extern i16 id_LUKESKYWALKERDAGOBAH;
extern i32 newgamecam;
extern f32 newgamecamtime;
extern FadeSystem FadeSys;
i32 GetMenuID(void);

// Multiplies the ordinary camera's per-frame position blend.  The original
// keeps this as writable camera state (default 1.0), rather than folding it
// into the socket seek rate.
extern f32 CamStopBlend;
extern i32 netcamera;

extern "C" {
    extern i16 id_BODYGUARD;
    extern i16 id_GRIEVOUS;
}

#include "legoapi/characters/motion/move_types.h"



static void ShiftPadDirectionHistory(GameObject_s *object, GAMEPAD_s *pad) {
    i32 player_index = -1;
    if (Player[0] == object) {
        player_index = 0;
    } else if (Player[1] == object) {
        player_index = 1;
    }
    if (player_index == -1) {
        return;
    }

    PadOldSpeed2[player_index] = PadOldSpeed[player_index];
    PadOldAngle2[player_index] = PadOldAngle[player_index];
    PadOldSpeed[player_index] = pad->input_magnitude;
    PadOldAngle[player_index] = pad->input_angle;
}

GAMEPAD_s *ViewCamGetGamePad();
void MovePlayer_ROLLING(GameObject_s *object);
extern f32 Hub_PadSpeed[2];
extern u16 Hub_PadAngle[2];
extern f32 drop_back_in_timer;
extern i32 LIFTPLAYER;
extern f32 OFFSCREEN_CATCHUP_TIME;

pushblock_s *BlockInBlock(WORLDINFO_s *, pushblock_s *, i32, pushblock_s **);
void MoveBlocksOverBlock(WORLDINFO_s *, pushblock_s *, i32, nuvec_s *);
void MoveBlocks(WORLDINFO_s *world, pushblock_s *block, i32 index, nuvec_s *velocity);

void MovePlayer(GameObject_s *object);

void Move_BEAST(GameObject_s *object);

void Move_BARMAN(GameObject_s *object);

void Move_CANNON(GameObject_s *object);

void Move_WALKER(GameObject_s *object);


void Move_CRITTER(GameObject_s *object);

void Move_DEFAULT(GameObject_s *object);

void Move_DRAGBOMB(GameObject_s *object);

void Move_DROIDEKA(GameObject_s *object);

i32 PodLevel(AREADATA_s *area);
f32 ForceAlongSock(GameObject_s *object);
extern LEVELDATA_s *PODRACEB_LDATA;
extern "C" {
    extern f32 pod_roll[2], pod_roll_target[2];
    extern i32 podrace_section;
}

i32 MovePlayer_POD(GameObject_s *object);

void Move_CHARACTER(GameObject_s *object);

void Move_GEONOSIAN(GameObject_s *object);

void Move_HOVERDROID(GameObject_s *object);

void MovePlayerSpline(GameObject_s *object);

i32 TwistLevel(LEVELDATA_s *level);
extern f32 CurrentSpeed;
extern f32 avg_currentspeed_mul;

f32 ForceAlongSock(GameObject_s *object);

i32 MovePlayer_TWIST(GameObject_s *object);

extern f32 speeder_smoke_rate;
extern f32 speeder_offset_speed;
extern f32 GetVehicleSpeedMul(GameObject_s *, f32);

void Move_SPEEDERBIKE(GameObject_s *object);

i32 MovePlayer_CIRCLE(GameObject_s *object);



// Retail splits the guard off as an inline head; the rest is this partial clone.
static MOVE_CLONE_ATTR1 void SelfDestructCodeBody(GameObject_s *object)
    __asm__("_ZL16SelfDestructCodeP12GameObject_si.part.10");



void PeriscodeCode(GameObject_s *object);

void Move_DROIDGENERIC(GameObject_s *object);

void GunShip_DragBombSeekBlowUp(GameObject_s *object);

void MovePlayer_ROLLING(GameObject_s *object);

void MoveSplinePosition(SPLINEPOS_s *position, f32 distance);

void MoveBlocksOverBlock(WORLDINFO_s *world, pushblock_s *block, i32 excluded, nuvec_s *velocity);

void MoveInactiveVehicle(GameObject_s *object, i32, GameObject_s **followed_object);

i32 GunshipInLevel(LEVELDATA_s *level);
extern i32 LevFlag[4];
// Original exported level control, initialized to 2 at 0x623d00.
extern i32 GUNSHIPAHACK;

i32 MovePlayer_GUNSHIPIN(GameObject_s *object);

void Move_REPUBLICGUNSHIP(GameObject_s *object);

void Move_SUPERBATTLEDROID(GameObject_s *object);

void MovePlayer_DIRECTIONAL(GameObject_s *object);

#include <stdlib.h>
extern "C" i16 id_LANDSPEEDER, id_WOOKIEFLYER, id_STAP2;
extern AREADATA_s *SPEEDERCHASE_ADATA;
extern GameObject_s *GetOtherActivePlayer(GameObject_s *);
extern GameObject_s *CarWashHack;
extern i32 IDLESPEEDINNARROWSOCKSONLY;
extern f32 GetVehicleSpeedMul(GameObject_s *, f32);
extern i32 GoingForwardsAlongNarrowSock(GameObject_s *);
extern f32 PodSprint_InStartCountdown(WORLDINFO_s *);
extern f32 DeathStar2BattleFire_GetSlowDownMul(GameObject_s *);
extern void VehicleCollisionCode(GameObject_s *);

i32 NeedsPretendAnim(GameObject_s *object);

void MovePlayer_VEHICLEDIRECTIONAL(GameObject_s *object);

i32 GetShootDirection_LSW(GameObject_s *object, nuvec_s *direction);

void GetShootOrigin_LSW(GameObject_s *object, nuvec_s *position);

extern i32 PODSPRINTDEB;

void PodCollisionCode(GameObject_s *object);

// These pod tuning values are zero-initialized in the original image.
extern f32 PosSeekPitch[2];
extern f32 PODBOOSTHEIGHT;

f32 FindPodHoverHeight(GameObject_s *object);
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
extern "C" void PlaySfxByIdAndSetPitch(i32 sfx_id, nuvec_s *position, f32 pitch);

void Move_POD(GameObject_s *object);

extern f32 zam_smoke_rate;
extern f32 test_hoverheight;
extern void (*LEGO_SET_SLOWDOWNFN)(GameObject_s *);


void Move_VEHICLE(GameObject_s *object);

void Move_ATAT(GameObject_s *object);

void Move_JAWA(GameObject_s *object);


extern i32 objopponent_ignoreaiopponent;
extern i32 test_ai_combo;
extern i32 CanPunchGirls;

GameObject_s *ObjOpponent(GameObject_s *object, f32 range, f32 extra_radius, i32 allow_untargeted, i32 mode,
                          i32 player_filter);



// Retail re-tests the three block animations at each use instead of caching a flag.
#define HAS_BLOCK_ANIMS(animations)                                                                                    \
    ((animations)[0x1a] != NULL || (animations)[0x1b] != NULL || (animations)[0x1c] != NULL)






// Retail inlines the fade and step checks into each caller; this is the remaining partial clone.
static MOVE_CLONE_ATTR2 void ForceGlowCode(GameObject_s *object, i32 model)
    __asm__("_ZL13ForceGlowCodeP12GameObject_si.part.34");


void CableCode(GameObject_s *object, i32 special_pressed, f32);

i32 LightSabre_ColourFromObj(i32 model, i32 *glow_model);

void LightSabreDebris(GameObject_s *object);


i32 ForcePushed_SetTargetMom(GameObject_s *object, float *seek_rate);

i32 FaceOpponent(GameObject_s *object, NUVEC *position);

i32 ForcePushed_YRotation(GameObject_s *object);

i32 ForcePushed_SuperPush_Occurring(GameObject_s *first, GameObject_s *second);


i32 ZapTarget(GameObject_s *object);



// Retail splits the context test off as an inline head; the rest is this partial clone.
static MOVE_CLONE_ATTR2 void ForcePushCodeBody(GameObject_s *object, i32 held)
    __asm__("_ZL13ForcePushCodeP12GameObject_sii.isra.32.part.33");



void NewRumbleAllPlayers(f32, f32, i32, i32);


// Retail keeps only the IPA-SRA clone of this function.
static MOVE_CLONE_ATTR2 void ForceThrowCode(GameObject_s *object, i32 pressed)
    __asm__("_ZL14ForceThrowCodeP12GameObject_sii.isra.13");


// Retail keeps only the IPA-SRA clone of this function.
static MOVE_CLONE_ATTR2 void ForceDeflectCode(GameObject_s *object, i32 pressed, i32 held)
    __asm__("_ZL16ForceDeflectCodeP12GameObject_siii.isra.31");


void Move_WEIRDO(GameObject_s *object);

void Move_JEDI(GameObject_s *object);

void MovePlayer_NETWORK(GameObject_s *object);






extern u8 show_lever_hint;

extern i32 show_autojump_hint;

i32 Slam_Start(GameObject_s *object, f32 speed);

void StartLunge(GameObject_s *object, f32 speed, f32 height);

extern "C" {
    extern i16 id_ATST, id_MINIATST, id_ATST_LOWRES, id_ATAT, id_MINIATAT, id_MINIATTE;
}

i32 CanObjSlide(GameObject_s *object, i32);

i32 StartSlide(GameObject_s *object, i32 check_contact);

i32 CanStepBack(GameObject_s *object);

void FlattenCode(GameObject_s *object);

i32 Glide_Start(GameObject_s *object);

void StartJetPackFall(GameObject_s *object, i32 fall);
void PlayLandSfx(GameObject_s *object, i32 type, i32 surface);


void JetPackCode(GameObject_s *object, i32 jump_pressed, i32 fall, i32);

float SeekLinearF(float current, float target, float step);

void StartLaunch(GameObject_s *object);

float GetVehicleHoverHeight(GameObject_s *object, float *separation_offset);


void ApplyGravity(GameObject_s *object, float *gravity, float hover_height, float seek_rate, float *ground_height);

i32 SetObjTarget(GameObject_s *object, GameObject_s *target);

i32 SnapPosTaken(WORLDINFO_s *world, pushblock_s *, nuvec_s *position, i32 excluded_index);

void StartFlatten(GameObject_s *source, GameObject_s *target);

void Hang_MoveCode(GameObject_s *object);

void HoldCode_Copy(GameObject_s *object);

void SetHeadTarget(GameObject_s *object, NUVEC *position, i8 priority, f32 time, f32 minimum_delay, f32 maximum_delay);

extern i32 DoubleJump_JediSlam;
extern f32 SLAMJUMPSPEED;
void PlayLandSfx(GameObject_s *, i32, i32);
void AddWaterSplash(GameObject_s *, NUVEC *);

i32 StartBackFlip(GameObject_s *object);


void UpdateLastSafePosition(GameObject_s *object);

void FindSlamOrigin(GameObject_s *object, NUVEC *start, NUVEC *end);

void JumpCode(GameObject_s *object, i32 jump_pressed, i32 jump_held, u32 animation_set, i32 action_pressed,
              i32 action_held, i32 hit_effect);

i32 MosEisleyC_PastBarrier(GameObject_s *object);

void Glide_MoveCode(GameObject_s *object);

void ForcedBackCode(GameObject_s *object);

i32 SetObjOnSurface(GameObject_s *object, i32 mode);

void TurnCodeCamSafe(GameObject_s *object, numtx_s *matrix);

void RotateGameMatrix(numtx_s *matrix, i32 order, u16 x, u16 y, u16 z);

i32 Hang_SetTargetMom(GameObject_s *object);

i32 Glide_SetTargetMom(GameObject_s *object);

void SetObjAsHeadTarget(GameObject_s *object, GameObject_s *target, signed char, float, float, float);

i32 Slide_SetTargetMom(GameObject_s *object, u16 input_angle, float input_speed);

i32 StepBackFromTarget(GameObject_s *object);

float FindGunshipHoverHeight(GameObject_s *object);

void ApplyGravity_Network(GameObject_s *object);

extern "C" i16 id_TIEFIGHTER, id_XWING;
void VehicleCollisionCode(GameObject_s *object);

float VehicleTurnOrLoopOffset(GameObject_s *object);

i32 WallShuffle_SetTargetMom(GameObject_s *object, u16 input_angle);

u16 SeekRot(u16 current, u16 target, f32 rate);

void SeekVec(NUVEC *result, NUVEC *current, NUVEC *target, f32 rate);

u16 TurnRot(u16 current, u16 target, i32 speed, i32 *difference_out);

void HoldCode(GameObject_s *object);

f32 SeekValF(f32 current, f32 target, f32 rate);

void TurnCode(GameObject_s *object, i32 mode, GAMEPAD_s *pad);

void FloatCode(GameObject_s *);

void StartHold(GameObject_s *object);

void SlideCode(GameObject_s *object);

void StartTurn(GameObject_s *object);

// Retail splits the context test off as an inline head; the rest is this partial clone.
static MOVE_CLONE_ATTR1 void GrabCodeBody(GameObject_s *object) __asm__("_ZL8GrabCodeP12GameObject_s.part.15");



extern i16 id_RANCOR;
extern i16 id_DEWBACK;
extern i16 id_BIGGUN;
extern i16 id_BANTHA;
extern i16 id_ATAT;
extern i16 id_CATAPULT;
extern i16 id_FIRETRUCK;
extern i32 TERRAINMASK_NONWEAPON;
void PushAway(NUVEC *, f32, NUVEC *, NUVEC *, GameObject_s *, GameObject_s *, f32, u32);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

DECOMP_ASSERT(sizeof(AWKWARDSHAPESPHERE_s) == 12, "Awkward shape sphere size");
DECOMP_ASSERT(offsetof(AWKWARDSHAPESPHERE_s, radius) == 8, "Awkward shape sphere radius offset");







i32 PlayerItem_GotAmmo(PLAYERITEM_s *);
extern "C" f32 animduration_blendouttime;
extern i16 id_GEONOSIAN, id_MINIATST, id_ATST_LOWRES, id_ATAT, id_MINIATAT, id_MINIATTE, id_SENTRYDROID;
void Move_CANNON(GameObject_s *);
void SetWeaponIn(GameObject_s *);



extern i16 id_EWOK;
extern i16 id_WICKET;
extern i16 id_CAPTAINTARPALS;
extern i16 id_SKELETON;
void StartJetPackFall(GameObject_s *, i32);

void Move_CHARACTER(GameObject_s *object);

extern "C" PART_s *FindPart(NUVEC *, i32, GameObject_s *);


void Buck_MoveCode(GameObject_s *, i32);

void Move_BEAST(GameObject_s *object);

void ResetForceBack();

void SetForceBack(GameObject_s *object, nuvec_s *position, float radius, i32 type);

void DrawForceBackEffect(nuhspecial_s *special);

// ==== src/legoapi/characters/motion/gameanim.cpp: declarations ====
#include "decomp.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "gameapi/edtools/edfile.h"
#include "globals.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutexanm.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nuquat.h"
#include "nu2api/numath/nuvec4.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/sfx.h"

#include <float.h>
#include <string.h>

void EvalAnim(nuhspecial_s *special, f32 frame, numtx_s *matrix, i32 include_instance_translation);
i32 UseFallAnim(GameObject_s *object);
i32 GetDefaultIdle(GameObject_s *object);
i32 SetProtocolDroidFallAnim(GameObject_s *object);
// Retail uses translation-unit-local helpers with compiler-inferred private
// calling conventions; keep their shared animation-mode call sites intact.
static void MoveAnim_Manage(GameObject_s *object, f32 movement_speed, i32 allow_tiptoe, i32 weapon_variant);
static void MoveAnim_Check(GameObject_s *object);
static void JumpAnimCode(GameObject_s *object);
static bool JumpAnim_HasAction(const GameObject_s *object, i16 action);
static GAMECHARACTERDATA *GetGameCharacterData(GameObject_s *object);
void UpdateCharacterIdle(GameObject_s *object);
void AutoWeaponOnOff(GameObject_s *object);
void AddFootSteps(GameObject_s *object);
f32 PodSprint_InStartCountdown(WORLDINFO_s *world);
i32 PodRace_InStartCountdown(WORLDINFO_s *world);
void SetPodMergeAnims(ANIMPACKET_s *packet, i32 index);
extern "C" i16 id_ANAKINSPODGREEN;
extern "C" void PlaySfxByIdAndSetVolume(i32 sfx_id, NUVEC *position, f32 volume);
i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

extern i16 id_BODYGUARD;
extern i16 id_IMPERIALGUARD;
extern i16 id_YODA;
extern i16 id_YODAGHOST;
extern i16 id_GONKDROID;
extern i16 id_JUMBOHOMINGDROID;
extern i16 id_GAMORREANGUARD, id_PRINCESSLEIASLAVE, id_LAMASU, id_TAUNWE;
extern i16 id_PENGUIN, id_UMBRELLA, id_GLIDEPACK;
extern f32 hub_jabbaawake;

#include "legoapi/characters/motion/gameanim_types.h"






void Animate_ATAT(GameObject_s *object);

void Animate_POD(GameObject_s *object);


void Animate_JEDI(GameObject_s *object);


void AnimatePlayer(GameObject_s *object);

void Animate_BEAST(GameObject_s *object);

void Animate_BARMAN(GameObject_s *object);

void Animate_CANNON(GameObject_s *object);

void Animate_WALKER(GameObject_s *object);

void Animate_CRITTER(GameObject_s *object);

void Animate_WEIRDO(GameObject_s *object);

void Animate_DEFAULT(GameObject_s *object);

extern i32 NeedsPretendAnim(GameObject_s *object);

void Animate_VEHICLE(GameObject_s *object);

void Animate_PROTOCOL(GameObject_s *object);

void Animate_DROIDEKA(GameObject_s *object);

void GameAnimSet_Draw(GAMEANIMSET_s &set);

i32 GameAnimSet_Play(GAMEANIMSET_s *set, float speed, i32 evaluate_state);

i32 GameAnimSet_Stop(GAMEANIMSET_s *set);

void Animate_ASTROMECH(GameObject_s *object);

void Animate_CHARACTER(GameObject_s *object);

void Animate_GEONOSIAN(GameObject_s *object);

i32 GameAnimSet_Reset(GAMEANIMSET_s *set);

void Animate_HOVERDROID(GameObject_s *object);

GAMEANIMSET_s *GameAnimSet_Create(variptr_u *buf, variptr_u *buf_end, GAMEANIMOBJPOOL_s *object_pool,
                                  GAMEANIMSYS_s *system);

void Animate_BATTLEDROID(GameObject_s *object);

void Animate_SPEEDERBIKE(GameObject_s *object);

i32 GameAnimSet_Playing(GAMEANIMSET_s *set);

void GameAnimSet_EvalAnim(GAMEANIMSET_s *set);

GAMEANIMOBJ_s *GameAnimSet_AddObject(GAMEANIMSET_s *set, nuhspecial_s *special, float start_frame, float end_frame,
                                     i32 append);

i32 GameAnimSet_JumpToEnd(GAMEANIMSET_s *set);

void GameAnimSet_SetOffset(GAMEANIMSET_s *set, NUVEC *offset);

f32 GameAnimSet_GetAnimPos(GAMEANIMOBJ_s *object);

void GameAnimSet_SetAnimPos(GAMEANIMOBJ_s *object, float position);

void GameAnimSet_SetTFactor(GAMEANIMSET_s *set, float factor);

void Animate_REPUBLICGUNSHIP(GameObject_s *object);

i32 GameAnimSet_JumpToStart(GAMEANIMSET_s *set);

void Animate_SUPERBATTLEDROID(GameObject_s *object);

void GameAnimSet_RemoveObject(GAMEANIMSET_s *set, GAMEANIMOBJ_s *object);

void GameAnimSet_ScaleFParam1(GAMEANIMSET_s *set, float scale);

i32 GameAnimSet_SetRepeating(GAMEANIMSET_s *set, i32 repeating);

void GameAnimSet_EvaluateState(GAMEANIMSET_s *set);

i32 GameAnimSet_GetAveragePos(GAMEANIMSET_s *set, NUVEC *position, i32 frame_selection, i32 include_animated,
                              i32 include_static);

GAMEANIMSET_VISIBILITY GameAnimSet_GetVisibility(GAMEANIMSET_s *set);

void GameAnimSet_JumpToAnimPos(GAMEANIMSET_s *set, float position);

void GameAnimSet_RemoveSpecial(GAMEANIMSET_s *set, nuhspecial_s *special);

void GameAnimSet_SetVisibility(GAMEANIMSET_s *set, i32 visibility);

void GameAnimSet_DrawReflection(GAMEANIMSET_s *set, i32 axis, float offset, numtx_s *matrix);

GAMEANIMOBJ_s *GameAnimSet_AddObjectByName(GAMEANIMSET_s *set, nugscn_s *scene, char *name, float start_frame,
                                           float end_frame, i32 append, GIZMOSYS_s *gizmo_sys, char *prefix,
                                           char *suffix);

void GameAnimSet_AddToSystemList(GAMEANIMSET_s *set);

f32 GameAnimSet_AutoSetReflectY(GAMEANIMSET_s *set, nuvec_s *position, numtx_s *matrix);

f32 GameAnimSet_GetCurrentFrame(GAMEANIMSET_s *set);

GAMEANIMOBJPOOL_s *GameAnimSet_CreateObjectPool(variptr_u *buf, variptr_u *buf_end, i32 object_data_size,
                                                i32 capacity);

i32 GameAnimSet_IsAnimationReset(GAMEANIMSET_s *set);

void GameAnimSet_RemoveAllObjects(GAMEANIMSET_s *set);

i32 GameAnimSet_GetCentreAndRadius(GAMEANIMSET_s *set, NUVEC *centre, f32 *radius, i32 frame_selection,
                                   i32 include_animated, i32 include_static);

f32 GameAnimSet_GetCompletionRatio(GAMEANIMSET_s *set);

void GameAnimSet_RemoveFromSystemList(GAMEANIMSET_s *set);

i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                            i32 joint_count, i32 first_joint, NUVEC *root_translation);
i32 ANI_SimpleAni3PlayerV4Joint_Blend_Quat3W(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                             i32 joint_count, i32 first_joint, NUVEC *root_translation);

extern "C" {

    i32 ANI_SimpleAni3PlayerV4Joint_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, i32 joint_count,
                                              i32 first_joint);
    i32 ANI_SimpleAni3PlayerV4Joint_Blend_EulerQuat(ani3_animheader_s *anim, f32 frame, nuanimbuff_s *buffer, f32 blend,
                                                    i32 joint_count, i32 first_joint, NUVEC *root_translation);

void ANI_FixUpAddrs(ani3_animheader_s *anim, isize delta, i32);

    // Original @0x2c17d0. The non-quaternion ANI4 player uses a compact
    // four-samples-per-word curve stream. Only groups enabled by the node's
    // translation/rotation/scale flags occupy space in that stream.

f32 GetInstAnimEndFrame(nugscn_s *scene, nuinstanim_s *instance_animation);

extern NUJOINTPROCANIMFN JointProcAnimFn;

void SetProceduralAnimationFn(void *function);

i32 StateAnimEvaluate(StateAnim *state, u8 *index, u8 *value, f32 frame);

i32 StateAnimEvaluate2(StateAnim *state, u8 *index, char *value, f32 frame);

void NuSpecialSetInstAnimTime(nuhspecial_s *special, f32 frame);

StateAnim *StateAnimFixPtrs(StateAnim *state, isize delta);

} // extern "C"

void SetAnimFrame(nuhspecial_s *special, float frame);

i32 GetAnimDirection(nuinstanim_s *animation);

i32 FindTexAnimFromMtl(nugscn_s *scene, numtl_s *material);


void InitTexAnimScripts(char **names);

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

void EvalAnim(nuhspecial_s *special, float frame, numtx_s *matrix, i32 include_instance_translation);

void EvalAnim2(nuhspecial_s *special, float frame);

void GameAnimSys_AllocateLevelProgressData(variptr_u *buf, variptr_u *buf_end, i32 capacity, i32 level_count);

u8 *GameAnimSys_GetProgressData(i32 index);

void GameAnimSys_StoreProgress(GAMEANIMSYS_s *system, i32 index);

void GameAnimSys_ReStoreProgress(GAMEANIMSYS_s *system, i32 index);

GAMEANIMSYS_s *GameAnimSys_Create(variptr_u *buf, variptr_u *buf_end);

// ==== src/legoapi/world/levels/episodeIII.cpp: declarations ====
#include <string.h>
#include <stdio.h>
#include "decomp.h"
#include "legoapi/world/level.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/gizmos/transport/tubes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "gameapi/ai/aisys/aisys.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numusic/numusic.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/chris.h"
#include "legoapi/render/fx.h"

extern i32 LevFlag[4];

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
}
#include "legoapi/render/core/render.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/audio/sfx.h"
#include "nu2api/nu3d/nutex.h"

i32 Players_AveragePos(NUVEC *, SOCKPOSITION_s *);
extern i16 id_PALPATINE;
extern i16 id_WOOKIEE;

extern PART_s * boulder_part[2];
extern i32 boulder_blowup_type;

void Boulder_Move(PART_s *, f32);
void Boulder_Kill(PART_s *, i32);
#include "legoapi/world/levels/episodeIII_types.h"
DECOMP_ASSERT(sizeof(VADERA_s) == 0x30, "VADERA_s size");
DECOMP_ASSERT(offsetof(VADERA_s, big_jump_locator) == 0x24, "VADERA_s locator offset");


static void VaderA_StartCollapseStage(WORLDINFO_s *world);


DECOMP_ASSERT(offsetof(CRUISERC_s, count_dooku) == 4, "CRUISERC_s Dooku offset");

extern "C" {
extern CRUISERCNETPACKET_s * crusiserc_netpacket;
}

DECOMP_ASSERT(sizeof(CRUISERDNETPACKET_s) == 12, "Cruiser D packet size");

extern "C" {
extern CRUISERDNETPACKET_s * cruiserd_netpacket;
    extern i32 CruiserD_LiftChase;
}

// Episode 3 level handlers, in the game's Episode_III progression:
// dogfight / cruiser / grievous / kashyyyk / temple / vader / a-new-hope.

// ===========================================================================
// Dogfight (Dogfight_A)
// ===========================================================================

extern AREADATA *DOGFIGHT_ADATA;
void DogFightARestart();

extern speedup_s DogFightSpeedList[9];

// ===========================================================================
// Cruiser (Cruiser_A / Cruiser_C / Cruiser_D)
// ===========================================================================

void CruiserAInit(WORLDINFO_s *world);

void CruiserAUpdate(WORLDINFO_s *);

void CruiserCReset(WORLDINFO_s *);

void CruiserCPanel(WORLDINFO_s *);

void CruiserCUpdate(WORLDINFO_s *);

void CruiserDInit(WORLDINFO_s *world);

void CruiserDUpdate(WORLDINFO_s *);

void CruiserDReset(WORLDINFO_s *);

// ===========================================================================
// Grievous (Grievous_A)
// ===========================================================================

void GrievousA_Init(WORLDINFO_s *world);

void GrievousA_Reset(WORLDINFO_s *);

void GrievousA_Update(WORLDINFO_s *world);

// ===========================================================================
// Kashyyyk (Kashyyyk_A / Kashyyyk_B / Kashyyyk_C / Kashyyyk_D)
// ===========================================================================

void KashyyykA_Init(WORLDINFO_s *world);

void KashyyykB_Init(WORLDINFO_s *);

void KashyyykC_Init(WORLDINFO_s *world);

void KashyyykD_Init(WORLDINFO_s *);

void KashyyykA_Panel(WORLDINFO_s *);

void KashyyykA_Reset(WORLDINFO_s *);

void KashyyykB_Reset(WORLDINFO_s *);

void KashyyykD_Reset(WORLDINFO_s *world);

i32 AnakinGreenSabre(GameObject_s *obj);

void KashyyykA_Update(WORLDINFO_s *);

void KashyyykB_Update(WORLDINFO_s *);

void KashyyykC_Update(WORLDINFO_s *);

void KashyyykD_Update(WORLDINFO_s *);

// ===========================================================================
// Temple (Temple_A / Temple_C)
// ===========================================================================

void TempleA_Init(WORLDINFO_s *world);

void TempleC_Init(WORLDINFO_s *world);

void TempleC_AlwaysUpdate(WORLDINFO_s *);

// ===========================================================================
// Vader (Vader_A / Vader_B / Vader_C)
// ===========================================================================

extern void * vadera_netpacket;



void GameCameraMakeMiniCut2(NUVEC *, NUVEC *, i32, f32, f32, f32, f32, i32, i32, i32);
void TickTockSfx();
int LoseCoins(GameObject_s *, i32);

void VaderA_Init(WORLDINFO_s *world);

void VaderB_Init(WORLDINFO_s *);

extern i32 Vader_ObiWanKilledAnakin;
extern void * vaderc_netpacket;

void VaderC_Init(WORLDINFO_s *world);

void VaderA_Reset(WORLDINFO_s *world);

void VaderB_Reset(WORLDINFO_s *);

void VaderC_Reset(WORLDINFO_s *);

void VaderA_Update(WORLDINFO_s *);

void VaderB_Update(WORLDINFO_s *);

void VaderC_Update(WORLDINFO_s *world);

void VaderA_DrawPanel(WORLDINFO_s *);

void VaderB_DrawPanel(WORLDINFO_s *);

void VaderC_DrawPanel(WORLDINFO_s *);

void VaderA_GoneThroughDoor(WORLDINFO_s *world, DOOR_s *door);


// ===========================================================================
// A New Hope (ANewHope_A)
// ===========================================================================

void ANewHopeA_Init(WORLDINFO_s *world);

// ==== src/legoapi/render/light/shadow.cpp: declarations ====
#include "legoapi/render/light/shadow.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern i32 VehicleArea;

extern "C" void NuRndrShadowDirCol(const NUVEC *direction, u32 colour, f32 near_distance, f32 far_distance);

f32 BlobShadowFade(NUVEC *position, f32 fade_start, f32 fade_end, f32 alpha);

void SetShadowLight();

void Shadow_SetMode();

void InitShadowLights();

void RenderShadowLights(i32);

void ConfigureComplexShadow(GameObject_s *object);

void ResetShadowMapRendering();

void EnableShadowMapRendering(i32);

void ResetShadowMapRenderingFn();

void EnableShadowMapRenderingFn();

// ==== src/legoapi/core/input/gamepads.cpp: declarations ====
#include "decomp.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/misc/androidbatman.h"
#include "gameapi/gui/apimenu.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/collect/spacelevel.h"
#include "nu2api/numusic/numusic.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/nupad.h"

#include <string.h>

extern "C" void NuSound3AddRumble(nupad_s *, f32, i32, i32, f32);
extern "C" void BackupMenu(void);
extern GAMECAMERA_s *GameCam;
extern WORLDINFO_s *WORLD;
extern i32 (*GamePads_IgnoreInputFn)(void);
extern NUPAD *pActivePad;
extern "C" {
    extern i32 Paused;
    extern i32 NewMode;
    extern "C++" FadeSystem FadeSys;
    extern i32 CutSceneWaiting;
}

// Original bss @0x127a500: 64 pads x 0x60 bytes.
extern GAMEPAD_s GamePad[64];
// Original bss @0x127a4e0.
extern i32 readpads_always;

extern u32 GAMEPAD_DRIGHT;
extern u32 GAMEPAD_DLEFT;
extern u32 GAMEPAD_DDOWN;
extern u32 GAMEPAD_DUP;
extern u32 GAMEPAD_TOGGLERIGHT;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_LIFT;
extern u32 GAMEPAD_TAG;
extern u32 GAMEPAD_SPECIAL;
extern u32 GAMEPAD_ACTION;
extern u32 GAMEPAD_JUMP;
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_R3;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;

// Two frames of direction history for each local player.  MovePlayer shifts
// these before accepting the new input; GamePad_Rotate then reports a stable
// angular velocity only when both consecutive turns agree.
extern f32 PadOldSpeed2[2];
extern f32 PadOldSpeed[2];
extern u16 PadOldAngle2[2];
extern u16 PadOldAngle[2];


void GamePads_Init();

f32 GamePad_Rotate(GameObject_s *object);

i32 GamePad_Waggle(GAMEPAD_s *pad);

GAMEPAD_s *GamePad_Allocate();

void GamePads_NetHost();

void GamePads_NetReset(i32);

u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad);

void GamePads_NetClient();

i32 GamePads_SkipMovie();

extern "C" {

bool TestForController(void);

} // extern "C"

void PadOutPause(i32 port, WORLDINFO_s *world);

void ResetRumble(RUMBLEPACKET *packet);

void UpdateRumble(RUMBLEPACKET *packet);

void NewBuzzFrames(nupad_s *pad, i32 frames, i32);

void TakeHitRumble(GameObject_s *object, float strength);

extern f32 SpaceRumbleTimer;
extern "C" f32 NuFsqrt(f32);
extern "C" f32 NuRandFloat();
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);
void GameCam_Judder(GAMECAMERA_s *, f32, i32, NUVEC *);
i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);

extern f32 MulDist;
extern f32 SpeedAdd;
extern f32 SpeedDist;
extern f32 MulAdd;

extern "C" void SetPreSeekStartPoint(f32 start_point);

void SpaceResetAudioPoint();

void SpaceRumbleProcess();

void NewRumbleAllPlayers(float strength, float duration, i32 frames, i32);

i32 ObjLookingWithLeftStick(GameObject_s *object);

void PerformPauseButtonStuff();

void VirtualControlDPad_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch);

void VirtualControlButton_OnDown_Callback(MechTouchUIElement &, TouchHolder &);

void VirtualControlButtonMover_OnDown_Callback(MechTouchUIElement &element, TouchHolder &touch);

void VirtualControlDPad_LockButton_OnClick_Callback(MechTouchUIElement &element, TouchHolder &);

i32 NoPad(i32 port, i32 require_game_input);

void NewBuzz(nupad_s *pad, float duration, i32);

i32 ReadPad(i32 port);

void ReadPads();

void NewRumble(nupad_s *pad, float strength, i32);

// ==== src/legoapi/misc/supportall.cpp: declarations ====
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
extern MemoryManager theMemoryManager;
#include "legoapi/misc/supportall_types.h"
extern BIGBUFFER_s BigBuffer;
extern void * theThingManager;

extern GameObject_s *alert_obj;
extern f32 alert_timer;
extern f32 LevelNameMul, LevelNameTime;
extern rtldata_s lev_rtldata;
extern "C" {
    extern f32 chattersfxwait, tieonsfxwait, tieoffsfxwait;
    extern i32 party_under_cover, nbaddies_can_see_players;
    extern i32 FalconDebKey[2];
    extern "C++" f32 TargetDist_Near2, TargetDist_Mid2;
    extern "C++" u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
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


DECOMP_ASSERT(sizeof(TIMINGBAR_LABEL) == 12, "Timing-bar label size");

extern i32 app_tbgameset;
extern i32 app_tbplayerset;
extern i32 app_tbaiset;
extern i32 app_tbdrawset;



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

void RndrTexQuad(f32 x, f32 y, f32 width, f32 height, i32 colour, numtl_s *material, i32 angle);

i32 SuperWeirdo(GameObject_s *object);

void bgProcClose();

extern "C" void edrtlCalculateBurnoutEx(burnset_s *set, NuBloomParameters *parameters, NUVEC *camera_position,
                                        f32 frame_time);

void BurnoutApply(i32 paused);

void bgprocFreeze();

void FindSlamOrigin(GameObject_s *, NUVEC *, NUVEC *);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);

void AddSlamDebris(GameObject_s *object);

void CloakMovement(GameObject_s *object);


void CheckResetBits();

void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material);

void bgProcAbortAll();

void bgprocUnFreeze();

extern AREADATA_s *PODRACE_ADATA;
extern AREADATA_s *BONUS_GUNSHIP_ADATA;
extern AREADATA_s *GUNSHIP_ADATA;
extern "C" i16 id_LANDSPEEDER;
void PodDust(WORLDINFO_s *, GameObject_s *);
extern "C" void AddVariableShotDebrisEffect(i32, NUVEC *, i32, i16, i16);

i32 bgprocIsFreezing();

void AddSurfaceDebris(GameObject_s *object);

extern "C" void DebFree(i32 *);
void DebFreeWithoutKey(debkeydatatype_s *key);

i32 CannotKill(GameObject_s *object);
extern "C" i32 DebrisPreCheckCollisions(NUVEC *position, f32 radius);
extern "C" i32 DebrisCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);
extern "C" i32 DebrisTorusCollisionCheckScaleY(NUVEC *position, f32 radius, f32 y_scale);

void DebrisKillPlayers();

i32 RndrUnfilledCircle(f32 x, f32 y, f32 radius, f32 border_width, f32 aspect, i32 colour, f32 progress, f32 z,
                       numtl_s *material);

void DebrisProcessSpheres(uv1deb *data, float time, debinftype *effect, debkeydatatype_s *key, i32 finite);

// Debug-capture output helpers consumed by NuDisplayListCaptureSortPriority.
// Transcribed from the original C-linkage symbols:
//   NuHtmlBegin    0x2d5ca0   NuHtmlFlush   0x2d5c30
//   NuHtmlWrite    0x2d5cd0   NuHtmlHeading1 0x2d5d40
extern "C" {
extern NUFILE hfh;
extern char unknown[8];
}

void NuHtmlFlush(i32 force);

extern "C" void NuHtmlBegin(void *file);

extern "C" void NuHtmlWrite(const char *text, ...);

extern "C" void NuHtmlHeading1(const char *fmt, ...);

extern "C" void NuHtmlBanner(void);

extern "C" void NuHtmlHeading2(const char *fmt, ...);

extern "C" void NuHtmlHeading3(const char *fmt, ...);

extern "C" void NuHtmlEnd(void);

void NuHtmlTitle(char *title);

extern "C" void NuHtmlBitmap(char *filename, i32 width, i32 height, char *caption, char *source);

void NuHtmlGraphArray(char **strings);

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

VARIPTR *DisplayListRenderBuffer();


void DebrisGetControlStackLock();



i32 SolveQuadratic(f32, f32, f32, f32 *, f32 *);
void DebrisProcessControlChunks(i32 panel_time);

extern "C" {
    extern i32 EDPP_MAX_TYPES;
    void edppDeleteEffect(i32);
}

void DebrisCleanUpDmaDebTypeTables();

void xxxNuDisplayListUpdateSpecial(nuhspecial_s *special);

void DebrisReleaseControlStackLock();

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern f32 globaltime;
    extern NUVEC debris_collide_pt;
}

i32 DebrisSingleCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags);

i32 DebrisSingleTorusCollisionCheckScaleYFlag(i32 key_index, NUVEC *position, f32 radius, f32 y_scale, u8 flags);

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

























// ==== functions in original address order ====

static void DrawWeapon_SetSabreObjects(GameObject_s *object, i32 red, i32 green, i32 blue, i32 purple, i32 *models,
                                       i32 *hilt) {
    if (red || green || blue || purple) {
        if (object->id == id_DARTHMAUL) {
            models[0] = *hilt = 0x12;
        } else if (object->id == id_COUNTDOOKU && WORLD->lev_objs[0x13].active) {
            models[0] = *hilt = 0x13;
        } else {
            models[0] = *hilt = 0x11;
        }
    }
    if (red) {
        if (object->id == id_DARTHMAUL) {
            if (object->apiobj.field_0x287 != 0)
                return;
            models[1] = 0x6d;
            models[2] = 0x6e;
            if (object->field_0xe22 & 8)
                models[3] = 0x6e;
        } else {
            if (object->apiobj.field_0x287 != 0)
                return;
            models[1] = 0x65;
            models[2] = 0x66;
            if (object->field_0xe22 & 8)
                models[3] = 0x66;
        }
        return;
    }

    if (green) {
        if (object->apiobj.field_0x287 != 0)
            return;
        models[1] = 0x67;
        models[2] = 0x68;
        if (object->field_0xe22 & 8)
            models[3] = 0x68;
        return;
    }

    if (blue) {
        if (object->apiobj.field_0x287 == 0) {
            models[1] = 0x69;
            models[2] = 0x6a;
            if (object->field_0xe22 & 8) {
                models[3] = 0x6a;
            }
        }
    } else if (purple && object->apiobj.field_0x287 == 0) {
        models[1] = 0x6b;
        models[2] = 0x6c;
        if (object->field_0xe22 & 8) {
            models[3] = 0x6c;
        }
    }
}

static void SetLayers_BOB(GameObject_s *object) {
    i32 head_layer[6] = {1, 2, 3, 21, 22, 23};
    i32 body_layer[8] = {5, 6, 7, 8, 9, 10, 16, 17};
    i32 legs_layer[3] = {11, 12, 15};
    i32 arms_layer[2] = {13, 14};
    i32 hands_layer[2] = {19, 20};
    object->random_layer_variant = qrand() <= 0x7fff;
    qrand();
    object->field_0x1054 = 1;
    object->field_0x1054 |= 1u << head_layer[qrand() / 0x2aab];
    object->field_0x1054 |= 1u << body_layer[qrand() / 0x2000];
    object->field_0x1054 |= 1u << legs_layer[qrand() / 0x5556];
    object->field_0x1054 |= 1u << arms_layer[qrand() / 0x8000];
    object->field_0x1054 |= 1u << hands_layer[qrand() / 0x8000];
}

void LoadSingleCharacter(bgprocinfo_s *) {
    APICHARACTERMODELLIST_s list[2] = {
        {static_cast<i16>(waiting_for_character), 1},
        {-1, 0},
    };

    apiloadcharactermodels_append = 1;
    apiloadcharactermodels_nopakfile = CHARPAK == 0;
    APILoadCharacterModels(list, 0, &characterbuffer_ptr, characterbuffer_end, 1);
    IconScenes_Load(list, 0, &characterbuffer_ptr, &characterbuffer_end);

    NUGSCN *icon_scene = IconScene_FindById(list[0].model_id);
    if (icon_scene != NULL && LEVELOBJECTCOUNT > 0) {
        for (i32 i = 0; i < LEVELOBJECTCOUNT; ++i) {
            if (ObjTab[i].kind != 3) {
                continue;
            }

            if (WORLD->lev_objs[i].active != 0) {
                continue;
            }

            if (NuSpecialFind(icon_scene, &WORLD->lev_objs[i].special, ObjTab[i].name, 1) != 0) {
                WORLD->lev_objs[i].active = 1;
            }
        }
    }

    Customiser_SaveModelTextureIDs(CharacterCustomiser, APICharacterLoaded(list[0].model_id));
    hub_character_ready = waiting_for_character;
    waiting_for_character = -1;
    g_loadingCharacterInHub = 0;
}

static void SetLayers_MOSEISLEYCITIZEN(u32 *layers) {
    static const u8 hat_layers[5] = {0, 0, 0, 7, 14};
    static const u8 head_layers[5] = {20, 20, 8, 8, 4};
    static const u8 body_layers[3] = {3, 9, 19};
    static const u8 arm_layers[3] = {1, 12, 15};
    static const u8 hand_layers[3] = {2, 13, 16};
    static const u8 waist_layers[3] = {6, 10, 17};
    static const u8 leg_layers[3] = {5, 11, 18};

    *layers = 0;
    *layers |= LayerBit(hat_layers[qrand() / 0x3334]);
    *layers |= LayerBit(head_layers[qrand() / 0x3334]);
    *layers |= LayerBit(body_layers[qrand() / 0x5556]);
    *layers |= LayerBit(arm_layers[qrand() / 0x5556]);
    *layers |= LayerBit(hand_layers[qrand() / 0x5556]);
    *layers |= LayerBit(waist_layers[qrand() / 0x5556]);
    *layers |= LayerBit(leg_layers[qrand() / 0x5556]);
}

static i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y) {
    const u32 flags = TerSurface[surface].flags;
    if ((flags & 1) != 0 ||
        ((flags & 0x4000) != 0 && (object->apiobj.character_data->model_flags & 0x10) == 0 &&
         (object->apiobj.character_data->game_character->flags_090 & 0x8000) == 0) ||
        ((flags & 0x8000) != 0 && (object->apiobj.field_0x27c == -1 || *normal_y > 0.0f))) {
        InstantKillParts(object, 1, 0.0f);
        if (object->apiobj.player_controlled && BonusWinner == -1) {
            const i32 coins = LoseCoins(object, 1);
            AddPickups(coins, 0, 0, 0, &object->apiobj.collision_position, NULL, 2.0f, -1, 1.0f, 2000000.0f, object, 1,
                       0, false);
        }
        KillPlayer(object, 2, 1, NULL);
        return 1;
    }
    return 0;
}

void FixUpCharacters(CHARFIXUP *fixup) {
    for (i32 i = 0; i < CHARCOUNT; ++i) {
        GCDataList[i] = GCDATA_DEFAULT;

        CHARACTERDATA &character = CDataList[i];
        if (character.field0_0x0 == -1) {
            character.field0_0x0 = tUNKNOWN;
        }
        if (character.move_fn == NULL) {
            character.move_fn = Move_CHARACTER;
        }
        if (character.animate_fn == NULL) {
            character.animate_fn = Animate_CHARACTER;
        }
    }

    if (fixup != NULL) {
        while (fixup->name != NULL) {
            if (fixup->id != NULL) {
                *fixup->id = (i16)CharIDFromName(fixup->name);
            }
            ++fixup;
        }
    }
}

void ExtraCharacterFixUpAfterConfig() {
    SetMoveAndAnimateFunctions(8, 8, 0, 0, -1, reinterpret_cast<void *>(Move_JEDI),
                               reinterpret_cast<void *>(Animate_JEDI), NULL);
    SetMoveAndAnimateFunctions(0x1000010, 0x10, 0, 0, -1, reinterpret_cast<void *>(Move_DROIDGENERIC), NULL, NULL);
    SetMoveAndAnimateFunctions(0x30, 0x30, 0, 0, -1, reinterpret_cast<void *>(Move_DROIDGENERIC),
                               reinterpret_cast<void *>(Animate_PROTOCOL), NULL);
    SetMoveAndAnimateFunctions(0x50, 0x50, 0, 0, -1, reinterpret_cast<void *>(Move_DROIDGENERIC),
                               reinterpret_cast<void *>(Animate_ASTROMECH),
                               reinterpret_cast<void *>(PostAnimate_ASTROMECH));
    SetMoveAndAnimateFunctions(0, 0, 0x800, 0x800, -1, reinterpret_cast<void *>(Move_CANNON),
                               reinterpret_cast<void *>(Animate_CANNON), NULL);
    SetMoveAndAnimateFunctions(0x2000, 0x2000, 0, 0, -1, reinterpret_cast<void *>(Move_VEHICLE),
                               reinterpret_cast<void *>(Animate_VEHICLE), NULL);
    SetMoveAndAnimateFunctions(0x40000000, 0x40000000, 0, 0, -1, reinterpret_cast<void *>(Move_BEAST),
                               reinterpret_cast<void *>(Animate_BEAST), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 4, reinterpret_cast<void *>(Move_DROIDGENERIC),
                               reinterpret_cast<void *>(Animate_BATTLEDROID), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 17, reinterpret_cast<void *>(Move_HOVERDROID),
                               reinterpret_cast<void *>(Animate_HOVERDROID), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 15, reinterpret_cast<void *>(Move_WALKER),
                               reinterpret_cast<void *>(Animate_WALKER), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 16, reinterpret_cast<void *>(Move_ATAT),
                               reinterpret_cast<void *>(Animate_ATAT), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 18, reinterpret_cast<void *>(Move_CRITTER),
                               reinterpret_cast<void *>(Animate_CRITTER), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 18, reinterpret_cast<void *>(Move_CRITTER),
                               reinterpret_cast<void *>(Animate_CRITTER), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 20, reinterpret_cast<void *>(Move_POD),
                               reinterpret_cast<void *>(Animate_POD), NULL);
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 2, NULL, NULL, reinterpret_cast<void *>(PostAnimate_FETT));
    SetMoveAndAnimateFunctions(0, 0, 0, 0, 0, reinterpret_cast<void *>(Move_WEIRDO),
                               reinterpret_cast<void *>(Animate_WEIRDO), NULL);
    for (i32 i = 0; i < CHARCOUNT; ++i) {
        if ((GCDataList[i].flags_094[3] & 0x10) != 0) {
            CDataList[i].move_fn = Move_GEONOSIAN;
            CDataList[i].animate_fn = Animate_GEONOSIAN;
        }
    }
    if (id_DROIDEKA != -1) {
        CDataList[id_DROIDEKA].move_fn = Move_DROIDEKA;
        CDataList[id_DROIDEKA].animate_fn = Animate_DROIDEKA;
    }
    if (id_MINIDROIDEKA != -1) {
        CDataList[id_MINIDROIDEKA].move_fn = Move_DROIDEKA;
        CDataList[id_MINIDROIDEKA].animate_fn = Animate_DROIDEKA;
    }
    if (id_SUPERBATTLEDROID != -1) {
        CDataList[id_SUPERBATTLEDROID].move_fn = Move_SUPERBATTLEDROID;
        CDataList[id_SUPERBATTLEDROID].animate_fn = Animate_SUPERBATTLEDROID;
    }
    if (id_BARMAN != -1) {
        CDataList[id_BARMAN].move_fn = Move_BARMAN;
        CDataList[id_BARMAN].animate_fn = Animate_BARMAN;
    }
    if (id_JAWA != -1) {
        CDataList[id_JAWA].move_fn = Move_JAWA;
    }
    if (id_UGNAUGHT != -1) {
        CDataList[id_UGNAUGHT].move_fn = Move_JAWA;
    }
    if (id_DRAGBOMB != -1) {
        CDataList[id_DRAGBOMB].move_fn = Move_DRAGBOMB;
    }
    if (id_REPUBLICGUNSHIP != -1) {
        CDataList[id_REPUBLICGUNSHIP].move_fn = Move_REPUBLICGUNSHIP;
        CDataList[id_REPUBLICGUNSHIP].animate_fn = Animate_REPUBLICGUNSHIP;
    }
    if (id_REPUBLICGUNSHIP_GREEN != -1) {
        CDataList[id_REPUBLICGUNSHIP_GREEN].move_fn = Move_REPUBLICGUNSHIP;
        CDataList[id_REPUBLICGUNSHIP_GREEN].animate_fn = Animate_REPUBLICGUNSHIP;
    }
    if (id_SPEEDERBIKE != -1) {
        CDataList[id_SPEEDERBIKE].move_fn = Move_SPEEDERBIKE;
        CDataList[id_SPEEDERBIKE].animate_fn = Animate_SPEEDERBIKE;
    }
    if (id_PROBEDROID != -1) {
        CDataList[id_PROBEDROID].animate_fn = Animate_DEFAULT;
    }
    if (id_BODYGUARD != -1) {
        CDataList[id_BODYGUARD].move_fn = Move_JEDI;
        CDataList[id_BODYGUARD].animate_fn = Animate_JEDI;
    }
    if (id_IMPERIALGUARD != -1) {
        CDataList[id_IMPERIALGUARD].move_fn = Move_JEDI;
        CDataList[id_IMPERIALGUARD].animate_fn = Animate_JEDI;
    }
}

void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *) {
    packet->field_0x664 = 0;
    packet->delayed_turn_timer = 0.0f;
    packet->context_state_flags &= static_cast<u8>(~0x01);
    packet->movement_lean_angle = 0;
    packet->secondary_lean_angle = 0;
    packet->tertiary_lean_angle = 0;

    Player_ResetContexts(packet);

    GAMEPAD_s *gamepad = packet->gamepad;
    gamepad->rumble_packet.field_0x00 = 0;
    gamepad->allocated_5a &= static_cast<u8>(~GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT);
    gamepad->buttons_held = 0;
    gamepad->buttons_pressed = 0;
    gamepad->buttons_released = 0;
    gamepad->left_directions = 0;
    gamepad->previous_left_directions = 0;
    gamepad->right_directions = 0;
    gamepad->previous_right_directions = 0;
    gamepad->unknown_20 = 0;
    gamepad->rumble_packet.rumble_amount = 0.0f;
    gamepad->rumble_packet.field_0x08 = 0;
    gamepad->rumble_packet.rumble_time = 0.0f;
    gamepad->allocated_5a &= static_cast<u8>(~(0x04 | 0x10));
    packet->secondary_flags &= static_cast<u8>(~0x04);
    packet->input_state = 0;
    gamepad->operator_data = NULL;

    CharShadows_Reset(packet);
    packet->field_0x6c8 = 0;
    packet->field_0x6e8 = 0;
    packet->field_0x614 = 0;
    packet->field_0x618 = 0;
    packet->field_0x61c = 0;
    packet->field_0x620 = 0;
    packet->field_0x624 = 0;
    packet->field_0x628 = 0;
    packet->field_0x62c = 0;
    packet->field_0x630 = 0;

    ResetForceGlow(packet);
    packet->ground_height = 2000000.0f;
    packet->field_0x6fc = 0;
    packet->field_0x73c = 0;
    packet->movement_flags &= static_cast<u8>(~0x01);
    packet->movement_angle_0 = -1;
    packet->force_glow_index = -1;
    packet->field_0x718 = 0;
    packet->movement_angle_1 = qrand() > 0x7fff ? 0x4f : 0x26;
    packet->random_state[0] = static_cast<u8>(qrand() >> 8);
    packet->random_state[1] = static_cast<u8>(qrand() >> 8);
    packet->random_state[2] = static_cast<u8>(qrand() >> 8);
    packet->random_state[3] = static_cast<u8>(qrand() >> 8);
    packet->render_flags &= static_cast<u8>(~0x01);
    packet->field_0x71c = 0;
    packet->field_0x720 = 0;
    packet->field_0x728 = 0;
    packet->field_0x69c = 0;
    packet->field_0x690 = 0;
    packet->field_0x694 = 0;
    packet->field_0x688 = 0;
    packet->field_0x794 = 0;
    packet->field_0x771 &= static_cast<u8>(~0x0c);
    packet->field_0x604 = NULL;
    packet->surface_type = -1;
    packet->reset_up_direction = v010;
}

void SetFlicker(GameObject_s *object, float duration) {
    object->field_0x1024 = duration;
    object->flicker_flags &= ~7;
}

void ResetPlayerAI(GameObject_s *object) {
    object->ai.movement_flags &= 0x9f;
    object->field_0xefe &= 0xdf;
    object->ai.field_0x1e5 &= 0xfd;
    object->ai.intersection_connection = NULL;
    object->ai.intersection_target_connection = NULL;
    object->ai.animation_override_from = -1;
    object->ai.animation_override_to = -1;
    object->ai.field_0x180 = NULL;
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->field_0x1098 = 0;
    object->field_0x1094 = 0;
    object->field_0xf08 = 0;
    object->field_0xf0c = 0;
    object->field_0xf10 = 0;
    object->ai.frame_state = 0;
    object->field_0xf14 = 0;
    object->field_0x109c = 0;
    memset(&object->ai.path_info, 0, sizeof(object->ai.path_info));
    object->ai.current_route = 0xff;
    object->ai.next_route = 0;
    object->ai.inside_path_node = -1;
    AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
}

void InitPlayerAI(GameObject_s *object) {
    StarWars_AutoSetAICapabilities(object);
    u8 *b = reinterpret_cast<u8 *>(object);
    i32 can_pull_levers = CanPullLevers(object->id);
    b[0xefe] = (b[0xefe] & 0x7f) | (can_pull_levers << 7);
    b[0x4a6] &= 0xfe;
    object->ai.character_type_mask_low = 0;
    object->ai.character_type_mask_high = 0;
    if (FreePlay && !(object->apiobj.field_0x1f4 & 0x400)) {
        object->ai.character_type_mask = _0xffffffffffffffff;
    } else if (SpecialRouteCharacterTypeIDFn) {
        u8 *row = *reinterpret_cast<u8 **>(b + 0xcac);
        char *name = row ? *reinterpret_cast<char **>(row + 4) : object->apiobj.character_data->file;
        u8 type = SpecialRouteCharacterTypeIDFn(name);
        if (type != 0xff) {
            u64 mask = type <= 63 ? u64(1) << type : _0xffffffffffffffff;
            object->ai.character_type_mask_high = static_cast<u32>(mask >> 32);
            object->ai.character_type_mask_low = static_cast<u32>(mask);
        }
    }
    b[0x370] = 0;
    PlayerAIFlagBits *flags_ef8 = PlayerAIFlags(b, 0xef8);
    PlayerAIFlagBits *flags_ef9 = PlayerAIFlags(b, 0xef9);
    PlayerAIFlagBits *flags_efa = PlayerAIFlags(b, 0xefa);
    PlayerAIFlagBits *flags_efb = PlayerAIFlags(b, 0xefb);
    PlayerAIFlagBits *flags_efc = PlayerAIFlags(b, 0xefc);
    PlayerAIFlagBits *flags_efd = PlayerAIFlags(b, 0xefd);
    PlayerAIFlagBits *flags_eff = PlayerAIFlags(b, 0xeff);
    PlayerAIFlagBits *flags_f00 = PlayerAIFlags(b, 0xf00);
    PlayerAIFlagBits *flags_f01 = PlayerAIFlags(b, 0xf01);
    PlayerAIFlagBits *flags_f02 = PlayerAIFlags(b, 0xf02);
    PlayerAIFlagBits *flags_f03 = PlayerAIFlags(b, 0xf03);
    PlayerAIFlagBits *flags_f04 = PlayerAIFlags(b, 0xf04);
    PlayerAIFlagBits *flags_4a5 = PlayerAIFlags(b, 0x4a5);
    PlayerAIFlagBits *flags_4a7 = PlayerAIFlags(b, 0x4a7);
    PlayerAIFlagBits *flags_7b5 = PlayerAIFlags(b, 0x7b5);
    flags_ef8->bit1 = 1;
    object->doomed_escape_locator = NULL;
    flags_ef8->bit2 = (object->apiobj.character_data->model_flags >> 7) & 1;
    flags_ef8->bit3 = 1;
    flags_ef8->bit4 = 0;
    flags_ef8->bit6 = 0;
    flags_ef8->bit7 = 0;
    flags_eff->bit3 = 0;
    flags_eff->bit4 = 0;
    flags_eff->bit5 = 0;
    flags_efc->bit0 = 0;
    flags_efc->bit1 = 0;
    flags_efc->bit2 = 0;
    flags_efc->bit3 = 0;
    flags_efc->bit4 = 0;
    flags_efc->bit5 = 0;
    flags_efd->bit0 = 0;
    flags_efd->bit3 = 0;
    flags_efd->bit4 = 0;
    flags_ef9->bit0 = 0;
    flags_ef9->bit1 = 0;
    flags_f01->bit1 = 0;
    flags_f01->bit2 = 0;
    flags_f01->bit4 = 0;
    flags_efb->bit1 = 0;
    flags_efb->bit3 = 0;
    flags_efb->bit4 = 0;
    flags_efb->bit6 = 0;
    flags_efb->bit7 = 0;
    flags_f00->bit0 = 0;
    flags_f00->bit1 = 0;
    flags_f00->bit5 = 0;
    flags_f00->bit7 = 0;
    flags_f03->bit1 = 0;
    flags_f03->bit2 = 0;
    flags_f03->bit3 = 0;
    flags_f03->bit6 = 0;
    flags_f03->bit7 = 0;
    object->run_speed_override = 1000000000.0f;
    object->walk_speed_override = 1000000000.0f;
    // The original scale-override sentinel; zero would hide the character.
    object->hover_height_override = 1000000000.0f;
    object->field_0x1038 = 1000000000.0f;
    flags_f04->bit0 = 0;
    flags_f04->bit1 = 0;
    flags_f02->bit2 = ((object->apiobj.field_0x1f4 >> 10) & 1) == 0;
    flags_f02->bit3 = 0;
    *reinterpret_cast<u32 *>(b + 0x1058) = 0;
    *reinterpret_cast<f32 *>(b + 0x4ac) = DEFAULT_MOVE_RANGE;
    flags_4a7->bit2 = DEFAULT_MOVE_RANGE > 0.0f;
    flags_4a7->bit3 = 0;
    flags_4a7->bit4 = 0;
    flags_f02->bit5 = 0;
    flags_f02->bit7 = 0;
    flags_ef8->bit0 = 0;
    flags_f03->bit0 = 0;
    flags_efa->bit2 = 0;
    flags_efa->bit3 = 0;
    flags_efa->bit4 = (object->apiobj.character_data->model_flags >> 27) & 1;
    flags_efa->bit5 = 0;
    flags_7b5->bit1 = 0;
    *reinterpret_cast<u32 *>(b + 0xecc) = 0;
    *reinterpret_cast<u32 *>(b + 0x394) = 0;
    *reinterpret_cast<u32 *>(b + 0x39c) = 0;
    flags_efb->bit0 = 0;
    flags_efb->bit2 = 0;
    flags_eff->bit0 = 0;
    *reinterpret_cast<f32 *>(b + 0xed4) = 1.0f;
    *reinterpret_cast<f32 *>(b + 0xed0) = 0.0f;
    *reinterpret_cast<f32 *>(b + 0x3a0) = 1000000000.0f;
    flags_4a5->bit4 = 0;
    flags_4a5->bit6 = 0;
    *reinterpret_cast<u32 *>(b + 0x3a4) = 0;
    *reinterpret_cast<u32 *>(b + 0x3ac) = 0;
    *reinterpret_cast<u32 *>(b + 0x2a0) = 0;
    *reinterpret_cast<u32 *>(b + 0x2a4) = 0;
    flags_ef9->bit3 = 0;
    *reinterpret_cast<u32 *>(b + 0xebc) = 0;
    *reinterpret_cast<f32 *>(b + 0x3b0) = 1000000000.0f;
    *reinterpret_cast<u32 *>(b + 0xec0) = 0;
    *reinterpret_cast<u32 *>(b + 0x10b0) = 0;
    *reinterpret_cast<u32 *>(b + 0x10b4) = 0;
    *reinterpret_cast<u32 *>(b + 0xec4) = 0;
    *reinterpret_cast<f32 *>(b + 0x448) = 0.0f;
    *reinterpret_cast<u32 *>(b + 0xec8) = 0;
    *reinterpret_cast<f32 *>(b + 0xed8) = 0.0f;
    memset(b + 0xf48, 0, 0x68);
    b[0x1089] = 0;
    *reinterpret_cast<u32 *>(b + 0xfe4) = 0;
    *reinterpret_cast<u32 *>(b + 0x3b4) = 0;
    *reinterpret_cast<u32 *>(b + 0x294) = 0;
    *reinterpret_cast<u32 *>(b + 0x298) = 0;
    *reinterpret_cast<u32 *>(b + 0x29c) = 0;
    bool special = (object->apiobj.character_data->model_flags & 0x4000) &&
                   (b[0x27c] == 0xff || *reinterpret_cast<void **>(b + 0xcc0));
    b[0x1f8] = (b[0x1f8] & 0xfd) | (special << 1);
    u8 *config = reinterpret_cast<u8 *>(object->apiobj.character_data->player_config);
    b[0x1f9] &= 0xf7;
    b[0x1fa] &= 0xe7;
    flags_f00->bit2 = 0;
    b[0xefe] &= 0xbf;
    flags_eff->bit2 = 0;
    flags_eff->bit7 = (*reinterpret_cast<u32 *>(config + 0x90) >> 13) & 1;
    b[0x108f] = 0;
    *reinterpret_cast<u32 *>(b + 0x10c0) = 0;
    *reinterpret_cast<u32 *>(b + 0x10d8) = 0;
    *reinterpret_cast<f32 *>(b + 0x1040) = 1.0f;
    *reinterpret_cast<u32 *>(b + 0x1044) = 0;
    flags_f04->bit2 = 0;
    flags_f04->bit3 = 0;
    flags_f04->bit7 = PlayerAIFlags(reinterpret_cast<u8 *>(object->apiobj.character_data->player_config), 0x98)->bit7;
    ResetPlayerAI(object);
}

void DeactivateGameObject(GameObject_s *object) {
    if (object == NULL) {
        return;
    }

    if (object->field_0xcc0 != NULL) {
        if ((object->apiobj.field_0x1f4 & 0x4000) != 0) {
            ReleaseTakeOver(object, 1);
        } else {
            KillGameObject(object->field_0xcc0, 4, 0);
            object->apiobj.character = 0;
        }
    }
    object->apiobj.character = 0;

    if (object->ai.field_0x134 != 0xff &&
        AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&object->ai),
                                         const_cast<char *>("InActive")) != 0) {
        object->ai.reset_mode = 0;
        WORLD->ai_sys->creatures[object->ai.field_0x134].activate_type = 2;
    } else {
        object->ai.reset_mode = 4;
    }
}

void DeactivateCharacter(char *name) {
    if (Mission_Active(NULL) != NULL || name == NULL)
        return;
    GameObject_s *object = GetNamedGameObject(WORLD->ai_sys, name);
    if (object != NULL && (FreePlay == 0 || (object->apiobj.field_0x1f4 & 0x400) != 0)) {
        DeactivateGameObject(object);
    }
}

void SetGameObjectCharacterData(GameObject_s *obj) {
    CHARACTERDATA *data = obj->apiobj.character_data;
    obj->apiobj.field_0xa8 = data->field17_0x3c;
    obj->field_0x1008 = data->field14_0x30;
    obj->field_0xffc = data->field15_0x34;
    obj->field_0x1000 = data->field16_0x38;
    ScaleGameObject(obj);
}

void SetHitPoints(GameObject_s *obj, i32 hp) {
    obj->current_hp = (u8)hp;
    if ((i8)hp > (i32)(u32)obj->hitpoints) {
        obj->current_hp = obj->hitpoints;
    }
}

i32 MakePlayerList(i32 count) {
    i32 player_count = 0;

    for (i32 i = 0; i < count; i++) {
        GameObject_s *player = Player[i];
        if (player == NULL) {
            if (makeplayerlist_freeplay == 1 && player_count == 1 && WORLD != NULL &&
                WORLD->current_level == HUB_LDATA && HUB_LDATA != NewLData) {
                PlayerList[1] = FreePlayModelList[i].model_id;
                PlayerProgress[1].active = 0;
                player_count = 2;
            }
            continue;
        }

        if (makeplayerlist_freeplay == 1) {
            PlayerList[player_count] = FreePlayModelList[i].model_id;
        } else if (makeplayerlist_freeplay != 2) {
            PlayerList[player_count] = player->id;
        }

        PlayerProgress[player_count].active = player->apiobj.player_controlled;

        bool unavailable = false;
        if (player->field_0x7a5 != 6 && player->field_0xe32 != 2) {
            unavailable = true;
            if ((player->field_0xe22 & 1) == 0) {
                unavailable = player->field_0xe32 != 0;
            }
        }
        PlayerProgress[player_count].field_0x7 = unavailable;
        PlayerProgress[player_count].coins = player->coinpacket != NULL ? player->coinpacket->coins : 0;
        PlayerProgress[player_count].field_0xb = player->field_0xe31 == 1;

        if (player->apiobj.field_0x287 != 0) {
            PlayerProgress[player_count].hitpoints = player->hitpoints;
            PlayerProgress[player_count].field_0x9 = 4;
        } else {
            PlayerProgress[player_count].hitpoints = player->current_hp;
            PlayerProgress[player_count].field_0x9 = player->field_0xe38;
        }
        PlayerProgress[player_count].field_0xa = player->field_0x108e;
        PlayerProgress[player_count].field_0x4 = player->field_0x106e;
        PlayerProgress[player_count].field_0xc = player->field_0xdec;

        PlayerSuit[player_count] = player->suit;
        PlayerTorpedoCount[player_count] = player->torpedo != NULL ? player->torpedo->count : 0;
        player_count++;
    }

    for (i32 i = player_count; i < 8; i++) {
        PlayerList[i] = -1;
    }
    makeplayerlist_freeplay = 0;
    return player_count;
}

void Player_CopyEssentials(GameObject_s *source, GameObject_s *destination) {
    destination->pad_gamepad = source->pad_gamepad;
    destination->coinpacket = source->coinpacket;
    destination->gizforce_los_info = source->gizforce_los_info;
    destination->batarang = source->batarang;
    destination->torpedo = source->torpedo;
    destination->oldpos = source->oldpos;
    destination->hitpoints = source->hitpoints;
    destination->field_0xeb4 = source->field_0xeb4;
    destination->apiobj.field_0x1f4 = source->apiobj.field_0x1f4;
}

void ClearAICreatures() {
    GameObject_s *object = Obj;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i, ++object) {
        if ((object->apiobj.flags_low & 1) != 0 && (object->apiobj.field_0x1f4 & 0x400) != 0) {
            FreeTorpedoPacket(&object->torpedo);
            RemoveGameObject(object, 1);
        }
    }
}

void StoreProgressAICharacter(LEVEL_PROGRESS_s *progress) {
    if (progress == NULL)
        return;
    progress->disabled_ai_object_mask[0] = 0;
    progress->disabled_ai_object_mask[1] = 0;
    GameObject_s *object = Obj;
    i32 count = HIGHGAMEOBJECT;
    for (i32 i = 0; i < count; ++i, ++object) {
        if ((object->apiobj.flags_low & 1) != 0 && (object->apiobj.field_0x1f4 & 0x400) != 0 &&
            object->ai.reset_mode == 4) {
            u64 bit = (u64)1 << i;
            progress->disabled_ai_object_mask[0] |= (u32)bit;
            progress->disabled_ai_object_mask[1] |= (u32)(bit >> 32);
        }
    }
}

void AICreatureResumeScript(GameObject_s *object) {
    AISCRIPT *script = object->ai.script_process.base_script;
    if (script != NULL) {
        AIScriptProcessorInit(WORLD->ai_sys, &object->ai, &object->ai.script_process, NULL, NULL, NULL, 0, script,
                              script->base_state);
        object->ai.script_process.active_ref_count = 0;
    }
}

i32 GetDefaultIdle(GameObject_s *obj) {
    CHARACTERDATA *character = obj->apiobj.character_data;
    DefaultIdleCharacterData *game_character = static_cast<DefaultIdleCharacterData *>(character->field11_0x24);

    i32 animation;
    i32 table_offset;
    if (game_character->use_standard_idle != 0) {
        animation = 25;
        table_offset = 100;
    } else {
        const u32 alternate = character->model_flags & 0x80;
        table_offset = alternate != 0 ? 472 : 100;
        animation = alternate != 0 ? 118 : 25;
    }
    if (obj->batarang != NULL && *(reinterpret_cast<u8 *>(obj->batarang) + 0x7d) != 0) {
        return 151;
    }

    u8 *animation_table = reinterpret_cast<u8 *>(obj->apiobj.character_model->model_data_b);
    void *entry = *reinterpret_cast<void **>(animation_table + table_offset);
    if ((*reinterpret_cast<i32 *>(animation_table + 4) == 0 || (obj->field_0xe22 & 1) != 0 || obj->field_0xe32 == 1) &&
        entry != NULL) {
        return animation;
    }
    return 1;
}

void ResetCharacterIdle(GameObject_s *object, i32 mode, i32 animation) {
    // Animation 151 belongs to the held-batarang override and deliberately
    // leaves the ordinary idle scheduler untouched.
    if (animation == 151) {
        return;
    }

    object->idle_animation = static_cast<i16>(animation);
    object->idle_animation_time = 0.0f;

    CHARACTERMODEL_s *model = object->apiobj.character_model;
    CHARACTERANIM_s *animation_info;
    i32 repetitions;
    // The signed > 0 test (rather than != 0) keeps the retail branch layout.
    if (model->model_data_b[object->idle_animation] != NULL &&
        (animation_info = static_cast<CHARACTERANIM_s *>(model->model_data_a[object->idle_animation]),
         repetitions = static_cast<u8>(animation_info->minimum_repetitions)) > 0) {
        const i32 maximum = static_cast<u8>(animation_info->maximum_repetitions);
        if (repetitions < maximum) {
            repetitions += qrand() / (0xffff / (maximum - repetitions) + 1);
        }
        object->idle_animation_limit =
            AnimDuration(object->id, object->idle_animation, 0.0f, 0.0f, 0) * repetitions - animduration_blendouttime;
    } else {
        object->idle_animation_limit = static_cast<f32>(qrand()) * (1.0f / 65535.0f) * 7.0f + 8.0f;
    }

    if (mode > 0) {
        object->idle_total_time = 0.0f;
        if (mode != 1) {
            object->previous_idle_animation = -1;
        }
    }
}

static void NewCharacterIdle(GameObject_s *object, i32 default_idle) {
    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    const i32 alternate_idle = game_character->field275_0x116 == 0 && (character->model_flags & 0x80) != 0 ? 118 : 25;

    i32 candidates[233];
    i32 candidate_count = 0;
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    for (i32 animation = 0; animation < 233; ++animation) {
        if (model->model_data_b[animation] == NULL) {
            continue;
        }
        CHARACTERANIM_s *info = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
        if ((info->flags & 0x10) == 0) {
            continue;
        }
        if (__builtin_expect(alternate_idle == default_idle, 0)) {
            if ((info->flags & 0x800) != 0) {
                candidates[candidate_count++] = animation;
            }
        } else if ((info->flags & 0x800) == 0) {
            candidates[candidate_count++] = animation;
        }
    }

    if (__builtin_expect(candidate_count == 0, 0)) {
        ResetCharacterIdle(object, 0, default_idle);
        return;
    }

    if (candidate_count != 1) {
        do {
            object->idle_animation = static_cast<i16>(candidates[qrand() / (0xffff / candidate_count + 1)]);
        } while (object->idle_animation == object->previous_idle_animation);
    } else {
        object->idle_animation = static_cast<i16>(candidates[0]);
    }

    CHARACTERANIM_s *info =
        static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[object->idle_animation]);
    i32 repetitions = static_cast<u8>(info->minimum_repetitions);
    if (repetitions == 0) {
        repetitions = 1;
    } else {
        const i32 maximum = static_cast<u8>(info->maximum_repetitions);
        if (maximum != 0 && maximum > repetitions) {
            repetitions += qrand() / (0xffff / (maximum - repetitions) + 1);
        }
    }
    object->previous_idle_animation = object->idle_animation;
    if (repetitions > 1 &&
        (static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[object->idle_animation])->flags &
         2) == 0) {
        repetitions = 1;
    }

    object->idle_animation_time = 0.0f;
    const f32 duration = AnimDuration(object->id, object->idle_animation, 0.0f, 0.0f, 0);
    object->idle_animation_limit = duration * repetitions - animduration_blendouttime;
}

i32 InitCreature(GameObject_s *obj, i32 id, i32 param) {
    addcreature_override_id_check = 0;
    if (id < 0) {
        return 0;
    }
    if (id > 0x153 || apicharsys->playermodelids[id] == -1) {
        return 0;
    }

    CHARACTERDATA *character_data = &apicharsys->char_data[id];
    obj->apiobj.character_data = character_data;
    GAMECHARACTERDATA *game_character_data = static_cast<GAMECHARACTERDATA *>(character_data->field11_0x24);
    obj->field_0x1054 = game_character_data->layer_mask;
    obj->apiobj.field_0x1f4 = 0;
    obj->field_0x107c = -1;
    memset(obj->player_packet, 0, 0x798);
    obj->apiobj.field_0x27c = -1;

    const NUVEC start_position = *Player_StartPos(obj);
    if (param != 0) {
        obj->apiobj.field_0x1f4 |= 0x4002;
    } else {
        obj->apiobj.field_0x1f4 |= 2;
    }
    if ((WORLD->current_level->flags & 0x40000) == 0) {
        obj->apiobj.field_0x1f4 |= 0x40;
    }

    obj->pad_gamepad = GamePad_Allocate();
    obj->pad_gamepad->input_mode = 1;
    const u8 hitpoints = obj->apiobj.character_data->game_character->hitpoints;
    obj->hitpoints = hitpoints;
    obj->current_hp = hitpoints;
    ResetPlayerPacket(reinterpret_cast<PLAYERPACKET_s *>(obj->player_packet),
                      reinterpret_cast<CHARACTERDATA_s *>(obj->apiobj.character_data));

    obj->apiobj.movement_direction = v000;
    const u8 field_0xf5 = obj->apiobj.character_data->game_character->field_0xf5;
    obj->field_0xe38 = 4;
    obj->field_0xe37 = field_0xf5;
    obj->id = static_cast<i16>(id);
    obj->apiobj.character_model = &apicharsys->models[apicharsys->playermodelids[id]];
    obj->suit = Suit_GetDefault(id);
    obj->ai.field_0x134 = 0xff;
    SetGameObjectCharacterData(obj);

    obj->apiobj.start_position = start_position;
    obj->apiobj.position = obj->apiobj.start_position;
    obj->field_0x1018 = 0.5f;
    obj->apiobj.initial_position = start_position;
    obj->apiobj.collision_position = obj->apiobj.initial_position;

    GetTopBot(obj);
    GameObjectDimensions(obj);
    obj->apiobj.anim_packet.animation_index = 1;

    i32 reset_animation = 1;
    if (obj->apiobj.character_model != NULL) {
        void **animation_table = obj->apiobj.character_model->model_data_b;
        if (animation_table[1] == NULL) {
            for (i32 i = 0; i < 0xe9; i++) {
                if (animation_table[i] != NULL) {
                    obj->apiobj.anim_packet.animation_index = static_cast<u16>(i);
                    reset_animation = i;
                    break;
                }
            }
        }
    }
    ResetAnimPacket(&obj->apiobj.anim_packet, reset_animation);
    ResetCharacterIdle(obj, 2, GetDefaultIdle(obj));
    ResetLights(&obj->apiobj.position, &obj->light_data, WORLD->rtl_set);

    obj->ai.mover_height = obj->apiobj.field_0x1dc + default_mover_extra;
    GameObjectOrigin(obj);
    obj->apiobj.last_safe_position = obj->apiobj.position;
    obj->saved_position = obj->apiobj.position;

    game_character_data = obj->apiobj.character_data->game_character;
    obj->field_0xf01 = (obj->field_0xf01 & ~8) | (((game_character_data->flags_090 >> 17) & 1) << 3);
    if (obj->id == id_MOSEISLEYCITIZEN) {
        SetLayers_MOSEISLEYCITIZEN(&obj->field_0x1054);
    } else if (obj->id == id_CANTINAALIEN) {
        static const u8 head_layers[3] = {0, 3, 6};
        static const u8 body_layers[3] = {1, 4, 7};
        static const u8 leg_layers[3] = {2, 5, 8};
        obj->field_0x1054 = 0;
        obj->field_0x1054 |= LayerBit(head_layers[qrand() / 0x5556]);
        obj->field_0x1054 |= LayerBit(body_layers[qrand() / 0x5556]);
        obj->field_0x1054 |= LayerBit(leg_layers[qrand() / 0x5556]);
    } else if (obj->id == id_CLOUDCITYCITIZEN) {
        static const u8 head_layers[3] = {1, 5, 7};
        static const u8 body_layers[3] = {2, 3, 6};
        static const u8 leg_layers[3] = {0, 4, 8};
        obj->field_0x1054 = 0;
        obj->field_0x1054 |= LayerBit(head_layers[qrand() / 0x5556]);
        obj->field_0x1054 |= LayerBit(body_layers[qrand() / 0x5556]);
        obj->field_0x1054 |= LayerBit(leg_layers[qrand() / 0x5556]);
        obj->field_0xf01 = (obj->field_0xf01 & ~8) | (((obj->field_0x1054 >> 7) & 1) << 3);
    } else if (obj->id == id_BOB) {
        SetLayers_BOB(obj);
    } else if (obj->id == id_GEONOSIAN) {
        obj->random_layer_variant = qrand() <= 0x7fff;
    }

    if ((obj->apiobj.character_data->game_character->flags_090 & 0x8000) != 0) {
        obj->apiobj.field_0x1f4 |= 0x20000;
    } else {
        obj->apiobj.field_0x1f4 &= ~0x20000;
    }
    return 1;
}

GameObject_s *AddCreature(i32 id, i32 param) {
    if ((u32)id >= 0x154) {
        return NULL;
    }
    if (apicharsys->playermodelids[id] == -1) {
        return NULL;
    }
    GameObject_s *g = AddGameObject(id);
    if (g == NULL) {
        return NULL;
    }
    InitCreature(g, id, param);
    return g;
}

void InitAICreatures(AISYS_s *system) {
    if (Mission_Active(NULL) != NULL || netclient != 0 || system == NULL) {
        return;
    }

    for (i32 creature_index = 0; creature_index < system->creature_count; ++creature_index) {
        AICREATURE &creature = system->creatures[creature_index];
        if ((creature.flags & AI_CREATURE_FLAG_SKIP_LOW_END) != 0 && g_lowEndLevelBehaviour != 0) {
            continue;
        }

        u8 count = creature.count;
        if (count == 0) {
            continue;
        }

        AIGROUP *group = NULL;
        for (i32 member = 0; member < count; ++member) {
            if (NOAICREATURES != 0 && (GCDataList[creature.type].flags_090 & 0x40) == 0) {
                continue;
            }
            if (((static_cast<u64>(creature.active_mask) >> member) & 1) == 0) {
                continue;
            }

            GameObject_s *object = AddCreature(creature.type, 1);
            if (object == NULL) {
                count = creature.count;
                continue;
            }

            object->apiobj.flags_high &= static_cast<u8>(~APIOBJECT_HIGH_FLAG_CHARACTER);
            object->apiobj.field_0x1f4 |= APIOBJECT_MOTION_FLAG_AI_CONTROLLED;

            const u32 model_flags = apicharsys->char_data[creature.type].model_flags;
            if ((model_flags & 0x200) != 0) {
                object->apiobj.field_0x1f4 |= 0x404;
            } else if ((model_flags & 0x4) != 0) {
                object->apiobj.field_0x1f4 |= 0x401;
            }
            object->field_0x1050 |= (model_flags & 0x1000) != 0 ? 5 : 1;
            object->ai.field_0x134 = static_cast<u8>(creature_index);

            if (member == 0 && creature.count > 1 && creature.start_stagger == 0.0f) {
                GAMECHARACTERDATA *character =
                    static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
                group = CreateAIGroup(system, creature.count_across, creature.x_spacing, creature.z_spacing,
                                      character->movement_speed);
            }
            if (group != NULL) {
                AddToAIGroup(group, &object->apiobj);
            }

            object->ai.area = creature.area;
            object->ai.locator = creature.locator;
            object->ai.respawn_locator = creature.respawn_locator;
            object->ai.creature_set = creature.set;
            count = creature.count;
        }
    }

    system->has_done_reset = 0;
}

void Players_Init(void) {
    memset(Player, 0, sizeof(Player));
    GameObject_s **p = Player;

    if (netclient != 0) {
        return;
    }

    PLAYERCOUNT = 0;

    if ((WORLD->current_level->flags & 2) != 0) {
        i16 *list;
        i16 list0;

        if (FreePlay == 0 && (Hub_UsePlayerList == 0 || HUB_ADATA == NULL || HUB_ADATA != WORLD->area) &&
            (UsePlayerList != 1 ||
             (PlayerList[0] != -1 && (apicharsys->playermodelids[PlayerList[0]] == -1 ||
                                      (PlayerList[1] != -1 && apicharsys->playermodelids[PlayerList[1]] == -1))))) {
            list = Area_PlayerIDList;
            list0 = Area_PlayerIDList[0];
        } else {
            list = PlayerList;
            list0 = PlayerList[0];
        }

        if (LevelChangesInArea == 0 && UsePlayerList != 0 && PlayerProgress[0].active == 0 &&
            PlayerProgress[1].active != 0 && FreePlay == 0 && (WORLD->area->flags & 5) == 1 && list0 != -1 &&
            list0 == Area_StoryModelList[0].model_id && list[1] != -1 && list[1] == Area_StoryModelList[2].model_id &&
            list0 != list[1]) {
            list[0] = list[1];
            list[1] = list0;
        }

        i32 idx = 0;
        while (list[idx] != -1 && idx != 8) {
            GameObject_s *g = AddCreature(list[idx], 1);
            if (g != NULL) {
                i32 pi;
                u8 slot;
                GAMECHARACTERDATA *cd;

                PreResetCode(g);
                PostResetCode(g);

                pi = PLAYERCOUNT;
                g->oldpos = &OldPlrSPos[PLAYERCOUNT];
                p[PLAYERCOUNT] = g;
                p[pi]->batarang = &Batarang[pi];
                g->apiobj.field_0x27c = (char)pi;
                p[pi]->torpedo = GetTorpedoPacket();

                g->field_0x1050 |= 1;
                g->field_0x108e = 0;
                PLAYERCOUNT = PLAYERCOUNT + 1;
                g->hitpoints = DEFAULT_PLAYERHITPOINTS;

                if (UsePlayerList != 0) {
                    char c = g->apiobj.field_0x27c;
                    *(u8 *)&g->apiobj.field_0x1f8 =
                        (u8)((*(u8 *)&g->apiobj.field_0x1f8 & 0x7f) | (PlayerProgress[c].active << 7));

                    if (Area == last_area) {
                        if (UsePlayerList == 1) {
                            g->field_0xe22 = (u8)((g->field_0xe22 & 0xfe) | (PlayerProgress[c].field_0x7 & 1));
                            SetHitPoints(g, PlayerProgress[c].hitpoints);
                            c = g->apiobj.field_0x27c;
                            g->field_0x108e = PlayerProgress[c].field_0xa;
                            g->field_0x106e = PlayerProgress[c].field_0x4;
                            g->field_0xdec = PlayerProgress[c].field_0xc;
                            g->suit = PlayerSuit[c];
                            if (g->torpedo != NULL) {
                                g->torpedo->count = PlayerTorpedoCount[c];
                            }
                        }
                    } else {
                        g->field_0xe22 &= 0xfe;
                        g->field_0x106e = 0;
                    }
                } else {
                    slot = g->apiobj.field_0x289;
                    *(u8 *)&g->apiobj.field_0x1f8 = (u8)((*(u8 *)&g->apiobj.field_0x1f8 & 0x7f) | ((slot == 0) << 7));
                    PlayerProgress[slot].hitpoints = g->current_hp;
                    g->field_0x106e = 0;
                }

                cd = (GAMECHARACTERDATA *)apicharsys->char_data[g->id].field11_0x24;
                g->apiobj.viewdistance = cd->viewdistance;
                g->apiobj.heardistance = cd->heardistance;
                g->apiobj.maxviewheight = cd->maxviewheight;
                g->apiobj.minviewheight = cd->minviewheight;

                {
                    g->ai.nearest_opponent = NULL;
                    g->ai.field_0xdc = 0;
                    g->ai.opponent = NULL;
                    g->ai.field_0xec = 0;
                    g->ai.field_0xe0 = 0x4e6e6b28;
                    g->ai.field_0xf0 = 0x4e6e6b28;
                    g->apiobj.field387_0x2a0 = 0;
                    g->apiobj.field388_0x2a4 = 0;
                    g->field_0xebc = 0;
                    g->field_0xec0 = 0;
                    g->opponent = NULL;
                    g->last_attacker = NULL;
                    g->field_0xecc = 0;
                    g->field_0xed0 = 0;
                    g->ai.antinode_timer = 0.0f;
                    g->field_0xec4 = 0;
                    g->field_0xed8 = 0;
                    g->field_0xec8 = 0;
                    g->ai.field_0x1e5 &= 0xaf;
                    g->field_0xef9 &= 0xf7;
                    g->field_0xf00 |= 0x40;
                    g->field_0xef8 &= 0xfe;
                }
            }
            idx++;
            if (bonusmodearcade != 0 || (HUB_ADATA != NULL && HUB_ADATA == WORLD->area) || VehicleArea != 0)
                break;
        }
    }

    UsePlayerList = 0;

    if (p[0] != NULL) {
        COINPACKET *cp = p[0]->coinpacket = CoinPacket;
        cp->lastcoin = LEGOOBJ_DEFAULTLASTCOIN;
        u32 coins = 0;
        if (Area == last_area) {
            coins = PlayerProgress[0].coins;
        }
        cp->coins = coins;
        p[0]->gizforce_los_info = GizForceLOSInfo;
        memcpy(BackUpPlayers, p[0], 0x439 * 4);
    }

    if (p[1] != NULL) {
        COINPACKET *cp = p[1]->coinpacket = CoinPacket + 1;
        cp->lastcoin = LEGOOBJ_DEFAULTLASTCOIN;
        u32 coins = 0;
        if (Area == last_area) {
            coins = PlayerProgress[1].coins;
        }
        cp->coins = coins;
        p[1]->gizforce_los_info = &GizForceLOSInfo[1];
        memcpy(BackUpPlayers + 0x439, p[1], 0x439 * 4);
    }

    i32 id1 = (p[1] != NULL) ? p[1]->id : -1;
    i32 id0 = (p[0] != NULL) ? p[0]->id : -1;
    RememberPlayerIDs(0, id0, id1);
}

void ResetPlayerMoves(GameObject_s *object) {
    ResetPlayerPacket(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet),
                      reinterpret_cast<CHARACTERDATA_s *>(object->apiobj.character_data));
    object->fall_acceleration_timer = 0.0f;
    object->pause_input_state = 0;
    object->apiobj.movement_direction = v000;
    object->input_toggle_hold_time = TOGGLEHOLDTIME;
    object->field_0xefc |= 0x80;
    ResetCharacterIdle(object, 2, GetDefaultIdle(object));
    if (object->apiobj.character_model->model_data_b[1] != NULL) {
        ResetAnimPacket(&object->apiobj.anim_packet, 1);
        SetAnimTimeRandom(object->apiobj.character_model, &object->apiobj.anim_packet);
    } else {
        ResetAnimPacket(&object->apiobj.anim_packet, -1);
    }
    object->apiobj.field_0x27e = 0;
    object->apiobj.field_0x27d = 0;
    SetGameObjectCharacterData(object);
    object->spawn_protection_timer = 0.0f;
    SetFlicker(object, 0.0f);
    ResetCoinPacket(object->coinpacket);
    object->apiobj.respawn_timer = 0.0f;
    object->apiobj.flags_high &= 0xdf;
    DrawOffsetCode(object, 1);
}

void ResetAICreature(GameObject_s *object, AISYS_s *system) {
    if (object == NULL) {
        return;
    }

    PreResetCode(object);
    PostResetCode(object);
    if (system == NULL) {
        return;
    }

    InitPlayerAI(object);
    ResetPlayerMoves(object);
    Player_ClearContext(object, 1);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));

    auto *character_data = object->apiobj.character_data;
    object->hitpoints = static_cast<GAMECHARACTERDATA *>(character_data->field11_0x24)->hitpoints;
    object->current_hp = object->hitpoints;
    object->field_0xe37 = static_cast<GAMECHARACTERDATA *>(character_data->field11_0x24)->field_0xf5;
    object->field_0xe38 = 4;

    AICREATURE &creature = system->creatures[object->ai.field_0x134];
    object->ai.creature_set = creature.set;
    object->ai.field_0x1e6 &= ~4u;
    object->ai.field_0x1e4 = static_cast<u8>((object->ai.field_0x1e4 & 0x7f) |
                                             ((creature.flags & AI_CREATURE_FLAG_FORMATION_REVERSED) << 7));
    object->apiobj.heardistance = creature.hear_distance;
    object->apiobj.maxviewheight = creature.max_view_height;
    object->apiobj.minviewheight = creature.min_view_height;
    object->apiobj.viewdistance = creature.view_distance;

    const u8 column = object->ai.group_column;
    const u8 row = object->ai.group_row;
    NUVEC offset;
    offset.x = static_cast<f32>((column + 1) / 2) * creature.x_spacing;
    if ((column & 1) != 0) {
        offset.x = -offset.x;
    }
    offset.y = 0.0f;
    offset.z = -creature.z_spacing * static_cast<f32>(row);

    i32 angle;
    AIPATHINFO *path_info;
    if (!(object->ai_respawn_count != 0 && object->ai.respawn_locator != NULL)) {
        angle = creature.y_rot;
        NuVecRotateY(&offset, &offset, angle);
        NuVecAdd(&object->apiobj.position, &offset, &creature.pos);
        path_info = &creature.path_info;
    } else {
        angle = object->ai.respawn_locator->flags;
        NuVecRotateY(&offset, &offset, angle);
        NuVecAdd(&object->apiobj.position, &offset, &object->ai.respawn_locator->position);
        AILOCATOR *locator = object->ai.respawn_locator;
        object->ai.respawn_locator = NULL;
        path_info = &locator->path_info;
    }

    object->apiobj.pos_x = object->apiobj.position.x;
    object->apiobj.pos_y = object->apiobj.position.y;
    object->apiobj.pos_z = object->apiobj.position.z;
    object->apiobj.start_position = object->apiobj.position;
    object->apiobj.initial_position = object->apiobj.position;
    plr_lastpos = object->apiobj.position;
    object->apiobj.velocity = v000;
    object->reset_velocity = v000;

    InitSurfaceInfo(object);
    SetObjOnSurface(object, 0);
    object->apiobj.field_0x276 = static_cast<u16>(angle);
    object->apiobj.facing_angle = static_cast<u16>(angle);
    object->apiobj.movement_facing_angle = static_cast<u16>(angle);
    NuVecRotateYValZ(&object->facing_direction, 1.0f, angle & 0xffff);

    AIPACKET *packet = reinterpret_cast<AIPACKET *>(&object->ai);
    AISysCharacterSetPath(packet, path_info->path);
    AISysCharacterSetPathCnx(packet, &object->apiobj.position, path_info->connection, path_info->direction);
    object->apiobj.field_0x287 = 0;

    AISCRIPTPROCESS *processor = &object->ai.script_process;
    AIScriptProcessorInit(WORLD->ai_sys, packet, processor, &creature, creature.script_name, NULL, 1, NULL, NULL);
    object->ai.field_0x138 = 0xff;
    object->ai.field_0x124 = -1;
    object->ai.field_0x139 = 0;
    AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, packet, 0xff, static_cast<i8>(object->apiobj.field_0x27d));

    ResetAnimPacket(&object->apiobj.anim_packet, 1);
    SetAnimTimeRandom(object->apiobj.character_model, &object->apiobj.anim_packet);
    object->ai.field_0x1e7 &= ~1u;
    object->field_0x1092 = 0;
    object->field_0x1093 = 0;
    object->ai.field_0x110 = 0;
    object->field_0x109c = 0;
    object->ai.reset_position = object->apiobj.position;
    object->ai.movement_target = NULL;
    PortalGameObject(object, 1, 1, -1, WORLD->current_gscn);

    object->apiobj.flags_high |= APIOBJECT_HIGH_FLAG_CHARACTER;
    ++object->ai_respawn_count;
    object->ai.reset_mode = AI_CREATURE_RESET_ACTIVE;

    if (object->ai.group != NULL) {
        AIGROUP *group = object->ai.group;
        if (object->ai.group_member_index < 32) {
            group->member_is_alive |= 1u << object->ai.group_member_index;
        }

        AIROW &group_row = group->rows[object->ai.group_row];
        const i32 member_column = object->ai.group_member_index - creature.count_across * object->ai.group_row;
        if (static_cast<u32>(member_column) < 8) {
            group_row.is_alive |= static_cast<u8>(1u << member_column);
        }
        if (object->ai.group_column == 0) {
            group_row.pos = object->apiobj.position;
            group_row.path_info = object->ai.path_info;
            group_row.y_rot = object->apiobj.field_0x276;
            group_row.is_turning = 0;
            group_row.next_connection = NULL;
        }
    }

    object->apiobj.previous_position[0] = object->apiobj.position.x;
    object->apiobj.previous_position[1] = object->apiobj.position.y;
    object->apiobj.previous_position[2] = object->apiobj.position.z;
    object->field_0x10c8 = object->apiobj.position.x;
    object->field_0x10cc = object->apiobj.position.y;
    object->field_0x10d0 = object->apiobj.position.z;
    if ((object->apiobj.character_data->model_flags & 0x20000000) != 0 &&
        object->apiobj.character_model->model_data_b[0x41] != NULL) {
        object->context_animation_timer = 1000000000.0f;
        object->field_0x7a5 = 0x17;
        object->context_animation = 0x41;
        ResetAnimPacket(&object->apiobj.anim_packet, 0x41);
    }
}

void ResetAICreatures(AISYS_s *system) {
    if (system == NULL || system->has_done_reset != 0) {
        return;
    }
    system->has_done_reset = 1;

    GameObject_s *objects = Obj;
    for (i32 object_index = 0; object_index < HIGHGAMEOBJECT; ++object_index, ++objects) {
        GameObject_s &object = *objects;
        if ((object.apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) == 0 ||
            (object.apiobj.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) == 0) {
            continue;
        }

        object.apiobj.flags_high &= static_cast<u8>(~APIOBJECT_HIGH_FLAG_CHARACTER);
        const bool permanently_removed =
            WORLD->level_progress != NULL && object_index < 64 &&
            (WORLD->level_progress->disabled_ai_object_mask[object_index / 32] & (1u << (object_index & 31))) != 0;
        if (permanently_removed) {
            object.ai.reset_mode = AI_CREATURE_RESET_DISABLED;
            object.apiobj.field_0x287 = 1;
            continue;
        }

        AICREATURE &creature = system->creatures[object.ai.field_0x134];
        object.ai_respawn_count = 0;
        object.ai.reset_mode = AI_CREATURE_RESET_READY;

        AIPACKET *packet = reinterpret_cast<AIPACKET *>(&object.ai);
        AISCRIPTPROCESS *processor = &object.ai.script_process;
        AIScriptProcessorInit(WORLD->ai_sys, packet, processor, &creature, creature.script_name, "InActive", 1, NULL,
                              NULL);
        if (processor->state != NULL && processor->state->name != NULL &&
            NuStrICmp(processor->state->name, "InActive") == 0) {
            creature.activate_type = 2;
        }

        if (creature.activate_type != 0) {
            continue;
        }
        if (Game.save_version < creature.activation_difficulty) {
            object.ai.reset_mode = AI_CREATURE_RESET_DISABLED;
            continue;
        }
        if (creature.count > 1) {
            const f32 start_stagger = creature.start_stagger;
            if (start_stagger > 0.0f && object.ai.group_member_index != 0) {
                object.ai.reset_mode = AI_CREATURE_RESET_STAGGERED;
                object.ai_spawn_delay =
                    static_cast<f32>(static_cast<u32>(object.ai.group_member_index)) * start_stagger;
                continue;
            }
        }

        ResetAICreature(&object, system);
    }
}

GameObject_s *ActivateCharacter(char *name, nuvec_s *position, i32 angle) {
    if (Mission_Active(NULL) != NULL)
        return NULL;
    if (name == NULL)
        return NULL;
    GameObject_s *object = GetNamedGameObject(WORLD->ai_sys, name);
    if (object == NULL || (object->apiobj.flags_high & 0x10) != 0)
        return NULL;
    if (FreePlay != 0 && (object->apiobj.field_0x1f4 & 0x400) == 0)
        return NULL;
    if (object->ai.field_0x134 != 0xff) {
        ResetAICreature(object, WORLD->ai_sys);
    } else {
        object->apiobj.flags_high |= 0x10;
        AIScriptSetBaseScriptStateByName(reinterpret_cast<AISCRIPTPROCESS *>(&object->ai), const_cast<char *>("Base"));
        if (position != NULL) {
            object->apiobj.position = *position;
            object->apiobj.field_0x276 = angle;
            object->apiobj.facing_angle = angle;
            object->apiobj.movement_facing_angle = angle;
            AISysGetCharacterPathPos(WORLD->ai_sys, &object->apiobj, &object->ai, 0xff, 1);
            object->apiobj.initial_position = object->apiobj.position;
            object->apiobj.collision_position = object->apiobj.position;
            plr_lastpos = object->apiobj.position;
            object->apiobj.start_position = object->apiobj.position;
            object->apiobj.respawn_position = object->apiobj.position;
            object->apiobj.last_safe_position = object->apiobj.position;
            object->saved_position = object->apiobj.position;
            object->reset_velocity = v000;
            object->apiobj.velocity = v000;
            InitSurfaceInfo(object);
            SetObjOnSurface(object, 0);
        }
    }
    LightGameObject(object, WORLD->rtl_set);
    return object;
}

i32 NewPlayerCharacter(GameObject_s *object, i32 id, i32 old_id, i32) {
    if (id == old_id)
        return 0;
    if (id < 0 || id >= CHARCOUNT || apicharsys->playermodelids[id] == -1)
        return 0;
    Shards_HandleLostObj(WORLD, object);
    if (VehicleArea == 0)
        AddGameDebris(WORLD->debris_sys, 0x5c, &object->apiobj.collision_position);
    i8 context = object->character_context;
    i16 lean = object->movement_lean_angle;
    u32 lean_enabled = object->apiobj.character_data->model_flags & 0x2000;
    LoseHelmet(object, 0, 1);
    DestroySnakeBody(object);
    object->id = id;
    u8 health = object->current_hp;
    NUVEC velocity = object->apiobj.velocity;
    u8 hitpoints = object->hitpoints;
    u8 saved_flags = (object->apiobj.flags_high >> 5) & 1;
    object->apiobj.character_model = &apicharsys->models[apicharsys->playermodelids[id]];
    object->apiobj.character_data = &apicharsys->char_data[id];
    object->takeover_source = NULL;
    object->suit = Suit_GetDefault(id);
    u32 saved_f14 = object->field_0xf14;
    u32 saved_f20 = object->field_0xf20;
    u8 saved_efe = (object->field_0xefe >> 5) & 1;
    u8 route = object->ai.current_route;
    u8 next_route = object->ai.next_route;
    AIPATHINFO path = object->ai.path_info;
    u32 frame_state = object->ai.frame_state;
    InitPlayerAI(object);
    object->field_0xefe = (object->field_0xefe & ~0x20) | (saved_efe << 5);
    object->field_0xf20 = saved_f20;
    object->ai.next_route = next_route;
    object->ai.path_info = path;
    object->field_0xf14 = saved_f14;
    object->field_0x1004 = 1.0f;
    object->ai.current_route = route;
    object->ai.frame_state = frame_state;
    GAMECHARACTERDATA *data = object->apiobj.character_data->game_character;
    object->field_0x1054 = data->layer_mask;
    object->field_0xe37 = data->field_0xf5;
    object->field_0xe38 = 4;
    Player_ClearContext(object, 1);
    ResetPlayerMoves(object);
    SetGameObjectCharacterData(object);
    ResetAnimPacket(&object->apiobj.anim_packet, -1);
    Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
    GetTopBot(object);
    GameObjectDimensions(object);
    object->hud_icon_timer = 2.0f;
    object->input_toggle_hold_time = TOGGLEHOLDTIME;
    object->field_0xefc |= 0x80;
    InitSurfaceInfo(object);
    i32 on_surface = SetObjOnSurface(object, 1);
    GizForce_ResetLOS(object);
    object->field_0xefe &= ~8;
    object->current_hp = health;
    object->hitpoints = hitpoints;
    object->field_0xef0 = 0;
    object->pause_context_state = 0;
    object->apiobj.flags_high = (object->apiobj.flags_high & ~0x20) | (saved_flags << 5);
    ResetMiniAnimPacket(&object->mini_animation, -1);
    object->weapon_scale = (object->field_0xe22 & 1) != 0 ? 1.0f : 0.0f;
    object->field_0xe32 = 0;
    if (!on_surface)
        object->apiobj.velocity.y = velocity.y;
    else
        object->ground_contact_grace_timer = 0.2f;
    if (context == 0x33 && StartSlide(object, 0)) {
        object->apiobj.velocity.x = velocity.x;
        object->apiobj.velocity.z = velocity.z;
    }
    if (object->apiobj.velocity.y == 0.0f)
        object->apiobj.velocity.y = -0.1f;
    if (lean_enabled && (object->apiobj.character_data->model_flags & 0x2000) != 0)
        object->movement_lean_angle = lean;
    data = apicharsys->char_data[object->id].game_character;
    // Read everything before the stores so the four-float copy vectorizes like retail.
    const f32 viewdistance = data->viewdistance;
    const f32 heardistance = data->heardistance;
    const f32 maxviewheight = data->maxviewheight;
    const f32 minviewheight = data->minviewheight;
    const u32 flags_090_bit17 = (object->apiobj.character_data->game_character->flags_090 >> 17) & 1;
    object->apiobj.viewdistance = viewdistance;
    object->apiobj.heardistance = heardistance;
    object->apiobj.maxviewheight = maxviewheight;
    object->apiobj.minviewheight = minviewheight;
    object->field_0xf01 = (object->field_0xf01 & ~8) | (flags_090_bit17 << 3);
    if (object->id == id_MOSEISLEYCITIZEN) {
        SetLayers_MOSEISLEYCITIZEN(&object->field_0x1054);
    } else if (object->id == id_CANTINAALIEN) {
        static const u8 head_layer[3] = {0, 3, 6};
        static const u8 body_layer[3] = {1, 4, 7};
        static const u8 legs_layer[3] = {2, 5, 8};
        object->field_0x1054 = 0;
        object->field_0x1054 |= LayerBit(head_layer[qrand() / 0x5556]);
        object->field_0x1054 |= LayerBit(body_layer[qrand() / 0x5556]);
        object->field_0x1054 |= LayerBit(legs_layer[qrand() / 0x5556]);
    } else if (object->id == id_CLOUDCITYCITIZEN) {
        static const u8 head_layer[3] = {1, 5, 7};
        static const u8 body_layer[3] = {2, 3, 6};
        static const u8 legs_layer[3] = {0, 4, 8};
        object->field_0x1054 = 0;
        object->field_0x1054 |= LayerBit(head_layer[qrand() / 0x5556]);
        object->field_0x1054 |= LayerBit(body_layer[qrand() / 0x5556]);
        object->field_0x1054 |= LayerBit(legs_layer[qrand() / 0x5556]);
        object->field_0xf01 = (object->field_0xf01 & ~8) | (((object->field_0x1054 >> 7) & 1) << 3);
    } else if (object->id == id_BOB) {
        SetLayers_BOB(object);
    } else if (object->id == id_GEONOSIAN) {
        object->random_layer_variant = qrand() <= 0x7fff;
    }
    if ((object->apiobj.character_data->game_character->flags_090 & 0x8000) != 0)
        object->apiobj.field_0x1f4 |= 0x20000;
    else
        object->apiobj.field_0x1f4 &= ~0x20000;
    return 1;
}

void ResetPlayer(GameObject_s *obj, i32 reset_moves, nuvec_s *position, i32 snap_to_surface) {
    if (obj == NULL) {
        return;
    }

    const bool restore_hitpoints = obj->apiobj.field_0x27c != -1 && obj->field_0x7a5 == 0x2b;

    if (reset_moves != 0) {
        if (position == NULL) {
            position = Player_StartPos(obj);
        }
        if (__builtin_expect(position != NULL, 0)) {
            obj->apiobj.position = *position;
        }

        if (obj->apiobj.field_0x27c != -1) {
            obj->field_0xf00 |= 0x40;
        }

        ResetPlayerMoves(obj);
        SyncPlayerSpawnPosition(obj);

        obj->reset_velocity = v000;
        obj->apiobj.velocity = v000;

        GetTopBot(obj);
        GameObjectDimensions(obj);
        obj->field_0xe23 &= static_cast<u8>(~8u);
        obj->use_model_origin = 0;
        obj->apiobj.field_0x288 = 0;
        GameObjectOrigin(obj);

        ResetRumble(&obj->pad_gamepad->rumble_packet);
        ResetLights(&obj->apiobj.position, &obj->light_data, WORLD->rtl_set);
        ResetMiniAnimPacket(&obj->mini_animation, -1);

        obj->sock_position.location.sock = -1;
        obj->sock_position.location.segment = -1;
        f32 surface_y = kInvalidSurfaceHeight;
        if (WORLD->sock_sys != NULL) {
            if (Door_NextSock != -1 && WORLD->sock_sys->sock[Door_NextSock].valid != 0) {
                complexsockposition_forcesock = Door_NextSock;
                movegamecamera_forcesock = Door_NextSock;
            }
            ComplexSockPosition(WORLD->sock_sys, &obj->apiobj.position, -1, -1, &obj->sock_position);
            ComplexSockAngles(&obj->sock_angles);
            if (obj->apiobj.field_0x27c < 2 && obj->sock_position.location.sock != -1) {
                if (!(BONUS_GUNSHIP_ADATA != NULL && BONUS_GUNSHIP_ADATA == WORLD->area)) {
                    if (BATTLEOVERCORUSCANT_ADATA != NULL && BATTLEOVERCORUSCANT_ADATA == WORLD->area) {
                        SOCK *sock = &WORLD->sock_sys->sock[obj->sock_position.location.sock];
                        if ((sock->flags & SOCK_FLAG_MISSING_C_OR_D) == 0) {
                            obj->apiobj.position = obj->sock_position.midpoint;
                            if (Player[0] != NULL && Player[0]->apiobj.player_controlled && Player[1] != NULL &&
                                Player[1]->apiobj.player_controlled) {
                                NUVEC offset;
                                f32 side = obj == Player[0] ? 1.0f : -1.0f;
                                NuVecNorm(&offset, &sock->segments[obj->sock_position.location.segment].planes[4]);
                                NuVecScale(&offset, &offset, side);
                                NuVecAdd(&obj->apiobj.position, &obj->apiobj.position, &offset);
                            }
                            SyncPlayerSpawnPosition(obj);
                        }
                        snap_to_surface = 0;
                    }
                } else {
                    obj->apiobj.position = obj->sock_position.midpoint;
                    surface_y = FindGunshipHoverHeight(obj);
                    if (WORLD->current_level == BONUS_GUNSHIPA_LDATA && reinterpret_cast<u8 *>(LevFlag)[0] != 0) {
                        obj->apiobj.position.x = 12.0f + obj->apiobj.position.x;
                    }
                    snap_to_surface = 0;
                }
            }
        }

        u8 player_index = static_cast<u8>(obj->apiobj.field_0x27c);
        if (player_index > 7) {
            player_index = obj->apiobj.field_0x289;
        }
        player_index &= 7;

        const u16 facing = static_cast<u16>(PlayerStart[player_index].angle);
        obj->apiobj.roll_angle = 0;
        obj->apiobj.pitch_angle = 0;
        obj->apiobj.movement_facing_angle = facing;
        obj->apiobj.facing_angle = facing;
        obj->apiobj.field_0x276 = facing;
        NuVecRotateYValZ(&obj->facing_direction, 1.0f, facing);

        obj->field_0xc34 = 0x3f800000;
        obj->field_0xc38 = 0.0f;
        CurrentStart(obj, 0, 1);
        obj->field_0xe23 &= static_cast<u8>(~8u);
        obj->field_0xefe &= static_cast<u8>(~4u);
        obj->field_0xeff &= static_cast<u8>(~2u);
        obj->apiobj.model_draw_result = 1;
        obj->use_model_origin = 0;
        obj->apiobj.field_0x288 = 0;
        obj->field_0x1084 = 0;

        InitSurfaceInfo(obj);
        if (surface_y == kInvalidSurfaceHeight) {
            surface_y = GetHoverPosY(obj);
        }
        if (snap_to_surface == 0 || (obj->apiobj.character_data->model_flags & 0x2000) != 0) {
            if (__builtin_expect(surface_y != kInvalidSurfaceHeight, 1)) {
                obj->apiobj.position.y = surface_y;
                SyncPlayerSpawnPosition(obj);
            }
        } else {
            SetObjOnSurface(obj, 0);
        }

        GizForce_ResetLOS(obj);
        PortalGameObject(obj, 1, 1, -1, WORLD->current_gscn);

        if ((ResetBits & 2) != 0 || obj->apiobj.field_0x287 != 0 || restore_hitpoints) {
            SetHitPoints(obj, DEFAULT_PLAYERHITPOINTS);
            obj->field_0xe38 = 4;
        }
        if ((ResetBits & 8) != 0) {
            const i32 progress_index = static_cast<i8>(obj->apiobj.field_0x27c);
            if (progress_index >= 0 && progress_index < 8) {
                if (PlayerProgress[progress_index].field_0xb != 0) {
                    obj->field_0xe31 = 1;
                }
                obj->field_0xe38 = PlayerProgress[progress_index].field_0x9;
            }
        }

        ResetPlayerAI(obj);
        obj->apiobj.last_safe_position = obj->apiobj.position;
        obj->saved_position = obj->apiobj.position;
        if (__builtin_expect((obj->apiobj.character_data->model_flags & 0x2000) != 0, 1)) {
            obj->in_narrow_socket = ObjInNarrowSock(obj, WORLD->sock_sys, WORLD->level_idx);
            f32 speed = GetVehicleSpeedMul(obj, 0.0f);
            obj->field_0xdc8 = speed;
            if (speed > 0.0f) {
                obj->apiobj.velocity.x = NU_SIN_LUT(obj->apiobj.facing_angle) * speed;
                obj->apiobj.velocity.z = speed * NU_COS_LUT(obj->apiobj.facing_angle);
            }
        } else {
            obj->field_0xdc8 = 0.0f;
        }
    } else {
        obj->field_0xdc8 = 0.0f;
    }

    obj->field_0x1004 = 1.0f;
    obj->field_0x101c = 0.0f;
    obj->field_0xda8 = 0.0f;
    obj->field_0xd78 = 1.0f;
    obj->apiobj.field_0x214 = kInvalidSurfaceHeight;
    obj->field_0xdbc = 0.0f;
    obj->field_0xf1c = 0.0f;
    obj->field_0xc54 = -1.0f;
    obj->field_0xde0 = 0.0f;
    obj->apiobj.field_0x287 = 0;
    obj->field_0xe36 = 0;
    obj->apiobj.field_0x285 = 0;
    obj->apiobj.field_0x1f8 &= 0xfa83;
    obj->apiobj.field_0x1fa &= static_cast<u8>(~2u);
    obj->apiobj.field_0x1f4 &= ~0x100u;
    obj->field_0xefc |= 0x80;
    obj->field_0xe21 |= 0x80;
}

void SetPlayerGroupPosition(f32 x, f32 y, f32 z) {
    NUVEC offset = {0.0f, 0.0f, 0.2f};
    for (i32 i = 0; i < PLAYERCOUNT; i++) {
        NUVEC position = {x, y, z};
        NuVecAdd(&position, &position, &offset);
        ResetPlayer(Player[i], 1, &position, 1);
        NuVecRotateY(&offset, &offset, 0x2000);
    }
}

void UpdateCharacterIdle(GameObject_s *object) {
    if (object->apiobj.character_model == NULL) {
        return;
    }

    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);
    const i32 alternate_idle = game_character->field275_0x116 == 0 && (character->model_flags & 0x80) != 0 ? 118 : 25;
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    const i16 requested = packet.requested_animation;

    i32 idle;
    if (requested == 1 && packet.previous_animation == alternate_idle) {
        idle = 1;
    } else if (requested == alternate_idle && packet.previous_animation == 1) {
        idle = alternate_idle;
    } else if (requested != 1 && requested != alternate_idle) {
        idle = GetDefaultIdle(object);
    } else {
        idle = -1;
    }
    if (idle != -1) {
        ResetCharacterIdle(object, 1, idle);
        return;
    }

    if (packet.requested_animation == 1) {
        object->idle_total_time += FRAMETIME;
        object->idle_animation_time += FRAMETIME;
        if (object->idle_animation_time >= object->idle_animation_limit) {
            if (object->idle_animation == 1) {
                NewCharacterIdle(object, 1);
            } else {
                ResetCharacterIdle(object, 1, 1);
            }
        }
    } else if (packet.requested_animation == alternate_idle) {
        object->idle_total_time += FRAMETIME;
        object->idle_animation_time += FRAMETIME;
        if (object->idle_animation_time >= object->idle_animation_limit) {
            if (object->idle_animation == alternate_idle) {
                NewCharacterIdle(object, alternate_idle);
            } else {
                ResetCharacterIdle(object, 1, alternate_idle);
            }
        }
    } else {
        ResetCharacterIdle(object, 1, GetDefaultIdle(object));
        return;
    }
    packet.requested_animation = object->idle_animation;
}

void KeepOnScreen(GameObject_s *object) {
    const f32 previous_keep_time = object->field_0xda8;
    object->field_0xda8 = 0.0f;

    if (newgamecam != 0 || (object->field_0xf03 & 0x10) != 0)
        return;
    i32 controlled = 0;
    // The original checks all eight player slots in straight-line code.
#define KEEPONSCREEN_CHECK_CONTROLLER(index)                                                                      \
    if (Player[index] != NULL && Player[index]->character_context == 0x51 && Player[index]->field_0x788 != NULL && \
        static_cast<TECHNO *>(Player[index]->field_0x788)->controlled_object == object)                           \
        controlled = 1;
    KEEPONSCREEN_CHECK_CONTROLLER(0)
    KEEPONSCREEN_CHECK_CONTROLLER(1)
    KEEPONSCREEN_CHECK_CONTROLLER(2)
    KEEPONSCREEN_CHECK_CONTROLLER(3)
    KEEPONSCREEN_CHECK_CONTROLLER(4)
    KEEPONSCREEN_CHECK_CONTROLLER(5)
    KEEPONSCREEN_CHECK_CONTROLLER(6)
    KEEPONSCREEN_CHECK_CONTROLLER(7)
#undef KEEPONSCREEN_CHECK_CONTROLLER
    if (__builtin_expect(static_cast<i8>(object->apiobj.flags_low) >= 0, 0)) {
        if (controlled == 0)
            return;
    }
    if (object->apiobj.field_0x287 != 0) {
        return;
    }
    if (object->character_context == 0x2b || (object->field_0xefe & 4) == 0 || object->character_context == 0x47 ||
        object->character_context == 0x0f || object->character_context == 0x1f || object->character_context == 0x46 ||
        object->character_context == 0x51 || MiniCutCam != 0 || GetMenuID() != -1) {
        return;
    }

    if (WORLD->current_level == BONUS_GUNSHIPA_LDATA && GameCam->mode == 0x0b) {
        const f32 push_speed = object->apiobj.character_data->game_character->run_speed * 5.0f;
        f32 count = 0.0f;
        NUVEC response;
        NUVEC normal;
#define KEEPONSCREEN_GUNSHIP_PLANE(plane_index, rotation)                                                         \
    if (__builtin_expect(OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[plane_index].point,       \
                                         &PlayPlane[plane_index].normal, NULL, 0.0f, NULL) != 0,                  \
                         1)) {                                                                                    \
        NuVecRotateY(&normal, &GunshipANorm, rotation);                                                           \
        Surface_Deflect(&normal, &object->apiobj.velocity, &response, 0);                                         \
        response.x += normal.x * push_speed;                                                                      \
        response.z += normal.z * push_speed;                                                                      \
        count += 1.0f;                                                                                            \
    }
        KEEPONSCREEN_GUNSHIP_PLANE(3, 0x6000)
        KEEPONSCREEN_GUNSHIP_PLANE(2, 0xa000)
        KEEPONSCREEN_GUNSHIP_PLANE(1, 0x2000)
        KEEPONSCREEN_GUNSHIP_PLANE(4, 0xe000)
#undef KEEPONSCREEN_GUNSHIP_PLANE
        if (__builtin_expect(count > 0.0f, 1)) {
            const f32 inverse_count = 1.0f / count;
            // The original retains the last response, then divides by the
            // number of intersected planes; it does not accumulate responses.
            object->apiobj.movement_direction.x = response.x * inverse_count;
            object->apiobj.movement_direction.z = response.z * inverse_count;
        }
        return;
    }

    if (__builtin_expect(VehicleArea == 0, 0)) {
        GameObject_s *other_player = object == Player[0] ? Player[1] : object == Player[1] ? Player[0] : NULL;
        if (other_player != NULL &&
            (static_cast<i8>(other_player->apiobj.flags_low) >= 0 || other_player->character_context == 0x2b))
            return;
    }
    const i32 two_players = Player[0] != NULL && static_cast<i8>(Player[0]->apiobj.flags_low) < 0 &&
                            Player[1] != NULL && static_cast<i8>(Player[1]->apiobj.flags_low) < 0;
    GAMECHARACTERDATA *character = object->apiobj.character_data->game_character;
    const f32 push_distance = VehicleArea != 0 ? character->walk_speed : character->run_speed;
    NUVEC constrained_movement = object->apiobj.velocity;
    i32 constrained = 0;

    // Near plane has no object-size inset in the original.
    if (__builtin_expect(OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[0].point, &PlayPlane[0].normal, NULL, 0.0f,
                        NULL) != 0, 1)) {
        Surface_Deflect(&PlayPlane[0].normal, &constrained_movement, &constrained_movement, 0);
        constrained_movement.x += PlayPlane[0].normal.x * push_distance;
        constrained_movement.z += PlayPlane[0].normal.z * push_distance;
        constrained = 1;
    }

    // Test the right plane first, then the left plane.  Each response is
    // calculated from the unmodified velocity, exactly as in the target.
    if (OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[2].point, &PlayPlane[2].normal, NULL,
                        -object->apiobj.field_0x1dc, NULL) != 0) {
        Surface_Deflect(&PlayPlane[2].normal, &object->apiobj.velocity, &constrained_movement, 0);
        constrained_movement.x += PlayPlane[2].normal.x * push_distance;
        constrained_movement.z += PlayPlane[2].normal.z * push_distance;
        constrained = 1;
    } else if (OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[1].point, &PlayPlane[1].normal, NULL,
                               -object->apiobj.field_0x1dc, NULL) != 0) {
        Surface_Deflect(&PlayPlane[1].normal, &object->apiobj.velocity, &constrained_movement, 0);
        constrained_movement.x += PlayPlane[1].normal.x * push_distance;
        constrained_movement.z += PlayPlane[1].normal.z * push_distance;
        constrained = 1;
    }

    if (__builtin_expect((VehicleArea != 0 && (WORLD->area == NULL || WORLD->area != PODSPRINT_ADATA)) || KEEPONSCREEN_SIDESONLY == 0 ||
        (two_players && abs(RotDiff(0, GameCam->pitch)) > 0x2000), 0)) {
        PLAYPLANE_s *depth_plane = NULL;
        if (OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[4].point, &PlayPlane[4].normal, NULL,
                            -object->apiobj.field_0x1e0, NULL) != 0) {
            // Crossing the lower screen plane pushes toward the depth plane
            // selected by the camera pitch.
            depth_plane = !(GameCam->dir.y <= 0.0f) ? &PlayPlane[5] : &PlayPlane[0];
        } else if (OnOrInsidePlane(&object->apiobj.collision_position, &PlayPlane[3].point, &PlayPlane[3].normal, NULL,
                                   -object->apiobj.field_0x1e0, NULL) != 0) {
            // The upper screen plane uses the opposite depth response.  This
            // distinction is important: using the same plane for both sides
            // can cancel movement in the wrong world-space direction.
            depth_plane = !(GameCam->dir.y <= 0.0f) ? &PlayPlane[0] : &PlayPlane[5];
        }

        if (depth_plane != NULL) {
            Surface_Deflect(&depth_plane->normal, &object->apiobj.velocity, &constrained_movement, 0);
            constrained_movement.x += depth_plane->normal.x * push_distance;
            constrained_movement.z += depth_plane->normal.z * push_distance;
            constrained = 1;
        }
    }

    if (__builtin_expect(!!(constrained), 0)) {
        object->apiobj.movement_direction.x = constrained_movement.x;
        object->apiobj.movement_direction.z = constrained_movement.z;
        object->field_0xda8 = previous_keep_time + FRAMETIME;
        SpecialMove_Cancel(object);
    } else {
        object->field_0xda8 = 0.0f;
    }
}

void MovePlayerSpline(GameObject_s *object) {
    APIOBJECT_s &api = object->apiobj;
    api.field_0x214 = api.field_0x218;
    GAMEPAD_s *pad = object->pad_gamepad;
    api.start_position = api.position;
    api.initial_position = api.collision_position;
    api.field_0x27e = api.field_0x27d;
    pad->previous_input_angle = pad->input_angle;
    pad->previous_input_magnitude = pad->input_magnitude;
    object->previous_movement_angle = api.field_0x276;
    object->field_0xefd &= ~0x40;

    if (api.model_draw_result == 0)
        object->field_0xf1c += FRAMETIME;
    else
        object->field_0xf1c = 0.0f;
    if (object->character_context == 0x2b)
        object->turn_braking += FRAMETIME;

    f32 speed = object->run_speed_override == 1000000000.0f ? api.character_data->game_character->run_speed
                                                            : object->run_speed_override;
    const f32 distance = speed * FRAMETIME;
    MoveSplinePosition(&object->movement_spline_position, distance);

    NUVEC position;
    u16 angle;
    u16 pitch;
    PointAlongSpline(object->movement_spline, object->movement_spline_position.along, &position, &angle, &pitch,
                     object->movement_spline_position.looping);
    if (distance < 0.0f) {
        angle += 0x8000;
        pitch = -pitch;
    }
    api.movement_facing_angle = angle;
    api.facing_angle = SeekRot(api.facing_angle, angle, spline_seek_ang);
    api.field_0x276 = api.facing_angle;
    api.pitch_angle = SeekRot(api.pitch_angle, -pitch, spline_seek_ang);

    if (WORLD->current_level != PODSPRINTA_LDATA || distance > 0.0f) {
        object->movement_spline_lateral_speed = SeekValF(object->movement_spline_lateral_speed,
                                                         -object->movement_spline_offset.x, spline_xoffset_speed_seek);
    }
    object->movement_spline_offset.x += object->movement_spline_lateral_speed * FRAMETIME;
    if (object->movement_spline_offset.x != 0.0f || object->movement_spline_offset.y != 0.0f ||
        object->movement_spline_offset.z != 0.0f) {
        NUVEC offset;
        NuVecRotateX(&offset, &object->movement_spline_offset, api.pitch_angle);
        NuVecRotateY(&offset, &offset, api.field_0x276);
        NuVecAdd(&position, &position, &offset);
    }

    if (object->spline_follow_terrain != 0) {
        const f32 hover_height = GetVehicleHoverHeight(object, NULL);
        f32 target_y = api.collision_position.y - api.lower_position.y + hover_height;
        if (WORLD->current_level == PODSPRINTA_LDATA)
            NewTerrPlatformsOff();
        api.field_0x218 = GameShadow(object, &position, 5.0f, 0x1f);
        if (api.field_0x218 == 2000000.0f) {
            position.y = SeekValF(api.position.y, target_y, 5.0f);
        } else {
            position.y = SeekValF(api.position.y, target_y + api.field_0x218, 5.0f);
            if (position.y < api.field_0x218)
                position.y = api.field_0x218;
        }
        if (WORLD->current_level == PODSPRINTA_LDATA)
            position.y += object->movement_spline_offset.y;
    }

    if (object->delayed_turn_timer > 0.0f && object->character_context != 0x2a && object->character_context != 0x36 &&
        object->character_context != 0x3a) {
        object->delayed_turn_timer -= FRAMETIME;
    }
    if (WORLD->current_level == SPEEDERCHASEA_LDATA)
        position.y += VehicleTurnOrLoopOffset(object);

    SeekVec(&api.position, &api.position, &position, spline_seek_pos);
    NuVecSub(&api.velocity, &api.position, &api.start_position);
    NuVecScale(&api.velocity, &api.velocity, 1.0f / FRAMETIME);
    object->field_0x1086 = 0;

    if ((api.character_data->game_character->flags_090 & 1) != 0) {
        i32 roll = 0;
        if (object->character_context != 0x36 && object->character_context != 0x2a &&
            object->character_context != 0x3a) {
            roll = static_cast<i32>(static_cast<f32>(RotDiff(object->previous_movement_angle, api.field_0x276)) /
                                    FRAMETIME);
            roll += static_cast<i32>(object->movement_spline_lateral_speed * spline_xoffset_speed_roll);
        }
        i32 target_roll;
        if (roll < -0x10000) {
            target_roll = -0x2000;
        } else if (roll > 0x10000) {
            target_roll = 0x2000;
        } else {
            target_roll = roll / 4;
            if (target_roll < -0x2000)
                target_roll = -0x2000;
            else if (target_roll > 0x2000)
                target_roll = 0x2000;
        }
        object->movement_lean_angle = SeekRot(object->movement_lean_angle, target_roll, 8.0f);
    }
    APIObjectVelocities(object);
    GameObjectOrigin(object);
}

// Original: 3,219 bytes.
void MovePlayer(GameObject_s *object) {
    GAMEPAD_s *pad = object->pad_gamepad;
    APIOBJECT &api = object->apiobj;
    const i32 analog_y = pad->pad != NULL ? pad->pad->analog_left_y : 0;
    const i32 analog_x = pad->pad != NULL ? pad->pad->analog_left_x : 0;
    object->field_0xe22 &= ~GAMEOBJECT_E22_FLAG_INPUT_ANGLE_VALID;
    if (pad->input_magnitude != pad->previous_input_magnitude)
        pad->allocated_5a |= GAMEPAD_RUNTIME_MAGNITUDE_CHANGED;
    api.field_0x214 = api.field_0x218;
    api.start_position = api.position;
    api.initial_position = api.collision_position;
    api.field_0x27e = api.field_0x27d;
    pad->previous_input_angle = pad->input_angle;
    pad->previous_input_magnitude = pad->input_magnitude;
    object->field_0xefd &= ~GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS;
    object->previous_movement_angle = api.field_0x276;
    object->field_0xf1c = api.model_draw_result == 0 ? object->field_0xf1c + FRAMETIME : 0.0f;
    if (__builtin_expect(object->character_context == 0x2b, 0))
        object->turn_braking += FRAMETIME;
    if (__builtin_expect(!!(object->field_0xf03 & 4), 0)) {
        APIObjectVelocities(object);
        GameObjectOrigin(object);
        return;
    }
    if (api.field_0x287 != 0)
        return;
    pad->allocated_5a &= ~GAMEPAD_RUNTIME_WAGGLED;
    pad->waggle_magnitude = 0.0f;
    object->field_0x107a = -1;
    object->field_0xefd &= ~GAMEOBJECT_MOVEMENT_FLAG_FACE_REVERSED;
    const i32 menu_id = GetMenuID();
    if (__builtin_expect((api.field_0x1f8 & APIOBJECT_FLAG_AI_PLAYER_MASK) != APIOBJECT_FLAG_PLAYER_ACTIVE, 0)) {
        if (object->field_0xefc & 0x10) {
            pad->input_magnitude = 0.0f;
            object->pad_gamepad->buttons_held = 0;
            object->pad_gamepad->buttons_pressed = 0;
        } else if (object->ai.movement_stopped == 1) {
            pad->input_magnitude = 0.0f;
            pad->input_mode = 1;
            pad->input_direction_z = pad->input_direction_x = 0.0f;
        } else if (object->character_context == 0x17 && (object->field_0xf04 & 1) == 0) {
            pad->input_magnitude = 0.0f;
        } else {
            f32 x = object->ai.movement_position.x - api.position.x;
            f32 z = object->ai.movement_position.z - api.position.z;
            if (x == 0.0f) {
                if (z == 0.0f) {
                    pad->input_magnitude = 0.0f;
                    pad->input_angle = 0;
                    pad->input_direction_z = pad->input_direction_x = 0.0f;
                    goto ai_input_done;
                }
            }
            {
                f32 distance = NuFsqrt(x * x + z * z);
                f32 inverse = 1.0f / distance;
                pad->input_magnitude = distance / ai_moveradius;
                pad->input_direction_z = z * inverse;
                pad->input_direction_x = x * inverse;
                if (pad->input_magnitude > 1.0f)
                    pad->input_magnitude = 1.0f;
                pad->input_angle = NuAtan2D(pad->input_direction_x, pad->input_direction_z);
                pad->input_angle = NuAngSub(pad->input_angle, GameCam->input_yaw);
            }
        ai_input_done:
            pad->input_mode = 1;
        }
        if (object->context_target_position != NULL) {
            pad->input_magnitude = 0.0f;
        } else {
            GAMECHARACTERDATA *data = api.character_data->game_character;
            f32 run_speed = object->field_0xee0 == 1000000000.0f ? data->run_speed : object->field_0xee0;
            f32 walk_speed =
                object->walk_speed_override == 1000000000.0f ? data->walk_speed : object->walk_speed_override;
            pad->input_magnitude = (object->ai.runtime_flags & 4) ? 0.0f : pad->input_magnitude * run_speed;
            if ((object->ai.runtime_flags & 8) == 0 && pad->input_magnitude < data->tiptoe_speed * 0.5f)
                pad->input_magnitude = 0.0f;
            if ((object->ai.movement_flags & 8) == 0) {
                if (object->ai.goal_speed_mode == 1 && pad->input_magnitude > walk_speed)
                    pad->input_magnitude = walk_speed;
                else if (object->ai.goal_speed_mode == 2 && pad->input_magnitude > data->tiptoe_speed)
                    pad->input_magnitude = data->tiptoe_speed;
            }
            if ((api.field_0x1f4 & 0x400) == 0 && (object->field_0xefb & 2) == 0 &&
                (object->field_0xf1c >= OFFSCREEN_CATCHUP_TIME ||
                 (api.model_draw_result == 0 && (object->tag_flags & 2) == 0 && drop_back_in_timer > 0.0f)) &&
                (pad->input_magnitude + 0.001f >= walk_speed || object->id == id_SPEEDERBIKE)) {
                object->field_0xefd |= GAMEOBJECT_MOVEMENT_FLAG_FACE_REVERSED;
            }
        }
        object->pad_gamepad->operator_data = object->ai.movement_look_target;
    } else {
        if (!(FadeSys.fade > 0.0f) && newgamecam == 0 &&
            !(api.player_controlled && (MiniCutCam == 2 || (menu_id != 0x0e && menu_id != -1)))) {
            if (object->pad_gamepad == ViewCamGetGamePad()) {
                pad->input_magnitude = 0.0f;
            } else {
                f32 x, z;
                u32 buttons = pad->buttons_held;
                if (buttons & (GAMEPAD_DLEFT | GAMEPAD_DRIGHT | GAMEPAD_DUP | GAMEPAD_DDOWN)) {
                    x = buttons & GAMEPAD_DLEFT ? -127.5f : buttons & GAMEPAD_DRIGHT ? 127.5f : 0.0f;
                    z = buttons & GAMEPAD_DDOWN ? -127.5f : buttons & GAMEPAD_DUP ? 127.5f : 0.0f;
                    if (x != 0.0f || z != 0.0f)
                        pad->input_mode = 1;
                } else {
                    x = static_cast<f32>(analog_x) - 127.5f;
                    z = static_cast<f32>(analog_y) - 127.5f;
                    if (WORLD->current_level != DOGFIGHTA_LDATA)
                        z = -z;
                    if (__builtin_expect(x * x + z * z < 1806.25f, 0)) {
                        x = z = 0.0f;
                    } else if (__builtin_expect(x != 0.0f || z != 0.0f, 1)) {
                        pad->input_mode = 2;
                    }
                }
                NUVEC input;
                input.x = x * 0.007843137718737125f;
                input.z = z * 0.007843137718737125f;
                NUANG angle = NuAtan2D(input.x, input.z);
                NUVEC axis = {0.0f, 0.0f,
                              fabsf(input.x) > fabsf(input.z) ? fabsf(input.x) : fabsf(input.z)};
                NuVecRotateY(&input, &axis, angle);
                f32 magnitude = NuFsqrt(input.x * input.x + input.z * input.z);
                ShiftPadDirectionHistory(object, pad);
                if (0.2f > magnitude) {
                    pad->input_magnitude = 0.0f;
                } else {
                    GAMECHARACTERDATA *data = api.character_data->game_character;
                    if ((api.character_data->model_flags & 0x20) != 0 &&
                        (api.field_0x27c == -1 ? object->current_hp <= 2 : object->field_0xe38 <= 2)) {
                        pad->input_magnitude = data->walk_speed;
                    } else if (magnitude < 0.5f && (data->flags_090 & 8) == 0) {
                        pad->input_magnitude = data->tiptoe_speed;
                    } else if (magnitude < 0.8f) {
                        pad->input_magnitude = data->walk_speed;
                    } else {
                        pad->input_magnitude = data->run_speed;
                    }
                }
                // The original player path writes X to +0x2c and Z to +0x30;
                // the AI path below uses the opposite component convention.
                pad->input_direction_x = input.z;
                pad->input_direction_z = input.x;
                if (pad->input_magnitude > 0.0f)
                    pad->input_angle = NuAtan2D(input.x, input.z);
                i32 waggle = GamePad_Waggle(object->pad_gamepad);
                pad->allocated_5a =
                    static_cast<u8>((pad->allocated_5a & ~GAMEPAD_RUNTIME_WAGGLED) | ((waggle & 1) << 3));
                if ((Player[0] == object || Player[1] == object)) {
                    pad->waggle_magnitude = GamePad_Rotate(object);
                } else {
                    pad->waggle_magnitude = 0.0f;
                }
            }
        } else {
            pad->input_magnitude = 0.0f;
            pad->input_direction_x = 0.0f;
            pad->input_direction_z = 0.0f;
            pad->input_mode = 1;
        }
    }
    if (__builtin_expect(static_cast<i8>(api.flags_low) < 0, 1) && menu_id == 0x0e &&
        (object == Player[0] || object == Player[1])) {
        i32 index = object == Player[0] ? 0 : 1;
        Hub_PadSpeed[index] = pad->input_magnitude;
        Hub_PadAngle[index] = pad->input_angle;
        pad->input_mode = 1;
        pad->input_magnitude = 0.0f;
        pad->input_direction_z = pad->input_direction_x = 0.0f;
    } else if (pad->input_magnitude > 0.0f) {
        pad->animation_input_magnitude = pad->input_magnitude;
    }
    if (object->character_context == 0x3b)
        return;
    if (object->character_context == 0x39)
        return;
    object->field_0x1086 = 2;
    ShoveSystemCheckGameObject(object);
    if (object->id == id_DRAGBOMB) {
        MovePlayer_ROLLING(object);
    } else if (__builtin_expect(MovePlayer_TWIST(object) == 0, 1) && __builtin_expect(MovePlayer_CIRCLE(object) == 0, 1) &&
               __builtin_expect(MovePlayer_GUNSHIPIN(object) == 0, 1) && __builtin_expect(MovePlayer_POD(object) == 0, 1)) {
        if ((api.character_data->model_flags & 0x2000) == 0)
            MovePlayer_DIRECTIONAL(object);
        else
            MovePlayer_VEHICLEDIRECTIONAL(object);
    }
    if (LIFTPLAYER != 0 && __builtin_expect(menu_id == -1, 1) && api.field_0x287 == 0 && object->character_context != 0x2b &&
        (api.character_data->game_character->flags_090 & 0x80) == 0 && api.player_controlled &&
        (pad->buttons_held & GAMEPAD_LIFT) != 0 && pad != ViewCamGetGamePad()) {
        GameObject_s *lift = object;
        if (object->character_context == 0x51) {
            TECHNO *techno = static_cast<TECHNO *>(object->field_0x788);
            if (techno != NULL && techno->target_mode == 1 && techno->controlled_object != NULL)
                lift = static_cast<GameObject_s *>(techno->controlled_object);
        }
        lift->apiobj.position.y += 1.5f * FRAMETIME;
        lift->apiobj.velocity.y = 0.0f;
        lift->apiobj.field_0x27d = 0;
        lift->field_0x105c = 0;
        if (__builtin_expect(lift->character_context == 0x33, 1))
            lift->character_context = -1;
    }
    api.field_0x1fa |= 1;
    api.previous_velocity.x = api.velocity.x;
    api.previous_velocity.y = api.velocity.y;
    api.previous_velocity.z = api.velocity.z;
    APIObjectVelocities(object);
    GameObjectOrigin(object);
}

void TerrainPlayer(GameObject_s *object) {
    NUVEC next_position;
    GAMEPAD_s *const entry_pad = object->pad_gamepad;
    const u8 entry_is_underwater = object->apiobj.is_underwater;
    const u8 entry_intersects_water = object->apiobj.intersects_water;
    const i32 entry_menu_id = GetMenuID();
    const i16 entry_platform = object->apiobj.supporting_platform_id;

    i32 impact_count = -1;
    // Original 0x102720 skips integration and callbacks for inactive objects.
    if (object->apiobj.field_0x287 == 0 && object->field_0xcc0 != NULL && object->character_context == 0x3b) {
        TakeOverCode(object, object->pad_gamepad->buttons_pressed & GAMEPAD_TAG);
    } else if (object->apiobj.field_0x287 == 0) {
        TERRAIN_IMPACT_RECORD impact_records[8];

        APIOBJECT &api = object->apiobj;
        i32 terrain_mask = TERRAINMASK_NONWEAPON;
        if ((api.character_data->model_flags & 0x10) == 0 &&
            (api.character_data->game_character->flags_090 & 0x8000) == 0) {
            terrain_mask |= TERRAINMASK_NONDROID;
        }
        if ((api.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) != 0 && object->id != id_DRAGBOMB &&
            (WORLD->area != HOTHBATTLE_ADATA || api.character_data->game_character->field_0x28 != 0.0f)) {
            terrain_mask |= 0x40;
        }
        object->field_0xe20 &= static_cast<u8>(~8u);
        const f32 entry_vertical_velocity = api.velocity.y;
        const AIPATHINFO &path_info = object->ai.path_info;
        i32 special_path_endpoints = false;
        if (path_info.path != NULL && path_info.connection != NULL) {
            const AIPATHCNX *path_connection = path_info.connection;
            const AIPATHNODE *nodes = path_info.path->nodes;
            special_path_endpoints = ((nodes[path_connection->node_indices[path_info.direction]].runtime_flags |
                                       nodes[path_connection->node_indices[path_info.direction == 0]].runtime_flags) &
                                      0x82) != 0;
        }
        i32 shadow_grounding = false;
        i32 retain_floor = false;
        if (object == CarWashHack) {
            shadow_grounding = true;
        } else if ((api.character_data->game_character->flags_090 & 0x80) != 0) {
            api.velocity.z = 0.0f;
            api.velocity.x = 0.0f;
            shadow_grounding = true;
        } else if ((api.field_0x1f8 & 0x40) == 0 && (CInfo[object->character_context].flags & 0x8000) == 0) {
            const bool ai_controlled = (api.field_0x1f4 & APIOBJECT_MOTION_FLAG_AI_CONTROLLED) != 0;
            const bool ordinary_ai_path =
                ai_controlled && object->movement_spline == NULL && object->character_context != 0x1c &&
                object->context_target_position == NULL && !special_path_endpoints &&
                (path_info.flags & (AIPATHINFO_FLAG_ON_PATH | AIPATHINFO_FLAG_NARROW_PATH)) ==
                    AIPATHINFO_FLAG_ON_PATH &&
                Technos_FindControllingTechno(object) == NULL && (api.field_0x1fa & 4) == 0 &&
                (path_info.connection == NULL ||
                 ((path_info.connection->original_traversal_flags[0] & static_cast<u32>(LEGO_AIPATHCNX_BLOCKAGE)) ==
                      0 &&
                  (path_info.connection->traversal_flags[0] & static_cast<u32>(LEGO_AIPATHCNX_WALLSHUFFLE)) == 0 &&
                  (path_info.connection->traversal_flags[0] & static_cast<u32>(LEGO_AIPATHCNX_FULLTERRAIN)) == 0)) &&
                (object->character_context != 0x5a || object->field_0x7a3 != 0);
            if (ordinary_ai_path) {
                shadow_grounding = true;
                retain_floor = (api.field_0x1f8 & 0x10) != 0;
            } else {
                retain_floor = !ai_controlled && (api.field_0x1f8 & 0x10) != 0;
                if ((object->field_0xf00 & 0x40) != 0) {
                    object->field_0xf00 &= static_cast<u8>(~0x40u);
                    shadow_grounding = true;
                } else {
                    shadow_grounding = (api.field_0x1f8 & 8) != 0;
                }
            }
        }

        // TerrainPlayer selects either the inexpensive shadow-grounding path used
        // by ordinary AI or the swept capsule resolver required by players and
        // special traversal connections, then refreshes floor/contact state before
        // dispatching the character movement callback.
        const f32 lower_bound = object->character_bottom * api.field_0xa8;
        // Original 0x102856..0x10316f consumes the previous edge-stop request
        // and probes the next horizontal position before integrating movement.
        const bool player_edge_probe = static_cast<i8>(api.flags_low) < 0 && object->spawn_protection_timer > 1.25f;
        const bool requested_edge_stop = static_cast<i8>(object->edge_stop_requests) > 0;
        object->field_0xf04 &= static_cast<u8>(~0x40u);
        object->edge_stop_requests = 0;
        if (VehicleArea == 0 && api.field_0x287 == 0 &&
            (api.field_0x27d != 0 || requested_edge_stop ||
             (player_edge_probe && object->character_context == -1 && object->field_0xe31 != 1))) {
            const bool walkable_floor = api.field_0x218 != 2000000.0f && object->surface_normal.y > NuTrigTable[0x238e];
            const bool probe_edge = walkable_floor ? (player_edge_probe || requested_edge_stop ||
                                                      (CInfo[object->character_context].parameter & 1) != 0)
                                                   : requested_edge_stop;
            if (probe_edge && api.movement_direction.x == 0.0f && api.movement_direction.z == 0.0f &&
                (api.velocity.x != 0.0f || api.velocity.z != 0.0f)) {

                next_position.x = api.position.x + api.velocity.x * FRAMETIME;
                next_position.z = api.position.z + api.velocity.z * FRAMETIME;
                next_position.y = api.position.y;
                const f32 next_floor = GameShadow(object, &next_position, 5.0f, terrain_mask | 0x1f);
                if (next_floor != 2000000.0f && ShadNorm.y > NuTrigTable[0x238e] &&
                    (player_edge_probe ? api.collision_min.y : api.field_0x218) - next_floor > 0.2f) {
                    object->field_0xf04 |= 0x40;
                    api.velocity.x = 0.0f;
                    api.velocity.z = 0.0f;
                }
            }
        }
        f32 movement_threshold = 0.0f;
        if (!(object->pad_gamepad->input_magnitude > 0.0f) && object->character_context != 0x33) {
            const i32 surface = static_cast<i8>(api.field_0x281);
            const bool slippery =
                api.field_0x27d != 0 && surface >= 0 && surface <= 31 && 1.0f > TerSurface[surface].movement_scale;
            if (!slippery && AnimSpeed(api.character_model, CurrentAnim(&api.anim_packet)) == 0.0f &&
                (api.character_data->model_flags & 0x800) == 0) {
                movement_threshold = api.character_data->game_character->field_0x0c * api.field_0xa8;
            }
        }

        const f32 speed_squared =
            (api.velocity.x * api.velocity.x + api.velocity.y * api.velocity.y) + api.velocity.z * api.velocity.z;
        const bool perform_motion =
            !((object->field_0xefc & 0x20) == 0 && (object->field_0xf02 & 0x40) != 0 && !special_path_endpoints &&
              object->ai.field_0x180 == NULL && static_cast<i8>(api.flags_low) >= 0 &&
              (object->field_0xe23 & 0x10) == 0 && object->field_0xe31 == 0 && object->character_context != 0 &&
              object->character_context != 0x1c && api.field_0x281 != 8 &&
              (static_cast<u32>(GameTimer.update_count) > 1 || (api.field_0x1f4 & 5) != 0) &&
              (api.field_0x27d & 3) != 0 && api.supporting_platform_id == -1 &&
              movement_threshold * movement_threshold >= speed_squared && object->character_context != 0x0f &&
              object->character_context != 0x0b && object->character_context != 0x1e);
        api.field_0x1f8 = (api.field_0x1f8 & ~4u) | (perform_motion ? 0u : 4u);
        object->field_0x1084 = 0;
        const u8 integration_flags = object->field_0xe20;
        // Original 0x102952/0x102bfb carries this byte through the call-free
        // integration selection before setting its terrain-update bit.
        const i32 previous_platform = entry_platform;
        const bool direct_integration = (api.flags_low & 0x20) != 0 || (integration_flags & 0x20) != 0 ||
                                        object->movement_spline != NULL || object->move_override != NULL ||
                                        (object->character_context == 0x0f && object->field_0x7a3 <= 1) ||
                                        (object->character_context == 0x2c && object->field_0x7a3 == 0) ||
                                        (object->character_context == 0x47 && object->action_movement_state == 1);
        if (direct_integration) {
            // Original 0x102940/0x102bf8 selects integration without a terrain
            // query for these motion owners and action states.
            api.respawn_timer = 0.0f;
            object->field_0xe20 = integration_flags | 2;
            if (TimingBarSet == 2)
                TBOPENFN("Ter", 2);
            api.field_0x27d = 0;
            api.supporting_platform_id = -1;
            object->field_0x6b0 = 0;
            api.position.x += api.velocity.x * FRAMETIME;
            api.position.y += api.velocity.y * FRAMETIME;
            api.position.z += api.velocity.z * FRAMETIME;
            if (TimingBarSet == 2)
                TBCLOSEFN("Ter", 2);
        } else if (shadow_grounding || retain_floor || (WORLD->current_level->flags & 0x10) != 0) {
            // Ordinary path-following AI uses the target's inexpensive shadow
            // grounding path.  Full swept collision is reserved for path
            // connections whose traversal flags require special collision.
            api.respawn_timer = 0.0f;
            if (perform_motion) {
                if (TimingBarSet == 2)
                    TBOPENFN("Ter", 2);
                object->field_0xe20 |= 2;
                api.supporting_platform_id = -1;
                api.position.x += api.velocity.x * FRAMETIME;
                api.position.y += api.velocity.y * FRAMETIME;
                api.position.z += api.velocity.z * FRAMETIME;

                if (shadow_grounding) {
                    if (object == CarWashHack) {
                        NewTerrPlatformsOff();
                    }
                    if (testshadowfix == 0) {
                        api.field_0x218 = GameShadow(object, &api.collision_position, 5.0f, terrain_mask | 0x1f);
                    } else {
                        NUVEC shadow_position = api.collision_position;
                        if (api.initial_position.y > api.collision_position.y) {
                            shadow_position.y = api.initial_position.y;
                        } else {
                            shadow_position.y = api.collision_position.y;
                        }
                        api.field_0x218 = GameShadow(object, &shadow_position, 5.0f, terrain_mask | 0x1f);
                    }
                    api.supporting_platform_id = static_cast<i16>(NewShadowOnPlatform());
                    GetSurfaceInfo(object, api.field_0x218 != 2000000.0f ? 1 : 0, api.field_0x218);
                    if (api.field_0x218 == 2000000.0f) {
                        api.field_0x281 = 0;
                        object->field_0xe41 = 0;
                        object->surface_normal = NUVEC{0.0f, 1.0f, 0.0f};
                        api.water_height = 2000000.0f;
                        api.field_0x220 = 2000000.0f;
                        api.field_0x27f = static_cast<u8>(-1);
                        api.field_0x280 = static_cast<u8>(-1);
                        object->field_0x1078 = -1;
                    }
                    api.is_underwater = static_cast<u8>(UnderWater(object));
                    api.intersects_water = static_cast<u8>(IntersectWater(object));
                    if (api.supporting_platform_id == -1 && api.field_0x27d != 0 && object->field_0x1078 != -1) {
                        api.supporting_platform_id = object->field_0x1078;
                    }
                }
                const f32 grounded_lower_bound = object->character_bottom * api.field_0xa8;
                if (api.field_0x218 != 2000000.0f && api.position.y + grounded_lower_bound < api.field_0x218) {
                    api.velocity.y = 0.0f;
                    api.position.y = api.field_0x218 - grounded_lower_bound;
                }

                api.field_0x27d = 0;
                if (GameObjectNearFloor(object, 1.0f, NULL) != 0) {
                    api.field_0x27d |= APIOBJECT_TERRAIN_CONTACT_NEAR_FLOOR;
                }
                if (api.collision_min.y <= api.field_0x218) {
                    api.field_0x27d |= APIOBJECT_TERRAIN_CONTACT_FLOOR;
                }
                if ((api.field_0x27d & APIOBJECT_TERRAIN_CONTACT_FLOOR) != 0) {
                    object->field_0x1084 = 1;
                    object->contact_position.x = api.position.x;
                    object->contact_position.y = api.field_0x218;
                    object->contact_position.z = api.position.z;
                    object->contact_normal = shadow_grounding ? object->surface_normal : NUVEC{0.0f, 1.0f, 0.0f};
                    object->field_0x6b0 = api.field_0x281;
                    object->terrain_impact_speed = -api.velocity.y;
                } else {
                    object->field_0x6b0 = 0;
                }
                if (TimingBarSet == 2)
                    TBCLOSEFN("Ter", 2);
            }
        } else {
            if (!perform_motion) {
                api.velocity.x = 0.0f;
                api.velocity.y = 0.0f;
                api.velocity.z = 0.0f;
            } else {
                // UpdateGameObjects supplies objects from the initialized Obj
                // array. Original 0x104830 computes its unsigned index before
                // invoking any full-collision callbacks.
                const i32 object_index =
                    static_cast<u32>(reinterpret_cast<uintptr_t>(object) - reinterpret_cast<uintptr_t>(Obj)) /
                    sizeof(GameObject_s);
                NUVEC collision_position = api.position;
                collision_position.y += lower_bound;

                NUVEC movement;
                NUVEC incoming_velocity = api.velocity;
                NuVecScale(&movement, &api.velocity, FRAMETIME);

                // Original 0x1048e9 clears the complete contact metadata word.
                object->field_0x6b0 = 0;
                memset(object->pad_6b1, 0, sizeof(object->pad_6b1));
                if (TimingBarSet == 2)
                    TBOPENFN("Ter", 2);
                // Original 0x1048fc..0x10497c and 0x10545f..0x105544.
                if (object->use_model_origin != 0 && api.field_0x27d != 0 &&
                    (movement.x != 0.0f || movement.z != 0.0f) &&
                    !(WORLD->current_level == ENDORBATTLEC_LDATA && object->id == id_ATST)) {
                    i32 deflection_mode = 0;
                    if (VehicleArea != 0 && (api.character_data->model_flags & 0x2000) != 0 &&
                        api.character_data->game_character->field_0x28 != 0.0f) {
                        deflection_mode = 2;
                    } else if (object->character_context == -1 && object->pad_gamepad->input_magnitude > 0.0f) {
                        deflection_mode = 1;
                    }
                    if (deflection_mode != 0) {
                        NUVEC deflected;
                        Surface_Deflect(&object->surface_normal, &movement, &deflected, deflection_mode);
                        if (deflected.y > movement.y) {
                            movement = deflected;
                            if (deflection_mode == 1) {
                                object->field_0xe20 |= 8;
                            }
                        }
                    }
                }
                TerrainSetPlatConnectTol((api.character_data->model_flags & 0x800) != 0 ? 0.0f : 0.01f);
                if ((object->ai.field_0x1e4 & 0x80) != 0) {
                    IgnoreWallSplines = 1;
                }
                TerrainSetImpactData(impact_records, &impact_count, 8);
                f32 movement_scale = 1.0f;
                if ((object->field_0xefd & 4) != 0) {
                    GAMECHARACTERDATA *character = api.character_data->game_character;
                    if (object->id == id_SPEEDERBIKE) {
                        if (object->ai.goal_speed_mode == 1) {
                            movement_scale = 15.0f / character->walk_speed;
                        } else if (object->ai.goal_speed_mode == 2) {
                            movement_scale = 15.0f / character->tiptoe_speed;
                        } else {
                            movement_scale = 15.0f / character->run_speed;
                        }
                    } else {
                        movement_scale = 2.5f / character->run_speed;
                    }
                    movement.x *= movement_scale;
                    movement.z *= movement_scale;
                }

                // Original 0x104a8f..0x104bdb excludes owned and selected character
                // platforms for this query, then re-enables the same list.
                i32 excluded_platforms[32];
                i32 excluded_count = 0;
                if (object->field_0x107a != -1) {
                    excluded_platforms[excluded_count++] = object->field_0x107a;
                }
                if (object->field_0x107c != -1) {
                    excluded_platforms[excluded_count++] = object->field_0x107c;
                }
                if (object->character_context != 0) {
                    if (object->character_context == 0x58 && object->field_0x7a3 == 1) {
                        GIZMOBLOWUP_s *blowup = static_cast<GIZMOBLOWUP_s *>(object->field_0x788);
                        if (blowup->platform_id != -1) {
                            excluded_platforms[excluded_count++] = blowup->platform_id;
                        }
                    }
                } else {
                    GameObject_s *target = object->airborne_collision_target;
                    if (target != NULL && target->field_0x107c != -1) {
                        excluded_platforms[excluded_count++] = target->field_0x107c;
                    }
                }
                CHARPLATFORMSYS_s *platforms = WORLD->char_platform_sys;
                if (platforms != NULL && VehicleArea == 0) {
                    const bool player = static_cast<i8>(api.flags_low) < 0;
                    if (!player || object->character_context == 0x3d || object->field_0xcc0 != NULL ||
                        object->character_context == 0x3b ||
                        (api.character_data->game_character->flags_090 & 0x8040) != 0) {
                        for (i32 i = 0; i < platforms->platform_count; ++i) {
                            GameObject_s *platform_object = platforms->platforms[i].object;
                            if (platform_object != NULL && ((object->field_0xf02 & 8) == 0 || player)) {
                                excluded_platforms[excluded_count++] = platform_object->field_0x107c;
                            }
                        }
                    }
                }
                for (i32 i = 0; i < excluded_count; ++i) {
                    PlatOnOff(excluded_platforms[i], 0);
                }

                NewTerrainScaleYMask(&collision_position, &movement, reinterpret_cast<u8 *>(&object->field_0x105c),
                                     object_index,
                                     entry_pad->input_magnitude > 0.0f ? 0.0f : movement_threshold * FRAMETIME,
                                     api.collision_radius, object->collision_y_scale, 0, 0, terrain_mask);
                IgnoreWallSplines = 0;
                for (i32 i = 0; i < excluded_count; ++i) {
                    PlatOnOff(excluded_platforms[i], 1);
                }
                if ((object->field_0xefd & 4) != 0) {
                    movement.x /= movement_scale;
                    movement.z /= movement_scale;
                }
                extern i32 TERRAINCALLS;
                ++TERRAINCALLS;
                if (TimingBarSet == 2)
                    TBCLOSEFN("Ter", 2);
                api.supporting_platform_id = static_cast<i16>(NewShadowOnPlatform());

                object->field_0x1084 = static_cast<u8>(TerrImpact);
                if (object->field_0x1084 != 0 && impact_count > 0) {
                    // Original copies the last complete 0x1c-byte impact record,
                    // including material and flags, before deriving impact speed.
                    memcpy(&object->contact_position, &impact_records[impact_count - 1], sizeof(TERRAIN_IMPACT_RECORD));
                    object->terrain_impact_speed = -NuVecDot(&object->contact_normal, &incoming_velocity);
                } else {
                    object->field_0x1084 = 0;
                }
                NuVecScale(&api.velocity, &movement, 1.0f / FRAMETIME);
                api.position.x = collision_position.x;
                const f32 updated_y = collision_position.y - object->character_bottom * api.field_0xa8;
                api.position.z = collision_position.z;
                api.position.y = updated_y;
                // Original 0x104eb0..0x10501a carries all three headings with the
                // rotation of the supporting platform.
                if (api.field_0x287 == 0 && api.supporting_platform_id != -1) {
                    NUMTX *current = NULL;
                    NUMTX *previous = NULL;
                    TerrainPlatGetMtx(api.supporting_platform_id, &previous, &current);
                    if (previous != NULL && current != NULL) {
                        NUVEC forward = {0.0f, 0.0f, 1.0f};
                        NUVEC previous_forward, current_forward;
                        NuVecMtxRotate(&previous_forward, &forward, previous);
                        const NUANG previous_heading = NuAtan2D(previous_forward.x, previous_forward.z);
                        NuVecMtxRotate(&current_forward, &forward, current);
                        const NUANG current_heading = NuAtan2D(current_forward.x, current_forward.z);
                        const NUANG rotation = NuAngSub(current_heading, previous_heading);
                        if (rotation != 0) {
                            api.field_0x276 = NuAngAdd(rotation, api.field_0x276);
                            api.facing_angle = NuAngAdd(rotation, api.facing_angle);
                            api.movement_facing_angle = NuAngAdd(rotation, api.movement_facing_angle);
                        }
                    }
                }
                if (api.field_0x287 == 0) {
                    bool wall_impact = false;
                    if (object->field_0x1084 != 0 && (api.flags_low & 0x84) == 0 && object->character_context != 0 &&
                        object->character_context != 0x43 && object->character_context != 0x45 &&
                        (object->field_0xf02 & 1) == 0) {
                        for (i32 i = 0; i < impact_count; ++i) {
                            if (impact_records[i].normal.y < 0.707f) {
                                wall_impact = true;
                                break;
                            }
                        }
                    }
                    if (!wall_impact) {
                        api.respawn_timer -= FRAMETIME;
                        if (api.respawn_timer < 0.0f) {
                            api.respawn_timer = 0.0f;
                        }
                    } else {
                        api.respawn_timer += FRAMETIME;
                    }
                }
            }
            const f32 floor_height = (api.flags_low & 4) != 0
                                         ? api.field_0x218
                                         : GameShadow(object, &api.position, 5.0f, terrain_mask | 0x1f);
            bool stopped_without_terrain = false;
            if (floor_height != 2000000.0f) {
                object->field_0xe20 |= 2;
                if (perform_motion) {
                    const f32 bottom_offset = api.field_0xa8 * object->character_bottom;
                    const f32 top = object->character_top * api.field_0xa8 + api.position.y;
                    const f32 bottom = api.position.y + bottom_offset;
                    const f32 height = top - bottom;
                    if (floor_height > 0.1f * height + bottom && height + 0.01f >= (floor_height + height) - top) {
                        api.position.y = floor_height - bottom_offset;
                    }
                }
            } else {
                object->field_0xe20 &= static_cast<u8>(~2u);
                if (NOTERRAINSTOP != 0) {
                    api.velocity.z = 0.0f;
                    api.velocity.x = 0.0f;
                    api.position.x = api.start_position.x;
                    api.position.z = api.start_position.z;
                    stopped_without_terrain = true;
                }
            }
            if (!stopped_without_terrain && (api.flags_low & 4) == 0) {
                api.field_0x218 = floor_height;
                GetSurfaceInfo(object, floor_height != 2000000.0f ? 1 : 0, floor_height);
                if (api.supporting_platform_id == -1 && api.field_0x27d != 0 && object->field_0x1078 != -1) {
                    api.supporting_platform_id = object->field_0x1078;
                }
                if ((api.character_data->game_character->flags_090 & 0x8000) == 0 &&
                    object->field_0x1020 == 2000000.0f && object->field_0x1078 != -1) {
                    const f32 reflection_height = FindReflectionNoPlatforms(&api.position);
                    if (reflection_height != 2000000.0f) {
                        object->field_0x1020 = reflection_height;
                        object->field_0x1087 = 2;
                    }
                }
            }
            // Contact reconstruction joins the near-floor test at 0x102d26.
            api.field_0x27d = object->field_0x105c != 0 ? APIOBJECT_TERRAIN_CONTACT_FLOOR : 0;
            if (GameObjectNearFloor(object, 1.0f, NULL) != 0) {
                api.field_0x27d |= APIOBJECT_TERRAIN_CONTACT_NEAR_FLOOR;
            }
            if (api.field_0x27d != 0) {
                object->field_0xeff |= 2;
            }
        }

        // Original 0x1029e6/0x103cb4 transfers vertical momentum only when
        // leaving or landing on the object's recorded character platform.
        if (api.field_0x27d != 0) {
            if (previous_platform == -1 && api.supporting_platform_id != -1 &&
                api.supporting_platform_id == object->field_0x1078 && entry_vertical_velocity < 0.0f) {
                const i32 platform_id = api.supporting_platform_id;
                GRABBER_s *grabber = WORLD->grabber;
                if (grabber != NULL && grabber->platform_id == platform_id) {
                    if (grabber->platform_contact_timer <= 0.0f) {
                        grabber->platform_contact_timer = 0.25f;
                    }
                } else {
                    GameObject_s *platform_owner =
                        CharPlatform_FindObjFromPlatID(WORLD->char_platform_sys, platform_id);
                    if (platform_owner != NULL &&
                        platform_owner->apiobj.character_data->game_character->field_0x28 > 0.0f) {
                        platform_owner->apiobj.velocity.y =
                            entry_vertical_velocity * 0.5f + platform_owner->apiobj.velocity.y;
                    }
                }
            }
        } else {
            if (previous_platform != -1 && api.supporting_platform_id == -1 &&
                object->field_0x1078 == previous_platform) {
                const i32 platform_id = previous_platform;
                GRABBER_s *grabber = WORLD->grabber;
                if (grabber != NULL && grabber->platform_id == platform_id) {
                    if (grabber->platform_contact_timer <= 0.0f) {
                        grabber->platform_contact_timer = 0.25f;
                    }
                } else {
                    GameObject_s *platform_owner =
                        CharPlatform_FindObjFromPlatID(WORLD->char_platform_sys, platform_id);
                    if (platform_owner != NULL &&
                        platform_owner->apiobj.character_data->game_character->field_0x28 > 0.0f) {
                        platform_owner->apiobj.velocity.y -= entry_vertical_velocity * 0.5f;
                    }
                }
            }
        }

        // Original 0x102a01..0x102a9a handles a moving character platform
        // above the player before dispatching the normal movement callback.
        if (VehicleArea == 0 && static_cast<i8>(api.flags_low) < 0 && api.supporting_platform_id != -1) {
            GameObject_s *platform_owner =
                CharPlatform_FindObjFromPlatID(WORLD->char_platform_sys, api.supporting_platform_id);
            if (platform_owner != NULL) {
                const GAMECHARACTERDATA *data = platform_owner->apiobj.character_data->game_character;
                if ((data->flags_090 & 0x1000) != 0 && platform_owner->apiobj.position.y > api.position.y &&
                    platform_owner->apiobj.horizontal_velocity_magnitude > 0.5f * data->run_speed) {
                    StartFlatten(platform_owner, object);
                }
            }
        }

        if (api.field_0x287 != 0) {
            if (api.field_0x27d != 0) {
                api.velocity.y = 0.0f;
                api.flags_low |= 4;
            }
            if ((api.flags_low & 4) == 0) {
                ApplyGravity(object, NULL, 0.0f, 0.0f, NULL);
            }
        } else {
            // Original 0x1032a0 checks swept contacts or the standing surface.
            const i32 check_terrain_hazards =
                object->character_context != 0x2b && (api.flags_low & 4) == 0 && gone_through_door_to_new_level == 0;
            // Original 0x1038b0 completes the doomed state on contact, timeout,
            // or a collision during the falling variant.
            if (object->character_context == 0x2b &&
                (api.field_0x27d != 0 || api.model_draw_result == 0 || object->turn_braking >= 5.0f ||
                 (object->field_0xe36 == 3 && object->field_0x1084 != 0))) {
                if (api.model_draw_result != 0) {
                    InstantKillParts(object, 0, 0.0f);
                }
                KillPlayer(object, 3, 1, NULL);
                if (WORLD->current_level == CRUISERD_LDATA && object->field_0xe36 == 3) {
                    MiscTime = 0.1f;
                }
            }
            if (check_terrain_hazards && CannotKill(object) == 0 && api.field_0x287 == 0 &&
                (VehicleArea == 0 || static_cast<i8>(api.flags_low) < 0) && (object->field_0xefe & 4) != 0 &&
                (object->field_0xefa & 4) == 0 && object->character_context != 0x5d) {
                if (object->field_0x1084 != 0) {
                    const i32 count = impact_count == -1 ? 1 : impact_count;
                    for (i32 i = 0; i < count; ++i) {
                        const i32 surface =
                            static_cast<i8>(impact_count == -1 ? object->field_0x6b0 : impact_records[i].material[0]);
                        NUVEC *normal = impact_count == -1 ? &object->contact_normal : &impact_records[i].normal;
                        if (static_cast<u32>(surface) > 31) {
                            continue;
                        }
                        if (WORLD->current_level == BONUS_GUNSHIPA_LDATA ||
                            (PODRACE_ADATA != NULL && WORLD->area == PODRACE_ADATA && GameTimer.time_elapsed >= 1.0f)) {
                            NewRumble(object->pad_gamepad->pad, (static_cast<f32>(qrand()) * (1.0f / 65535.0f)) * 0.5f,
                                      0);
                            if (api.model_draw_result == 0 && object->character_context != 0x23) {
                                if (static_cast<i8>(api.flags_low) < 0) {
                                    LoseCoins(object, 2);
                                }
                                KillPlayer(object, 2, 1, NULL);
                                break;
                            }
                        } else if (((api.character_data->model_flags & 0x2000) != 0 ||
                                    (api.character_data->game_character->flags_090 & 0x04000000) != 0) &&
                                   api.field_0x27f == 6) {
                            break;
                        }
                        if ((WORLD->area == NULL || WORLD->area != PODSPRINT_ADATA) &&
                            TerrainKillPlayer(object, surface, &normal->y) != 0) {
                            if (WORLD->current_level == ASTEROIDCHASEC_LDATA) {
                                GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
                            }
                            break;
                        }
                        if ((TerSurface[surface].flags & 0x80) != 0 && WORLD->current_level != BONUS_GUNSHIPA_LDATA) {
                            NewRumble(object->pad_gamepad->pad, 0.6f, 0);
                            NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                            if (object->field_0xd24 != 1.0f && CannotKill(object) == 0) {
                                ObjHitObj(NULL, object, 1, 0, 0, 1);
                            }
                            break;
                        }
                    }
                } else if (api.field_0x27d != 0 && (WORLD->area == NULL || WORLD->area != PODSPRINT_ADATA)) {
                    const i32 surface = static_cast<i8>(api.field_0x281);
                    if (static_cast<u32>(surface) <= 31) {
                        TerrainKillPlayer(object, surface, &object->surface_normal.y);
                    }
                }
            }
            if (check_terrain_hazards && CannotKill(object) == 0 && (object->field_0xefa & 4) == 0 &&
                api.field_0x287 == 0 && api.water_height != 2000000.0f) {
                const i32 layer = static_cast<i8>(api.field_0x27f);
                const bool tractor_swamp =
                    layer == 9 && object->id == id_TRACTOR && WORLD->area != NULL && WORLD->area == DAGOBAH_ADATA;
                if ((layer == 3 || tractor_swamp || (TerLayer[layer].flags & 0x20) != 0 ||
                     ((api.character_data->model_flags & 0x2000) == 0 &&
                      (api.character_data->game_character->flags_090 & 0x04000000) == 0)) &&
                    NoLayerKill(object) == 0 && object->character_context != 0x5d) {
                    const u32 layer_flags = TerLayer[static_cast<i8>(api.field_0x27f)].flags;
                    const bool exempt_context =
                        static_cast<i8>(api.flags_low) >= 0 &&
                        (object->character_context == 0x46 || object->character_context == 0x47);
                    const bool rescue_below =
                        ((layer_flags & 1) != 0 || tractor_swamp) && api.water_height > api.position.y &&
                        !(VehicleArea != 0 && BonusArea != 0 && api.character_data->game_character->field_0x28 > 0.0f);
                    const bool rescue_above = (layer_flags & 0x20) != 0 && api.collision_max.y > api.water_height;
                    if (!((rescue_below || rescue_above) && !exempt_context)) {
                        if ((api.character_data->model_flags & 0x40) == 0 && object->id != id_SNAKE &&
                            api.field_0x27f == 9 && api.water_height > api.collision_max.y) {
                            if (BonusWinner == -1) {
                                const i32 coins = LoseCoins(object, 2);
                                if (coins > 0) {
                                    AddPickups(coins, 0, 0, 0, &api.upper_position, NULL, 2.0f,
                                               static_cast<i8>(api.field_0x27c), 1.0f, 2000000.0f, NULL, 1, 0, false);
                                }
                            }
                            InstantKillParts(object, 1, 2.0f);
                            KillPlayer(object, 3, 1, NULL);
                        }
                    } else {
                        if (object->doomed_escape_locator != NULL && static_cast<i8>(api.flags_low) >= 0) {
                            if ((object->field_0xefd & 8) != 0 || TouchHacks::AiPlayerTakeDamageOnKillRescue(*object)) {
                                ObjHitObj(NULL, object, 1, 0, 0, 1);
                            }
                            if (api.field_0x287 == 0) {
                                StartBigJump(object, &object->doomed_escape_locator->position, 0, 1.0f, 1.0f, 0, 0);
                            }
                        } else {
                            ClearLastSafeTakeOver(object);
                            Player_ClearContext(object, 1);
                            object->context_animation = 5;
                            if (object->character_context != -1 &&
                                api.character_model->model_data_b[api.anim_packet.requested_animation] != NULL) {
                                object->context_animation = api.anim_packet.requested_animation;
                            }
                            object->character_context = 0x2b;
                            object->field_0xe31 = 0;
                            if ((TerLayer[static_cast<i8>(api.field_0x27f)].flags & 0x20) == 0) {
                                if (api.field_0x27f == 6) {
                                    api.velocity.y = -0.25f;
                                    object->field_0xe36 = 2;
                                    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                                } else if (WORLD->current_level == VADERC_LDATA) {
                                    api.velocity.y = -0.5f;
                                    object->field_0xe36 = 4;
                                    NewBuzz(object->pad_gamepad->pad, 0.1f, 0);
                                } else {
                                    object->field_0xe36 = 1;
                                }
                            } else {
                                object->field_0xe36 = 3;
                            }
                            object->turn_braking = 0.0f;
                            PlayDieSfx(object);
                            object->current_hp = 0;
                            if (static_cast<i8>(api.flags_low) < 0 && object->coinpacket != NULL &&
                                object->coinpacket->coins != 0 && BonusWinner == -1) {
                                const i32 coins = LoseCoins(object, 2);
                                if (coins > 0) {
                                    f32 height = api.water_height;
                                    if (height != 2000000.0f &&
                                        (TerLayer[static_cast<i8>(api.field_0x27f)].flags & 1) == 0) {
                                        height = 2000000.0f;
                                    }
                                    AddPickups(coins, 0, 0, 0, &api.collision_position, NULL, 2.0f,
                                               static_cast<i8>(api.field_0x27c), 1.0f, height, NULL, 1, 0, false);
                                }
                                object->coinpacket->scale = 1.5f;
                            }
                        }
                    }
                }
            }
            // Original 0x1036a0 refreshes these even if the hazard handler killed
            // the object. The bypass and level-transition routes retain old state.
            if (check_terrain_hazards) {
                api.is_underwater = static_cast<u8>(UnderWater(object));
                api.intersects_water = static_cast<u8>(IntersectWater(object));
            }
            // Original 0x102ddb emits the splash only on a dry-to-water transition.
            if (api.intersects_water != 0 && (entry_is_underwater | entry_intersects_water) == 0) {
                bool splash = object->character_context == 0x1f || object->character_context == 0 ||
                              (CInfo[object->character_context].flags & 4) != 0 || object->character_context == 0x0e ||
                              object->character_context == 0x0d;
                if (!splash) {
                    const i32 animation = CurrentAnim(&api.anim_packet);
                    splash = animation == 0x28 || animation == 5 || static_cast<u32>(animation - 0x4b) <= 1;
                }
                if (splash) {
                    AddWaterSplash(object, &api.collision_position);
                }
            }
            if (api.model_draw_result != 0 && object->character_context != 0x40 && object->character_context != 0x2c) {
                extern void GameObjectRotation(GameObject_s *, i32);
                GameObjectRotation(object, 2);
            }
            // The jump callback returns to the common saved-button restoration
            // at 0x102ed4, including when it changes the object's gamepad.
            const u32 held_buttons = object->pad_gamepad->buttons_held;
            const u32 pressed_buttons = object->pad_gamepad->buttons_pressed;
            if ((api.field_0x1f8 & 0x180) == 0x80 && (entry_menu_id != -1 || MiniCutCam == 2)) {
                object->pad_gamepad->buttons_held = 0;
                object->pad_gamepad->buttons_pressed = 0;
            }
            Tag_Check(object);
            PreResetCode(object);
            api.character_data->move_fn(object);
            PostResetCode(object);
            if ((api.character_data->model_flags & 0x00200000) == 0) {
                BigJumpCode(object);
            }
            object->pad_gamepad->buttons_held = held_buttons;
            object->pad_gamepad->buttons_pressed = pressed_buttons;
        }

        if ((object->field_0xefa & 4) == 0 && CurTerr != NULL && api.field_0x287 == 0 &&
            (WORLD->current_level->flags & 0xe0) == 0 &&
            static_cast<f32>(CurTerr->minimum_height) - 5.0f > api.position.y) {
            if (api.model_draw_result != 0) {
                InstantKillParts(object, 0, 0.0f);
            }
            KillPlayer(object, 3, 1, NULL);
        }
    }

    // Common epilogue at 0x102b60.
    entry_pad->allocated_5a &= 0xe9;
    object->field_0xd14 = 0;
    entry_pad->operator_data = NULL;
    NuVecRotateYValZ(&object->facing_direction, 1.0f, object->apiobj.field_0x276);
}

void AnimatePlayer(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    packet.previous_animation = packet.animation_index;
    object->mini_animation.previous_animation = object->mini_animation.animation_index;
    CHARACTERMODEL_s *model = object->apiobj.character_model;

    if ((object->apiobj.field_0x1f4 & APIOBJECT_STATE_FLAG_IGNORE_DOORS) == 0) {
        object->apiobj.character_data->animate_fn(object);
    }

    if (object->ai.animation_override_from != -1 &&
        ((object->ai.animation_override_from == 0xe9 && object->character_context != 0x1c) ||
         object->ai.animation_override_from == packet.requested_animation)) {
        packet.requested_animation = object->ai.animation_override_to;
    }

    if (model == NULL ||
        (object->apiobj.field_0x287 != 0 && (object->field_0x1018 == 0.0f || object->apiobj.field_0x287 == 1))) {
        return;
    }

    const f32 direction = (object->field_0xefd & GAMEOBJECT_MOVEMENT_FLAG_BACKWARDS) != 0 ? -1.0f : 1.0f;
    f32 movement_speed;
    if (object->apiobj.character_model->model_data_b[1] != NULL) {
        movement_speed = object->pad_gamepad->animation_input_magnitude;
        if (movement_speed > 0.0f && object->id == id_GONKDROID && Cheat_IsOn(8) == 0)
            movement_speed = object->apiobj.character_data->game_character->walk_speed;
    } else if ((object->apiobj.character_data->game_character->flags_090 &
                GAMECHARACTER_FLAG_ANIMATION_SPEED_FROM_VELOCITY) != 0) {
        const f32 forward_speed = object->apiobj.velocity.x * object->facing_direction.x +
                                  object->apiobj.velocity.z * object->facing_direction.z;
        movement_speed = forward_speed < 0.0f ? 0.0f : forward_speed;
    } else {
        movement_speed = object->pad_gamepad->input_magnitude;
    }

    // The original passes signed movement into UpdateAnimPacket.  Clips with
    // CHARACTER_ANIMATION_FLAG_REVERSE_WITH_MOVEMENT (including the acrobatic
    // jump clips) start at their end frame and play backwards while the
    // character is travelling backwards.
    f32 time_multiplier = 1.0f;
    if (object->character_context == 0x2d && object->field_0x788 != NULL)
        time_multiplier = GizBuildItMul(object);
    UpdateAnimPacket(model, &packet, (FRAMETIME * 30.0f) * time_multiplier, movement_speed * direction,
                     time_multiplier * FRAMETIME,
                     object->apiobj.character_data->game_character->backwards_speed_multiplier);
    if ((packet.flags & ANIMPACKET_FLAG_ANIMATION_CHANGED) != 0 &&
        (packet.blending != 0 ? packet.blend_animation_b : packet.animation_index) == 0x5f) {
        f32 *time = packet.blending != 0 ? &packet.blend_target_time : &packet.current_time;
        const f32 duration = AnimDuration(object->id, 0x5f, 0.0f, 0.0f, 0);
        i32 count = static_cast<i32>(duration / 0.3f);
        if (NuFmod(duration, 3.0f) > 0.15f)
            ++count;
        const i32 phase = qrand() / (0xffff / count + 1);
        CHARACTERMODEL_s *current_model = object->apiobj.character_model;
        const f32 start =
            static_cast<f32>(phase) *
                (0.3f * static_cast<CHARACTERANIM_s *>(current_model->model_data_a[0x5f])->playback_rate) +
            1.0f;
        if (NuAnimEndFrame(current_model->model_data_b[0x5f]) > start)
            *time = start;
        object->field_0xe21 = (object->field_0xe21 & ~0x40) | ((phase & 1) << 6);
    }
    AutoWeaponOnOff(object);
    AddFootSteps(object);
}

i32 DrawGameObjectsProcess() {
    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {
        object->apiobj.field_0x1f4 |= 0x800;
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001)
            continue;
        const u8 previous_draw_pending = object->apiobj.field_0x288 & 1;
        object->apiobj.field_0x288 = 0;
        object->field_0xe23 = (object->field_0xe23 & ~8) | (previous_draw_pending << 3);
        object->field_0xe24 &= ~8;
        if (object->apiobj.field_0x287 != 0 && (object->field_0x101c > 0.0f || object->field_0x1018 == 0.0f))
            continue;
        if ((object->field_0xe20 & 0x20) != 0 && object->character_context != 0x23 && object->character_context != 0x24)
            continue;
        if (object->character_context == 0x0f && object->field_0x7a3 == 1 && object->field_0x788 != NULL &&
            (static_cast<TELEPORT_s *>(object->field_0x788)->flags & 4) == 0)
            continue;
        if (object->character_context != 0x39 && object->character_context != 0x3b) {
            if (CharClipToBlobShadows != 0 && WORLD->area != KAMINO_ADATA && WORLD->area != HOTHBATTLE_ADATA &&
                WORLD->area != DEATHSTARBATTLE2_ADATA && static_cast<i8>(object->field_0xf04) < 0 &&
                object->apiobj.field_0x27c == -1 && (object->field_0xefb & 8) == 0 &&
                object->ai_update_distance > static_cast<u8>(WORLD->current_level->blob_shadow_fade_far))
                continue;

            NUVEC scale;
            NUVEC position;
            position.x = object->apiobj.position.x;
            position.y = object->apiobj.position.y;
            if ((object->apiobj.field_0x1f4 & 0x100) != 0)
                position.y += object->character_bottom * object->apiobj.field_0xa8;
            position.z = object->apiobj.position.z;
            NuVecAdd(&position, &position, &object->render_offset);
            object->field_0xf01 = (object->field_0xf01 & ~1) | (object->apiobj.model_draw_result & 1);
            object->apiobj.model_draw_result = 1;
            scale.x = object->apiobj.field_0xa8;
            const f32 drop_scale = DropInOutScale(object);
            scale.x *= drop_scale;
            scale.y = scale.z = scale.x;
            NUMTX matrix __attribute__((aligned(16)));
            NUMTX shadow __attribute__((aligned(16)));
            NUMTX reflected __attribute__((aligned(16)));
            NUMTX orientation __attribute__((aligned(16)));
            NuMtxSetScale(&matrix, &scale);
            switch (object->field_0x1086) {
                case 0:
                    DrawSetRotationY(&orientation, NUANG_180DEG);
                    if (object->apiobj.pitch_angle != 0)
                        ShopRotateX(&orientation, object->apiobj.pitch_angle);
                    if (object->apiobj.field_0x276 != 0)
                        ShopRotateY(&orientation, object->apiobj.field_0x276);
                    if (object->apiobj.roll_angle != 0)
                        ShopRotateZ(&orientation, object->apiobj.roll_angle);
                    NuMtxMulVU0(&matrix, &matrix, &orientation);
                    NuMtxTranslate(&matrix, &position);
                    break;
                case 1:
                    DrawSetRotationY(&orientation, object->apiobj.field_0x276 + NUANG_180DEG);
                    if (object->apiobj.pitch_angle != 0)
                        ShopRotateX(&orientation, object->apiobj.pitch_angle);
                    if (object->apiobj.roll_angle != 0)
                        ShopRotateZ(&orientation, object->apiobj.roll_angle);
                    NuMtxMulVU0(&matrix, &matrix, &orientation);
                    NuMtxTranslate(&matrix, &position);
                    break;
                default:
                case 2:
                    DrawSetRotationY(&orientation, object->apiobj.field_0x276 + NUANG_180DEG);
                    if (object->apiobj.roll_angle != 0)
                        ShopRotateZ(&orientation, object->apiobj.roll_angle);
                    if (object->apiobj.pitch_angle != 0)
                        ShopRotateX(&orientation, object->apiobj.pitch_angle);
                    NuMtxMulVU0(&matrix, &matrix, &orientation);
                    NuMtxTranslate(&matrix, &position);
                    break;
                case 3:
                    DrawSetRotationY(&orientation, NUANG_180DEG);
                    if (object->apiobj.roll_angle != 0)
                        ShopRotateZ(&orientation, object->apiobj.roll_angle);
                    if (object->apiobj.pitch_angle != 0)
                        ShopRotateX(&orientation, object->apiobj.pitch_angle);
                    if (object->apiobj.field_0x276 != 0)
                        ShopRotateY(&orientation, object->apiobj.field_0x276);
                    NuMtxMulVU0(&matrix, &matrix, &orientation);
                    NuMtxTranslate(&matrix, &position);
                    break;
                case 4:
                    matrix = object->vehicle_orientation;
                    NuMtxPreScale(&matrix, &scale);
                    NuMtxPreRotateY(&matrix, NUANG_180DEG);
                    NuMtxTranslate(&matrix, &position);
                    break;
                case 5:
                    matrix = object->vehicle_orientation;
                    NuMtxPreScale(&matrix, &scale);
                    NuMtxPreRotateY(&matrix, NUANG_180DEG);
                    break;
            }
            if (object->movement_lean_angle != 0) {
                i32 angle = object->movement_lean_angle;
                if (PODSPRINT_ADATA != NULL && WORLD->area == PODSPRINT_ADATA)
                    angle = static_cast<i32>(angle * PodSprint_RollMul(object));
                NuMtxPreRotateZ(&matrix, angle);
            }
            if (object->secondary_lean_angle != 0)
                NuMtxPreRotateX(&matrix, object->secondary_lean_angle);
            if (object->tertiary_lean_angle != 0)
                NuMtxPreRotateY(&matrix, object->tertiary_lean_angle);
            ApplyExtraRotation(object, &matrix);
            if (object->field_0x1086 != 4 && (object->character_context == 0x23 || object->character_context == 0x24)) {
                const f32 distance =
                    object->apiobj.character_data->collision_radius * object->apiobj.character_data->model_scale * 7.5f;
                f32 offset = 1.0f - drop_scale;
                if (object->character_context != 0x23)
                    offset = -offset;
                offset *= distance;
                NUVEC displacement;
                NuVecMtxTransformVU0(&displacement, &v001, &orientation);
                NuVecScale(&displacement, &displacement, offset);
                NuVecAdd(reinterpret_cast<NUVEC *>(&matrix.m30), reinterpret_cast<NUVEC *>(&matrix.m30), &displacement);
            }
            if (object->use_model_origin <= 1 &&
                (object->id == id_ATST || object->id == id_ATST_LOWRES || object->id == id_ATAT))
                matrix.m31 += object->character_bottom * object->apiobj.field_0xa8;
            object->apiobj.field_0xb8 = matrix;
            if (((object->field_0xefa & 0x40) != 0 && WORLD->rooms_visible_ptr[object->room_id] == 0) ||
                (scale.x == 0.0f && scale.y == 0.0f && scale.z == 0.0f)) {
                object->apiobj.model_draw_result = 0;
                continue;
            }
            object->apiobj.field_0x1f4 ^= 0x800;
            object->field_0xefe &= ~2;
            object->field_0x1088 = 0;
            if (object->field_0x1087 != 0 && object->field_0x1020 != 2000000.0f &&
                static_cast<u32>(static_cast<u8>(WORLD->current_level->reflection_range)) >
                    object->ai_update_distance) {
                object->field_0x1088 = MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                        WORLD->current_level->unknown_0cc, &reflected);
                object->apiobj.field_0x138 = reflected;
            }
            if (object->apiobj.field_0x287 == 0 && object->apiobj.field_0x218 != 2000000.0f &&
                (object->apiobj.field_0x1f4 & 0x40) != 0 &&
                object->apiobj.character_data->game_character->shadow_locators == 0) {
                NuMtxSetIdentity(&shadow);
                ShopRotateY(&shadow, object->apiobj.field_0x276 + NUANG_180DEG);
                if (object->field_0x1060 != 0)
                    ShopRotateZ(&shadow, object->field_0x1060);
                if (object->field_0x105e != 0)
                    ShopRotateX(&shadow, object->field_0x105e);
                shadow.m30 = object->apiobj.position.x;
                shadow.m31 = object->apiobj.field_0x218 + 0.005f;
                shadow.m32 = object->apiobj.position.z;
                object->apiobj.field_0xf8 = shadow;
                object->field_0xefe |= 2;
            }
        }
    }
    return 0;
}

i32 AnakinGreenSabre(GameObject_s *obj) {
    i32 result = 0;
    if (FreePlay == 0 && obj->id == id_ANAKINPADAWAN && WORLD->area != NULL) {
        if (WORLD->area == JEDI_ADATA || WORLD->area == DOOKU_ADATA)
            result = 1;
    }
    return result;
}

void ConfigureComplexShadow(GameObject_s *object) {
    NUVEC shadow_direction;
    if (object != NULL) {
        shadow_direction.x = 65536.0f;
        shadow_direction.y = object->apiobj.collision_position.y;
        shadow_direction.z = object->apiobj.field_0x218 - 0.1f;
    } else {
        shadow_direction = {0.0f, -6.25f, 0.0f};
    }

    const f32 shadow_distance = VehicleArea != 0 ? 150.0f : 30.0f;
    NuRndrShadowDirCol(&shadow_direction, 0x17000000, shadow_distance, shadow_distance);
}

i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags) {
    NUMTX special_matrix;
    NUMTX scaled_matrix;
    NUMTX scaled_reflection;
    NUMTX output_matrices[256];
    i32 drag_bomb = 0;
    if (object != NULL && object->id == id_DRAGBOMB && Cheat_IsOn(5) && WORLD->lev_objs[0x135].active) {
        ResetShadowMapRendering();
        special_matrix = *matrix;
        NuSpecialDrawAt(&WORLD->lev_objs[0x135].special, &special_matrix);
        if (reflection_matrix != NULL) {
            special_matrix = *reflection_matrix;
            NuRndrStartReflectionRender(1);
            NuSpecialDrawAt(&WORLD->lev_objs[0x135].special, &special_matrix);
            NuRndrEndReflectionRender();
        }
        scaled_matrix = *matrix;
        NUVEC scale = {0.5f, 0.5f, 0.5f};
        NuMtxPreScale(&scaled_matrix, &scale);
        matrix = &scaled_matrix;
        if (reflection_matrix != NULL) {
            scaled_reflection = *reflection_matrix;
            NuMtxPreScale(&scaled_reflection, &scale);
            reflection_matrix = &scaled_reflection;
        }
        drag_bomb = 1;
    }

    if (TimingBarSet == 5)
        TBOPENFN("chrs", 5);
    i32 complex_shadow = 0;
    if (!(object != NULL && (object->apiobj.character_data->model_flags & 0x10000) != 0 &&
          (WORLD->area == NULL || WORLD->area != DOGFIGHT_ADATA))) {
        model->hierarchy->suppress_shadow_surface_points = 1;
    } else {
        model->hierarchy->suppress_shadow_surface_points = 0;
        if (reflection_matrix != NULL) {
            ConfigureComplexShadow(object);
            complex_shadow = 1;
        }
    }
    drawcharactermodel_keepmergeaction = game_keepmergeaction;
    MakeLayerList = GCDataList[model->model_id].make_layer_list;
    i32 result = APIDrawCharacterModel(model, object != NULL ? object->apiobj.character_data : NULL, animation, matrix,
                                       secondary_matrix, reflection_matrix, NULL, auxiliary_matrix, object, flags,
                                       object != NULL && JointRotation_On ? object->joint_modifiers : NULL,
                                       object != NULL && JointRotation_On ? object->field_0x1089 : 0, Paused, FRAMETIME,
                                       output_matrices, NULL, WORLD->debris_sys);
    if (TimingBarSet == 5)
        TBCLOSEFN("chrs", 5);

    i32 draw_shadow = 0;
    if (result == 0 && object != NULL) {
        if (auxiliary_matrix != NULL) {
            for (i32 i = 0; i < 16; ++i) {
                if (model->points_of_interest[i] != NULL) {
                    auxiliary_matrix[i] = object->apiobj.field_0xb8;
                    auxiliary_matrix[i].m30 = object->apiobj.position.x;
                    auxiliary_matrix[i].m31 = object->apiobj.position.y;
                    auxiliary_matrix[i].m32 = object->apiobj.position.z;
                    auxiliary_matrix[i].m31 -=
                        (object->character_bottom + object->character_top) * object->apiobj.field_0xa8 * 0.5f;
                }
            }
        }
    } else if (__builtin_expect(result != 0 && object != NULL && (object->field_0xeff & 4) == 0 &&
               (object->apiobj.character_data->model_flags & 0x10000) == 0, 0)) {
        draw_shadow = 1;
    }
    if (g_lowEndLevelBehaviour != 0 && object != NULL && (object->apiobj.flags_low & 0x80) == 0)
        draw_shadow = 0;

    if (draw_shadow && object != NULL && secondary_matrix != NULL) {
        i32 level_alpha = static_cast<u8>(WORLD->current_level->blob_shadow_alpha);
        if (level_alpha != 0 && object->shadow_opacity > 0.0f && object->surface_normal.y > 0.0f &&
            (object->apiobj.water_height == -1.0f ||
             (TerLayer[static_cast<i8>(object->apiobj.field_0x27f)].flags & 1) == 0)) {
            if (object->id == id_MOUSEDROID && WORLD->lev_objs[0xa2].active) {
                special_matrix = *secondary_matrix;
                NuSpecialDrawAtAlpha(&WORLD->lev_objs[0xa2].special, &special_matrix,
                                     object->shadow_opacity * object->surface_normal.y);
            } else if (__builtin_expect(!!(CHARSHADOWS_ON), 1)) {
                PLAYERCHARACTERCONFIG_s *config = object->apiobj.character_data->player_config;
                i32 alpha = config->blob_shadow_alpha;
                if (alpha == 0xff)
                    alpha = level_alpha;
                if (alpha > 0) {
                    f32 radius = config->shadow_radius;
                    if (!(radius < 99.0f))
                        radius = object->apiobj.character_data->collision_radius;
                    if (radius > 0.0f) {
                        f32 drop_scale = DropInOutScale(object);
                        NuRndrAddShadow(NUMTX_GET_ROW_VEC(secondary_matrix, 3),
                                        drop_scale * radius * object->apiobj.field_0xa8,
                                        static_cast<i32>(alpha * object->shadow_opacity * object->surface_normal.y),
                                        object->field_0x105e, 0, object->field_0x1060);
                    }
                }
            }
        }
    }
    if (complex_shadow)
        ConfigureComplexShadow(NULL);
    if (drag_bomb)
        EnableShadowMapRendering(0);
    return result;
}

static void DrawCharacterAttachments(GameObject_s *object, NUMTX *joint_matrices) {
    extern i16 id_BATMAN, id_GLIDEPACK, id_CATWOMAN, id_PENGUIN, id_WHIP, id_UMBRELLA;
    i32 character_id;
    ANIMPACKET_s animation;
    SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
    if (suit != NULL && (suit->flags & 2) != 0) {
        if (object->id != id_BATMAN)
            return;
        character_id = id_GLIDEPACK;
        AnimPacket_MiniToFull(&object->mini_animation, &animation);
    } else if (object->id == id_CATWOMAN && (AnimPlaying(&object->apiobj.anim_packet, 0x51, 1, 1) ||
                                             AnimPlaying(&object->apiobj.anim_packet, 0x55, 1, 1) ||
                                             AnimPlaying(&object->apiobj.anim_packet, 0x56, 1, 1) ||
                                             AnimPlaying(&object->apiobj.anim_packet, 0x94, 1, 1))) {
        character_id = id_WHIP;
        animation = object->apiobj.anim_packet;
        if (animation.blending != 0 && animation.blend_animation_b != 0x51 && animation.blend_animation_b != 0x55 &&
            animation.blend_animation_b != 0x56 && animation.blend_animation_b != 0x94)
            return;
    } else if (object->id == id_PENGUIN) {
        character_id = id_UMBRELLA;
        AnimPacket_MiniToFull(&object->mini_animation, &animation);
    } else {
        return;
    }
    if (character_id == -1)
        return;
    i32 current_animation = CurrentAnim(&object->apiobj.anim_packet);
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    CHARACTERANIM_s *configuration = static_cast<CHARACTERANIM_s *>(model->model_data_a[current_animation]);
    if (configuration != NULL && (configuration->misc_flags & 8) != 0)
        return;
    i32 locator = object->apiobj.character_data->game_character->extra_character_locator;
    if (locator == -1 || model->points_of_interest[locator] == NULL)
        return;
    i16 model_index = apicharsys->playermodelids[character_id];
    if (model_index == -1)
        return;
    CHARACTERMODEL_s *attachment_model = &apicharsys->models[model_index];
    NUMTX reflected;
    NUMTX matrix = joint_matrices[locator];
    NUMTX *reflection = NULL;
    if (object->field_0x1088 != 0 && object->field_0x1020 != 2000000.0f &&
        MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020, WORLD->current_level->unknown_0cc,
                         &reflected)) {
        reflection = &reflected;
    }
    GameDrawCharacterModel(attachment_model, &animation, &matrix, NULL, reflection, NULL, NULL, 0xffffffff);
}

void UpdateCharacterLoad() {
    const i32 menu_id = GetMenuID();
    if (menu_id == 13) {
        return;
    }
    if (menu_id >= 15 && menu_id <= 19) {
        return;
    }

    if (LOADEROFF != 0 || BGLOAD == 0 || waiting_for_character != -1 || hub_character_ready != -1 ||
        bgGetProcActive() != NULL || global_dlist_manager.ndisplay_lists > 0xfd) {
        return;
    }

    const i32 required_buffer =
        (static_cast<i32>((static_cast<f32>(CHARACTERBUFFERSIZE) / 7077888.0f) * 1048576.0f) + 0x3ff) & ~0x3ff;
    if (required_buffer > static_cast<i32>(characterbuffer_end.addr - characterbuffer_ptr.addr)) {
        return;
    }

    i32 candidates[0x154] __attribute__((aligned(16)));
    i32 candidate_count = 0;

    if (id_BARMAN != -1 && APICharacterLoaded(id_BARMAN) == NULL) {
        candidates[candidate_count++] = id_BARMAN;
    } else if (id_CANTINABAND != -1 && APICharacterLoaded(id_CANTINABAND) == NULL) {
        candidates[candidate_count++] = id_CANTINABAND;
    } else if (id_WEIRDO1 != -1 && APICharacterLoaded(id_WEIRDO1) == NULL) {
        candidates[candidate_count++] = id_WEIRDO1;
    } else if (id_WEIRDO2 != -1 && APICharacterLoaded(id_WEIRDO2) == NULL) {
        candidates[candidate_count++] = id_WEIRDO2;
    } else if (id_JABBA != -1 && APICharacterLoaded(id_JABBA) == NULL) {
        candidates[candidate_count++] = id_JABBA;
    } else if (id_MOSEISLEYCITIZEN != -1 && APICharacterLoaded(id_MOSEISLEYCITIZEN) == NULL) {
        candidates[candidate_count++] = id_MOSEISLEYCITIZEN;
    } else if (id_CANTINAALIEN != -1 && APICharacterLoaded(id_CANTINAALIEN) == NULL) {
        candidates[candidate_count++] = id_CANTINAALIEN;
    } else if (id_WOMPRAT != -1 && APICharacterLoaded(id_WOMPRAT) == NULL) {
        candidates[candidate_count++] = id_WOMPRAT;
    } else {
        i32 pack_index = 0;
        for (STOREPACK *pack = StorePack; pack != StorePack + 11; ++pack, ++pack_index) {
            if (g_lowEndLevelBehaviour != 0 && Hub_LowEnd_IconsInsteadOfModels != 0) {
                continue;
            }
            if (Store_IsPackUnlocked(pack_index) != 0 || pack->id == NULL) {
                continue;
            }

            const i32 id = *pack->id;
            if (id != -1 && APICharacterLoaded(id) == NULL) {
                candidates[candidate_count++] = id;
                break;
            }
        }

        if (candidate_count == 0) {
            for (i32 id = 0; id < CHARCOUNT; ++id) {
                if (id == id_DROIDEKA || (CDataList[id].model_flags & 0x04002000) != 0 ||
                    APICharacterLoaded(id) != NULL) {
                    continue;
                }
                if (static_cast<i32>(CDataList[id].field5_0x14) + apicharsys->loaded_animation_count >
                    apicharsys->animation_capacity) {
                    continue;
                }
                if (InCollectList_Index(id, NULL, 0) == -1 || Collection_Got(id) == 0) {
                    continue;
                }
                candidates[candidate_count++] = id;
            }
        }
    }

    if (candidate_count == 0) {
        return;
    }

    i32 candidate = 0;
    if (candidate_count != 1) {
        candidate = qrand() / ((0xffff / candidate_count) + 1);
    }
    waiting_for_character = candidates[candidate];
    WaitingForCharacterTime = 0.0f;
    bgPostRequest(LoadSingleCharacter, NULL, NULL, 0);
}

i32 ActivePlayerInRange(nuvec_s *position, float range_squared, float *distance_squared) {
    for (i32 i = 0; i < 8; i++) {
        if (Player[i] != NULL && Player[i]->apiobj.player_controlled) {
            const f32 distance = NuVecDistSqr(&Player[i]->apiobj.collision_position, position, NULL);
            if (distance < range_squared) {
                if (distance_squared != NULL) {
                    *distance_squared = distance;
                }
                return 1;
            }
        }
    }
    return 0;
}

i32 AvailableToPlayer(u32 character_flags, i32 weapon_action, i32 context, i32 require_all) {
    for (i32 i = 0; i < 8; i++) {
        GameObject_s *object = Player[i];
        if (object == NULL || object->apiobj.character_data == NULL)
            continue;
        if (character_flags != 0) {
            if ((object->apiobj.character_data->model_flags & character_flags) == character_flags) {
                if (require_all == 0)
                    return 1;
            } else if (require_all != 0) {
                continue;
            }
        }
        if (weapon_action != -1) {
            if (static_cast<i8>(object->apiobj.character_data->game_character->uses_weapon_action) == weapon_action) {
                if (require_all == 0)
                    return 1;
            } else if (require_all != 0) {
                continue;
            }
        }
        if (context == 0 || object->field_0x108e == context)
            return 1;
    }

    if (FreePlay != 0) {
        for (i32 index = 0; index < apicharsys->character_count; ++index) {
            i32 model = apicharsys->playermodelids[index];
            if (model == -1 || FreePlay == 0 || (apicharsys->models[model].flags & 1) == 0)
                continue;
            if (!((Game_CharacterSave != NULL && (Game_CharacterSave[index] & 1) != 0) ||
                  (static_cast<i32>(GCDataList[index].flags_090) < 0 && Cheats_CheckFlags(0x100) != 0)))
                continue;
            if (character_flags != 0) {
                if ((CDataList[index].model_flags & character_flags) == character_flags) {
                    if (require_all == 0)
                        return 1;
                } else if (require_all != 0) {
                    continue;
                }
            }
            if (weapon_action == -1 || static_cast<i8>(GCDataList[index].uses_weapon_action) == weapon_action)
                return 1;
        }
    }
    return 0;
}

GameObject_s *GetOtherActivePlayer(GameObject_s *object) {
    GameObject_s *other;
    if (Player[0] == object)
        other = Player[1];
    else if (Player[1] == object)
        other = Player[0];
    else
        return NULL;
    if (other != NULL && static_cast<i8>(other->apiobj.field_0x1f8) < 0)
        return other;
    return NULL;
}

// Shares the original translation-unit-local layer table with FixUpLayers.
u32 AdjustLayerBits(u32 mask, GameObject_s *object) {
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    u32 cape = 0;
    i32 cape_layer = data->cape_layer;
    if (cape_layer != -1)
        cape = 1u << cape_layer;
    u32 hair = 0;
    i32 hair_layer = data->hair_layer;
    if (hair_layer != -1)
        hair = 1u << hair_layer;
    if (object->field_0x108e != 0)
        mask &= ~hair;
    SUIT_s *suit = static_cast<SUIT_s *>(object->suit);
    if (suit != NULL && object->id == id_BATMAN) {
        if ((suit->flags & 4) != 0) {
            mask = (mask & ~cape) | SpecialLayer[0].mask;
            mask = (mask & ~SpecialLayer[3].mask) | SpecialLayer[5].mask;
            if ((mask & SpecialLayer[20].mask) != 0)
                mask = (mask & ~SpecialLayer[20].mask) | SpecialLayer[24].mask;
            else if ((mask & SpecialLayer[21].mask) != 0)
                mask = (mask & ~SpecialLayer[21].mask) | SpecialLayer[25].mask;
            mask = (mask & ~SpecialLayer[17].mask) | SpecialLayer[19].mask;
        } else if ((suit->flags & 2) != 0) {
            mask &= ~cape;
        } else if ((suit->flags & 8) != 0) {
            mask = (mask & ~cape) | SpecialLayer[1].mask | SpecialLayer[7].mask;
            mask = (mask & ~SpecialLayer[3].mask) | SpecialLayer[4].mask;
            if ((mask & SpecialLayer[20].mask) != 0)
                mask = (mask & ~SpecialLayer[20].mask) | SpecialLayer[22].mask;
            else if ((mask & SpecialLayer[21].mask) != 0)
                mask = (mask & ~SpecialLayer[21].mask) | SpecialLayer[23].mask;
            mask = (mask & ~SpecialLayer[17].mask) | SpecialLayer[18].mask;
            if ((mask & SpecialLayer[8].mask) != 0)
                mask = (mask & ~SpecialLayer[8].mask) | SpecialLayer[14].mask;
            else if ((mask & SpecialLayer[9].mask) != 0)
                mask = (mask & ~SpecialLayer[9].mask) | SpecialLayer[15].mask;
            else if ((mask & SpecialLayer[10].mask) != 0)
                mask = (mask & ~SpecialLayer[10].mask) | SpecialLayer[16].mask;
        } else if ((suit->flags & 1) != 0) {
            mask |= SpecialLayer[2].mask;
            if ((mask & SpecialLayer[8].mask) != 0)
                mask = (mask & ~SpecialLayer[8].mask) | SpecialLayer[11].mask;
            else if ((mask & SpecialLayer[9].mask) != 0)
                mask = (mask & ~SpecialLayer[9].mask) | SpecialLayer[12].mask;
            else if ((mask & SpecialLayer[10].mask) != 0)
                mask = (mask & ~SpecialLayer[10].mask) | SpecialLayer[13].mask;
        }
    } else if (suit != NULL && object->id == id_ROBIN) {
        if ((suit->flags & 0x10) != 0) {
            mask = (mask & ~(hair | cape)) | SpecialLayer[27].mask;
            if ((mask & SpecialLayer[30].mask) != 0)
                mask = (mask & ~SpecialLayer[30].mask) | SpecialLayer[39].mask;
            else if ((mask & SpecialLayer[31].mask) != 0)
                mask = (mask & ~SpecialLayer[31].mask) | SpecialLayer[40].mask;
            else if ((mask & SpecialLayer[32].mask) != 0)
                mask = (mask & ~SpecialLayer[32].mask) | SpecialLayer[41].mask;
        } else if ((suit->flags & 0x40) != 0) {
            mask = (mask & ~cape) | SpecialLayer[26].mask;
            if ((mask & SpecialLayer[30].mask) != 0)
                mask = (mask & ~SpecialLayer[30].mask) | SpecialLayer[36].mask;
            else if ((mask & SpecialLayer[31].mask) != 0)
                mask = (mask & ~SpecialLayer[31].mask) | SpecialLayer[37].mask;
            else if ((mask & SpecialLayer[32].mask) != 0)
                mask = (mask & ~SpecialLayer[32].mask) | SpecialLayer[38].mask;
        } else if ((suit->flags & 0x20) != 0) {
            mask = (mask & ~cape) | SpecialLayer[28].mask;
            if ((mask & SpecialLayer[30].mask) != 0)
                mask = (mask & ~SpecialLayer[30].mask) | SpecialLayer[33].mask;
            else if ((mask & SpecialLayer[31].mask) != 0)
                mask = (mask & ~SpecialLayer[31].mask) | SpecialLayer[34].mask;
            else if ((mask & SpecialLayer[32].mask) != 0)
                mask = (mask & ~SpecialLayer[32].mask) | SpecialLayer[35].mask;
        } else if ((suit->flags & 0x80) != 0) {
            mask = (mask & ~cape) | SpecialLayer[29].mask;
        }
    }
    if (object->id == id_BODYGUARD) {
        if (object->current_hp <= 1)
            mask &= ~0x10u;
    } else if (object->id == id_GEONOSIAN) {
        if ((object->field_0xefd & 2) != 0)
            mask |= 0x20;
        else
            mask |= 0x40;
    } else if (CharacterCustomiser != NULL && object->id == CharacterCustomiser->character_ids[0]) {
        if ((CharacterCustomiser->pieces[static_cast<u16>(Game.customizer[0].pieces[5])].layer_flags & 0x40) == 0)
            mask |= cape;
    } else if (CharacterCustomiser != NULL && object->id == CharacterCustomiser->character_ids[1]) {
        if ((CharacterCustomiser->pieces[static_cast<u16>(Game.customizer[1].pieces[5])].layer_flags & 0x40) == 0)
            mask |= cape;
    } else if (object->id == id_CHEWBACCA) {
        if (Cheat_IsOn(4) != 0)
            mask |= 0xc0;
        data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    }
    if (data->ride_layers_off != 0 && object->field_0xcc0 != NULL && object->field_0x7a5 == 0x3b)
        mask &= ~data->ride_layers_off;
    return mask;
}

void FixUpLayers() {
    for (i32 model_index = 0; model_index < apicharsys->loaded_model_count; ++model_index) {
        CHARACTERMODEL_s *model = &apicharsys->models[model_index];
        GAMECHARACTERDATA_s *character = &GCDataList[model->model_id];

        for (i32 layer_index = 0; layer_index < character->layer_count; ++layer_index) {
            GAMECHARACTERLAYER_s *layer = &character->layers[layer_index];
            layer->hierarchy_layer_index = NuHGobjGetLayerIndex(layer->name, model->hierarchy);
        }
    }

    SPECIAL_LAYER_s *layer = SpecialLayer;
    SPECIAL_LAYER_s *layer_end = SpecialLayer + sizeof(SpecialLayer) / sizeof(SpecialLayer[0]);
    for (; layer != layer_end; ++layer) {
        layer->mask = 0;
        if (layer->character_id != NULL && *layer->character_id != -1) {
            layer->mask = 1 << LayerFromName(&GCDataList[*layer->character_id], layer->name);
        }
    }
}

i32 ObjLookingWithLeftStick(GameObject_s *object) {
    if (object->character_id_0x7a5 == 0x4d) {
        return object->field_0x7a3 == 0 ? 2 : 1;
    }
    return object->character_id_0x7a5 == 0x52 ? 2 : 1;
}

i32 SuperWeirdo(GameObject_s *object) {
    if ((object->apiobj.flags_low & 0x80) != 0 && (Game.field_0x7c26[1] & 1) != 0 && CharacterCustomiser != NULL &&
        (object->id == CharacterCustomiser->character_ids[0] || object->id == CharacterCustomiser->character_ids[1])) {
        return 1;
    }
    return 0;
}

i32 Player_HasPurpleForce(GameObject_s *obj) {
    if (Cheat_IsOn(0x1c))
        return 1;
    return obj != NULL && obj->field_0xdec > 0.0f;
}

static void DrawWeapons(GameObject_s *object, i32 reflection, f32 weapon_scale) {
    NUVEC hilt_scale_vec;
    i32 models[4] = {-1, -1, -1, -1};
    i32 hilt = -1;
    i32 reflected_models[4] = {-1, -1, -1, -1};
    if (weapon_scale <= 0.0f) {
        return;
    }
    GAMECHARACTERDATA *data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
    if (data->field_0x94 & 0x200000) {
        return;
    }
    i32 sabre = 0;
    u16 rotation = 0;
    auto f0x1020 = object->field_0x1020;
    for (i32 hand = 0; hand < 4; ++hand) {
        data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
        const i32 joint = data->weapon_joints[hand];
        if (joint != -1 && object->apiobj.character_model->points_of_interest[joint] != NULL) {
            const i32 disguise = Cheat_IsOn(15);
            // Services may replace the character data; the reference reloads it.
            data = static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24);
            if (disguise && (data->field275_0x116 == 8 || data->field275_0x116 == 1)) {
                models[0] = 0x59;
            } else {
                models[0] = data->weapon_model;
            }
            if (models[0] != -1) {
                if (object->id == id_JANGOFETT) {
                    rotation = 0xd1c8;
                } else if (models[0] == 0x65 || models[0] == 0x67 || models[0] == 0x69 || models[0] == 0x6b) {
                    i32 red, green, blue, purple;
                    if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25)) {
                        red = 1;
                        green = blue = purple = 0;
                    } else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object)) {
                        purple = 1;
                        red = green = blue = 0;
                    } else {
                        red = models[0] == 0x65;
                        green = models[0] == 0x67;
                        blue = models[0] == 0x69;
                        purple = models[0] == 0x6b;
                    }
                    DrawWeapon_SetSabreObjects(object, red, green, blue, purple, models, &hilt);
                    sabre = true;
                }
            } else {
                if ((object->apiobj.character_data->model_flags & 0x90) == 0x80) {
                    models[0] = 0xd;
                } else {
                    i32 red = 0, green = 0, blue = 0, purple = 0;
                    if (object->id == id_BOB) {
                        if (object->field_0xefd & 2)
                            green = 1;
                        else
                            blue = 1;
                    } else if (AnakinGreenSabre(object)) {
                        green = 1;
                    } else {
                        const u8 color =
                            static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->field_0x117;
                        if (color == 0)
                            red = 1;
                        else if (color == 1)
                            green = 1;
                        else if (color == 2)
                            blue = 1;
                        else if (color == 3)
                            purple = 1;
                    }
                    if (red || green || blue || purple) {
                        if (object->apiobj.field_0x27c != -1 && Cheat_IsOn(25)) {
                            red = 1;
                            green = blue = purple = 0;
                        } else if (object->apiobj.field_0x27c != -1 && Player_HasPurpleForce(object)) {
                            purple = 1;
                            red = green = blue = 0;
                        } else if (green && object->id == id_GRIEVOUS && (hand == 0 || hand == 3)) {
                            blue = 1;
                            green = 0;
                        }
                        DrawWeapon_SetSabreObjects(object, red, green, blue, purple, models, &hilt);
                        sabre = true;
                    }
                }
            }
            i32 count = 0;
            for (i32 part = 0; part < 4; ++part) {
                if (models[part] != -1) {
                    if (part != 3) {
                        ++count;
                    }
                    reflected_models[part] = LevelObject_GetReflection(models[part]);
                }
            }
            if (count != 0) {
                NUMTX blade_matrix = object->joint_matrices[joint];
                NUMTX hilt_matrix;
                NUVEC scale = {weapon_scale, weapon_scale, weapon_scale};
                if (hilt != -1) {

                    hilt_scale_vec.x = weapon_scale + weapon_scale;
                    hilt_matrix = blade_matrix;
                    if (hilt_scale_vec.x > 1.0f) {
                        hilt_scale_vec.x = 1.0f;
                    }
                    hilt_scale_vec.z = hilt_scale_vec.x;
                    hilt_scale_vec.y = hilt_scale_vec.x;
                    NuMtxPreScale(&hilt_matrix, &hilt_scale_vec);
                }
                if (rotation != 0) {
                    NuMtxPreRotateX(&blade_matrix, rotation);
                }
                NuMtxPreScale(&blade_matrix, &scale);
                u8 saved_matrix[sizeof(NUMTX)];
                memcpy(saved_matrix, &blade_matrix, sizeof(blade_matrix));
                for (i32 side = 0;; ++side) {
                    for (i32 part = 0; part < 4; ++part) {
                        if (!(side && models[part] == hilt) && models[part] != -1) {
                            if (part == 3) {
                                blade_matrix.m30 += object->weapon_trail_offset.x;
                                blade_matrix.m31 += object->weapon_trail_offset.y;
                                blade_matrix.m32 += object->weapon_trail_offset.z;
                            }
                            NUMTX *matrix = models[part] == hilt ? &hilt_matrix : &blade_matrix;
                            Draw3DObjectMtx(NULL, models[part], matrix);
                            if (reflection) {
                                i32 model = reflected_models[part];
                                if (model == -1 || !WORLD->lev_objs[model].active) {
                                    model = models[part];
                                }
                                NUMTX reflected;
                                if (MatrixReflection(matrix, object->field_0x1087, f0x1020,
                                                     WORLD->current_level->unknown_0cc, &reflected)) {
                                    NuRndrStartReflectionRender(0);
                                    Draw3DObjectMtx(NULL, model, &reflected);
                                    NuRndrEndReflectionRender();
                                }
                            }
                        }
                    }
                    if (side || object->id != id_DARTHMAUL || object->apiobj.field_0x27c == -1 ||
                        (!Cheat_IsOn(25) && (object->apiobj.field_0x27c == -1 || !Player_HasPurpleForce(object)))) {
                        break;
                    }
                    memcpy(&blade_matrix, saved_matrix, sizeof(blade_matrix));
                    NUVEC reverse = {1.0f, -1.0f, 1.0f};
                    NuMtxPreScale(&blade_matrix, &reverse);
                }
            }
        }
        if (sabre && object->id != id_GRIEVOUS) {
            return;
        }
    }
}

static void DrawParaphernalia(GameObject_s *object) {
    if (draw_para == 0)
        return;
    // Keep the effect matrices alive throughout the rendering callbacks.
    NUMTX matrix, reflected, special_reflected;
    NUMTX communicate_matrix, communicate_reflected;
    NUMTX thrust_matrix, thrust_reflected;
    NUMTX placement_matrix, placement_reflected;
    NUMTX removal_matrix, removal_reflected;
    NUMTX shield_matrix, shield_reflected;
    NUMTX hand_matrix, hand_reflected;
    NUMTX attachment_matrices[16];
    // Function-scope temporaries: the original keeps these in their own stack slots instead of sharing them with
    // the per-block scaling vectors.
    NUVEC position;
    NUANGVEC rotation;
    NUVEC effect_vector;
    GameObject_s *carried = object->field_0xcc0;
    if (!(carried != NULL && object->field_0x7a5 != 0x3b &&
          (object->apiobj.character_data->game_character->flags_098[0] & 0x40) == 0)) {
        if ((object->field_0xe24 & 1) && object->field_0x780 != NULL) {
            GameObject_s *passenger = static_cast<GameObject_s *>(object->field_0x780);
            i32 locator = object->apiobj.character_data->game_character->weapon_joints[0];
            if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
                matrix = object->joint_matrices[locator];
            } else {
                rotation.x = -0x4000;
                rotation.y = object->apiobj.field_0x276;
                rotation.z = 0;
                NuMtxSetRotationXYVU0(&matrix, &rotation);
                matrix.m30 = object->apiobj.position.x +
                             NuTrigTable[object->apiobj.field_0x276 / 2] * object->apiobj.field_0x1dc * 1.75f;
                matrix.m31 = object->apiobj.collision_min.y +
                             (object->apiobj.collision_max.y - object->apiobj.collision_min.y) * 0.6f;
                matrix.m32 = object->apiobj.position.z +
                             NU_COS_LUT(object->apiobj.field_0x276) *
                                 object->apiobj.field_0x1dc * 1.75f;
            }
            GameDrawCharacterModel(passenger->apiobj.character_model, &passenger->apiobj.anim_packet, &matrix, NULL,
                                   NULL, NULL, NULL,
                                   passenger->apiobj.character_data->game_character->layer_mask_medium);
        }
    } else {
        carried->field_0x1088 = object->field_0x1088;
        carried->field_0x1087 = object->field_0x1087;
        carried->field_0x1020 = object->field_0x1020;
        i32 locator = object->apiobj.character_data->game_character->ride_locator;
        if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
            matrix = object->joint_matrices[locator];
        } else {
            // The original fallback does not initialize its temporary Z component.
            // Zero gives this otherwise undefined path a deterministic position.
            position.x = 0.0f;
            position.y = object->apiobj.field_0x1e0;
            position.z = 0.0f;
            NuVecMtxRotate(&position, &position, &object->apiobj.field_0xb8);
            NuVecAdd(&position, &position, &object->apiobj.collision_position);
            matrix = object->apiobj.field_0xb8;
            matrix.m30 = position.x;
            matrix.m31 = position.y;
            matrix.m32 = position.z;
        }
        NUMTX *reflection = NULL;
        if (carried->field_0x1087 && carried->field_0x1020 != 2000000.0f &&
            static_cast<u32>(static_cast<u8>(WORLD->current_level->reflection_range)) > object->ai_update_distance &&
            MatrixReflection(&matrix, carried->field_0x1087, carried->field_0x1020, WORLD->current_level->unknown_0cc,
                             &reflected))
            reflection = &reflected;
        GAMECHARACTERDATA_s *carried_config = carried->apiobj.character_data->game_character;
        u32 layers = carried->id == id_ANAKINJEDISCARRED ? carried_config->layer_mask_special
                                                         : carried_config->layer_mask_medium;
        layers = AdjustLayerBits(layers, carried);
        if (GameDrawCharacterModel(carried->apiobj.character_model, &carried->apiobj.anim_packet, &matrix, NULL,
                                   reflection, attachment_matrices, NULL, layers)) {
            if (carried->field_0x108e)
                DrawObjectOnCharacter(WORLD, carried, carried->field_0x108e + 0xf9, NULL,
                                      carried->apiobj.character_data->game_character->helmet_locator, -1,
                                      attachment_matrices, carried->field_0x1088, layers, NULL, NULL, 1.0f, 1.0f);
            if (Cheat_IsOn(2))
                DrawObjectOnCharacter(WORLD, carried, 0xe7, NULL,
                                      carried->apiobj.character_data->game_character->head_locator, -1,
                                      attachment_matrices, carried->field_0x1088, layers, NULL, NULL, 1.0f, 1.0f);
            DrawCharacterAttachments(carried, attachment_matrices);
            if (carried->id == id_WEIRDO1 || carried->id == id_WEIRDO2)
                Customiser_DrawAccessories(CharacterCustomiser, carried, attachment_matrices);
        }
    }
    GAMECHARACTERDATA_s *config = object->apiobj.character_data->game_character;
    NUMTX *joints = object->joint_matrices;
    if ((config->flags_094[0] & 1) != 0) {
        if (BonusArea && VehicleArea) {
            CharMiniKit_Draw(object->id, &object->apiobj.field_0xb8, object->field_0x1087, object->field_0x1020,
                             WORLD->current_level->unknown_0cc);
        }
    } else {
        if (object->apiobj.character_data->flags & 1) {
            i32 locator = config->thingy_locator;
            matrix = locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL &&
                             object->apiobj.model_draw_result
                         ? joints[locator]
                         : object->apiobj.field_0xb8;
            reflected = object->apiobj.field_0x138;
            CharScene_Draw(WORLD, object->id, &matrix, object->field_0x1088 ? &reflected : NULL);
        } else if (object->id == id_CATAPULT && (object->field_0x7a5 != 0x0a || (object->context_flags & 0x40) == 0 ||
                                                 object->quick_shoot_bolt_id != -1 ||
                                                 (object->context_animation_timer < 0.5f &&
                                                  PickupFlickerFrame % PickUpFlickerFrames < PickUpFlickerTest))) {
            dco_prerotatez = 0xc000;
            DrawObjectOnCharacter(WORLD, object, 0xf0, NULL,
                                  object->apiobj.character_data->game_character->weapon_shoot_joints[0], -1, joints, 0,
                                  object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
        }
    }
    config = object->apiobj.character_data->game_character;
    if (config->uses_weapon_action == 1 && AnimPlaying(&object->apiobj.anim_packet, 0x60, 1, 1)) {
        DrawObjectOnCharacter(WORLD, object, 9, NULL, object->apiobj.character_data->game_character->weapon_joints[0],
                              -1, joints, object->field_0x1088, object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
    } else if (object->weapon_scale > 0.0f) {
        DrawWeapons(object, object->field_0x1088, object->weapon_scale);
        if (object->timer_d50 > 0.0f) {
            dco_locatorposonly = 1;
            DrawObjectOnCharacter(WORLD, object, 0x35, NULL,
                                  object->apiobj.character_data->game_character->weapon_shoot_joints[0], -1, joints,
                                  object->field_0x1088, object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
        }
    } else {
        f32 scale = 0.0f;
        f32 *weapon_time = NULL;
        if (object->apiobj.character_data->game_character->uses_weapon_action == 9 &&
            (weapon_time = AnimPlaying(&object->apiobj.anim_packet, 0x5e, 1, 0)) != NULL) {
            f32 frame = *weapon_time;
            if (frame >= 18.0f && frame <= 65.0f) {
                if (frame <= 27.0f)
                    scale = (frame - 18.0f) / 9.0f;
                else if (frame < 55.0f)
                    scale = 1.0f;
                else
                    scale = 1.0f - (frame - 55.0f) / 10.0f;
            }
        }
        DrawWeapons(object, object->field_0x1088, scale);
    }
    if (object->field_0xd24 > 0.0f || object->timer_d28 > 0.0f) {
        ResetShadowMapRendering();
        GAMECHARACTERDATA_s *shield_config = object->apiobj.character_data->game_character;
        const u8 render_reflection = object->field_0x1088;
        f32 scale;
        bool show;
        if (object->field_0xe37 != 0 || shield_config->field_0xf5 == 0 || !(object->timer_d28 > 0.0f)) {
            scale = object->field_0xd24;
            show = !(scale <= 0.0f);
        } else {
            scale = 1.0f;
            show = (GameTimer.update_count & 3) < 2;
        }
        if (show) {
            i32 locator = shield_config->shield_locator;
            NUVEC scaling = {scale, scale, scale};
            NuMtxSetScale(&shield_matrix, &scaling);
            if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL)
                NuMtxTranslate(&shield_matrix, reinterpret_cast<NUVEC *>(&joints[locator].m30));
            else
                NuMtxTranslate(&shield_matrix, &object->apiobj.collision_position);
            Draw3DObjectMtx(NULL, 0x6f + (object->timer_d28 > 0.0f), &shield_matrix);
            if (render_reflection && MatrixReflection(&shield_matrix, object->field_0x1087, object->field_0x1020,
                                                      WORLD->current_level->unknown_0cc, &shield_reflected)) {
                NuRndrStartReflectionRender(0);
                Draw3DObjectMtx(NULL, 0x92 + (object->timer_d28 > 0.0f), &shield_reflected);
                NuRndrEndReflectionRender();
            }
        }
        EnableShadowMapRendering(0);
    }
    if (object->communicate_blend > 0.0f) {
        const u8 render_reflection = object->field_0x1088;
        GAMECHARACTERDATA_s *communicate_config = object->apiobj.character_data->game_character;
        const i32 locator = communicate_config->thingy_locator;
        if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
            i32 model_id;
            if (object->apiobj.character_data->model_flags & 0x40) {
                f32 scale = object->communicate_blend * object->apiobj.field_0xa8;
                NUVEC scaling = {scale, scale, scale};
                NuMtxSetScale(&communicate_matrix, &scaling);
                NuMtxRotateY(&communicate_matrix, static_cast<u16>(object->apiobj.field_0x276 + 0x8000));
                communicate_matrix.m31 = joints[locator].m31;
                communicate_matrix.m30 = joints[locator].m30;
                communicate_matrix.m32 = joints[locator].m32;
                if (object->apiobj.field_0x27f == 9 && object->apiobj.water_height > communicate_matrix.m31)
                    communicate_matrix.m31 = object->apiobj.water_height;
                model_id = 0x19;
            } else {
                model_id = (communicate_config->flags_094[0] & 0x80) ? 0x0e : -1;
                communicate_matrix = joints[locator];
                if (object->communicate_blend != 1.0f) {
                    effect_vector.x = effect_vector.y = effect_vector.z = object->communicate_blend;
                    NuMtxPreScale(&communicate_matrix, &effect_vector);
                }
            }
            if (model_id != -1 && WORLD->lev_objs[model_id].active) {
                NuSpecialDrawAt(&WORLD->lev_objs[model_id].special, &communicate_matrix);
                i32 reflection_id = LevelObject_GetReflection(model_id);
                if (reflection_id != -1 && WORLD->lev_objs[reflection_id].active && render_reflection &&
                    MatrixReflection(&communicate_matrix, object->field_0x1087, object->field_0x1020,
                                     WORLD->current_level->unknown_0cc, &communicate_reflected)) {
                    NuRndrStartReflectionRender(0);
                    NuSpecialDrawAt(&WORLD->lev_objs[reflection_id].special, &communicate_reflected);
                    NuRndrEndReflectionRender();
                }
            }
        }
    }
    if (object->field_0x7a5 == 0x22) {
        if ((WORLD->area != NULL && WORLD->area == HUB_ADATA) || WORLD->current_level == BLOCKADERUNNERB_LDATA ||
            WORLD->current_level == DOOKUC_LDATA) {
            effect_vector.x = object->apiobj.collision_position.x;
            effect_vector.y = object->character_bottom * object->apiobj.field_0xa8 + object->apiobj.position.y +
                              (object->character_top - object->character_bottom) * 0.5f * object->apiobj.field_0xa8;
            effect_vector.z = object->apiobj.collision_position.z;
            f32 radius = NuFmax(object->apiobj.field_0x1dc, object->apiobj.field_0x1e0) * 1.75f;
            f32 alpha;
            if (object->field_0x768 < 0.2f) {
                alpha = object->field_0x768 / 0.2f;
            } else if (object->context_animation_timer < 0.2f) {
                alpha = object->context_animation_timer / 0.2f;
            } else {
                alpha = 1.0f;
            }
            DrawForceGlowSprite(&effect_vector, radius, 0xdf, alpha, object);
        }
    } else if (object->field_0xd80 > 0.0f && object->field_0xd8c > 0.0f &&
               WORLD->lev_objs[object->field_0xe1e].active != 0 &&
               (object->apiobj.character_data->player_config->flags_090 & 0x400) == 0) {
        DrawForceGlowSprite(&object->force_glow_position, object->field_0xd8c, object->field_0xe1e,
                            object->field_0xd80 / FORCEGLOWTIME, object);
    }
    if (Cheat_IsOn(2))
        DrawObjectOnCharacter(WORLD, object, 0xe7, NULL, object->apiobj.character_data->game_character->head_locator,
                              -1, joints, object->field_0x1088, object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
    if (object->field_0x108e) {
        i32 locator = object->apiobj.character_data->game_character->helmet_locator;
        if ((object->id == id_CHEWBACCA || object->id == id_WOOKIEE) && object->field_0x108e != 5 &&
            object->field_0x108e != 6)
            locator = 9;
        DrawObjectOnCharacter(WORLD, object, object->field_0x108e + 0xf9, NULL, locator, -1, joints,
                              object->field_0x1088, object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
    }
    if (object->apiobj.model_draw_result && object->apiobj.character_data->game_character->thrust_locators &&
        object->thrust_effect_scale > 0.0f) {
        const u8 render_reflection = object->field_0x1088;
        if (WORLD->lev_objs[0x77].active && WORLD->lev_objs[0x78].active) {
            i32 effect_index = 0;
            for (i32 locator = 0; locator < 16; ++locator) {
                if ((object->apiobj.character_data->game_character->thrust_locators & (1 << locator)) == 0 ||
                    object->apiobj.character_model->points_of_interest[locator] == NULL)
                    continue;
                NUVEC scaling;
                scaling.x =
                    object->thrust_effect_scale +
                    ((object->reserved_e27[effect_index++] / 255.0f - 0.5f) * 0.5f) * object->thrust_effect_scale;
                if (!(scaling.x <= 0.0f)) {
                    scaling.y = scaling.z = scaling.x;
                    thrust_matrix = joints[locator];
                    NuMtxPreScale(&thrust_matrix, &scaling);
                    Draw3DObjectMtx(NULL, 0x77, &thrust_matrix);
                    Draw3DObjectMtx(NULL, 0x78, &thrust_matrix);
                    if (render_reflection &&
                        MatrixReflection(&thrust_matrix, object->field_0x1087, object->field_0x1020,
                                         WORLD->current_level->unknown_0cc, &thrust_reflected)) {
                        i32 first = LevelObject_GetReflection(0x77);
                        if (!WORLD->lev_objs[first].active)
                            first = 0x77;
                        i32 second = LevelObject_GetReflection(0x78);
                        if (!WORLD->lev_objs[second].active)
                            second = 0x77;
                        NuRndrStartReflectionRender(0);
                        Draw3DObjectMtx(NULL, first, &thrust_matrix);
                        Draw3DObjectMtx(NULL, second, &thrust_matrix);
                        NuRndrEndReflectionRender();
                    }
                }
            }
        }
    }
    if (object->field_0x7a5 == 0x2d)
        GizDrawBuildItPiece(object, object->field_0x1088);
    if (object->field_0x7a5 == 0x48) {
        if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
            object->field_0x7a3 == 0) {
            const u8 render_reflection = object->field_0x1088;
            const i32 locator = object->apiobj.character_data->game_character->place_locator;
            f32 *placement_time;
            if (WORLD->lev_objs[0xec].active && locator != -1 &&
                object->apiobj.character_model->points_of_interest[locator] != NULL &&
                (placement_time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0)) != NULL) {
                f32 animation_end =
                    NuAnimEndFrame(object->apiobj.character_model->model_data_b[object->context_animation]);
                f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                if (!(start >= animation_end)) {
                    if (start < 1.0f)
                        start = 1.0f;
                    if (!(start > *placement_time)) {
                        f32 finish = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
                        if (!(start >= finish)) {
                            f32 scale = 1.0f;
                            if (!(animation_end < finish))
                                animation_end = finish;
                            if (animation_end > *placement_time)
                                scale = (*placement_time - start) / (animation_end - start);
                            NUVEC scaling = {scale, scale, scale};
                            placement_matrix = joints[locator];
                            NuMtxPreRotateY(&placement_matrix, object->takeover_start_angle);
                            NuMtxPreScale(&placement_matrix, &scaling);
                            NuSpecialDrawAt(&WORLD->lev_objs[0xec].special, &placement_matrix);
                            if (WORLD->lev_objs[0xed].active)
                                NuSpecialDrawAt(&WORLD->lev_objs[0xed].special, &placement_matrix);
                            if (render_reflection &&
                                MatrixReflection(&placement_matrix, object->field_0x1087, object->field_0x1020,
                                                 WORLD->current_level->unknown_0cc, &placement_reflected)) {
                                NuSpecialDrawAt(&WORLD->lev_objs[0xec].special, &placement_reflected);
                                if (WORLD->lev_objs[0xed].active)
                                    NuSpecialDrawAt(&WORLD->lev_objs[0xed].special, &placement_matrix);
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (object->field_0x7a5 == 0x49) {
            if (object->apiobj.character_model->model_data_b[object->context_animation] != NULL &&
                object->field_0x7a3 != 0) {
                const u8 render_reflection = object->field_0x1088;
                const i32 locator = object->apiobj.character_data->game_character->place_locator;
                f32 *placement_time;
                if (WORLD->lev_objs[0xec].active && locator != -1 &&
                    object->apiobj.character_model->points_of_interest[locator] != NULL &&
                    (placement_time = AnimPlaying(&object->apiobj.anim_packet, object->context_animation, 1, 0)) !=
                        NULL) {
                    f32 animation_end =
                        NuAnimEndFrame(object->apiobj.character_model->model_data_b[object->context_animation]);
                    f32 start = AnimListFrame(object->apiobj.character_model, object->context_animation, 0);
                    if (!(start >= animation_end)) {
                        if (start < 1.0f)
                            start = 1.0f;
                        if (!(start > *placement_time)) {
                            f32 finish = AnimListFrame(object->apiobj.character_model, object->context_animation, 1);
                            if (!(start >= finish)) {
                                f32 scale = 1.0f;
                                f32 last = AnimListFrame(object->apiobj.character_model, object->context_animation, 2);
                                if (last < finish)
                                    last = finish;
                                f32 decay_start = animation_end < finish ? animation_end : finish;
                                f32 decay_end = animation_end < last ? animation_end : last;
                                bool show = true;
                                if (!(decay_start >= *placement_time)) {
                                    if (decay_end > *placement_time && decay_end > decay_start)
                                        scale = 1.0f - (*placement_time - decay_start) / (decay_end - decay_start);
                                    else
                                        show = false;
                                }
                                if (show) {
                                    NUVEC scaling = {scale, scale, scale};
                                    removal_matrix = joints[locator];
                                    NuMtxPreRotateY(&removal_matrix, object->takeover_start_angle);
                                    NuMtxPreScale(&removal_matrix, &scaling);
                                    NuSpecialDrawAt(&WORLD->lev_objs[0xec].special, &removal_matrix);
                                    if (WORLD->lev_objs[0xed].active)
                                        NuSpecialDrawAt(&WORLD->lev_objs[0xed].special, &removal_matrix);
                                    if (render_reflection &&
                                        MatrixReflection(&removal_matrix, object->field_0x1087, object->field_0x1020,
                                                         WORLD->current_level->unknown_0cc, &removal_reflected)) {
                                        NuSpecialDrawAt(&WORLD->lev_objs[0xec].special, &removal_reflected);
                                        if (WORLD->lev_objs[0xed].active)
                                            NuSpecialDrawAt(&WORLD->lev_objs[0xed].special, &removal_matrix);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (object->field_0xe22 & 0x40) {
        dco_prerotatez = 0x4000;
        const u32 layers = object->field_0x1054;
        const u8 render_reflection = object->field_0x1088;
        const i32 locator = object->apiobj.character_data->game_character->throw_locator;
        const i32 model_id = Batarang_GetObjectFromCharID(object->id);
        DrawObjectOnCharacter(WORLD, object, model_id, NULL, locator, -1, joints, render_reflection, layers, NULL, NULL,
                              1.0f, 1.0f);
    }
    if (object->field_0x7a5 == 0x46)
        Grapple_DrawLine(object);
    if ((object->field_0xe22 & 0x80) || object->field_0x7a5 == 0x58)
        SuperCarry_DrawObject(object);
    DrawCharacterAttachments(object, joints);
    if (object->field_0x7a5 == 0x5d) {
        const u8 render_reflection = object->field_0x1088;
        if (WORLD->lev_objs[0xf7].active) {
            const i32 locator = object->apiobj.character_data->game_character->hand_locators[0];
            if (locator != -1 && object->apiobj.character_model->points_of_interest[locator] != NULL) {
                NuMtxSetTranslation(&hand_matrix, reinterpret_cast<NUVEC *>(&joints[locator].m30));
                Draw3DObjectMtx(NULL, 0xf7, &hand_matrix);
                if (render_reflection && MatrixReflection(&hand_matrix, object->field_0x1087, object->field_0x1020,
                                                          WORLD->current_level->unknown_0cc, &hand_reflected)) {
                    NuRndrStartReflectionRender(0);
                    Draw3DObjectMtx(NULL, 0xf7, &hand_reflected);
                    NuRndrEndReflectionRender();
                }
            }
        }
    }
    if (object->torpedo != NULL && object->torpedo->count != 0)
        DrawTorpedos(object);
    config = object->apiobj.character_data->game_character;
    if (config->uses_weapon_action == 0) {
        Customiser_DrawAccessories(CharacterCustomiser, object, NULL);
        config = object->apiobj.character_data->game_character;
    }
    if ((config->flags_090 & 0x01000000) && object->field_0xd80 > 0.0f && object->field_0xd8c > 0.0f)
        Transform_DrawTarget(&object->force_glow_position,
                             (1.4f + 0.20000004768371582f * object->field_0xd80) * object->field_0xd8c,
                             0.4f + 0.6f * object->field_0xd80);
    config = object->apiobj.character_data->game_character;
    f32 *special_time = NULL;
    if (config->uses_weapon_action == 1) {
        i32 locator = config->thingy_locator;
        if (locator == -1 || object->apiobj.character_model->points_of_interest[locator] == NULL ||
            (special_time = AnimPlaying(&object->apiobj.anim_packet, 0x6b, 1, 1)) == NULL)
            return;
        f32 frame = *special_time;
        if ((frame >= 70.0f && frame <= 210.0f) || (frame >= 310.0f && frame <= 433.0f)) {
            matrix = joints[locator];
            NuMtxPreRotateZ(&matrix, -0x4000);
            NuSpecialDrawAt(&WORLD->lev_objs[0x0a].special, &matrix);
            if (object->field_0x1088 && MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                         WORLD->current_level->unknown_0cc, &special_reflected))
                NuSpecialDrawAt(&WORLD->lev_objs[0x0a].special, &special_reflected);
        } else {
            f32 distance = 1.0f;
            i32 chosen = -1;
            for (i32 i = 10; i < 18; ++i) {
                if (NuSpecialExistsFn(&LevHSpecial[i])) {
                    f32 next =
                        NuVecDistSqr(&object->apiobj.collision_position, NuSpecialGetDrawPos(&LevHSpecial[i]), NULL);
                    if (next < distance) {
                        distance = next;
                        chosen = i;
                    }
                }
            }
            if (chosen != -1)
                NuSpecialDrawAt(&WORLD->lev_objs[0x0a].special, NuSpecialGetDrawMtx(&LevHSpecial[chosen]));
        }
    } else if (object->id == id_TWOFACE && (special_time = AnimPlaying(&object->apiobj.anim_packet, 1, 0, 0)) != NULL) {
        f32 start = AnimListFrame(object->apiobj.character_model, 1, 0);
        if (start >= 1.0f) {
            f32 finish = AnimListFrame(object->apiobj.character_model, 1, 1);
            f32 frame = *special_time;
            if (finish >= start && frame > start && frame < finish) {
                f32 fraction = (frame - start) / (finish - start);
                f32 scale;
                if (fraction < 0.1f)
                    scale = fraction / 0.1f;
                else if (fraction < 0.9f)
                    scale = 1.0f;
                else
                    scale = 1.0f - (fraction - 0.9f) / 0.1f;
                NUVEC scaling = {scale, scale, scale};
                NuMtxSetScale(&matrix, &scaling);
                NuMtxRotateX(&matrix, static_cast<u16>(static_cast<i32>(-65536.0f * fraction)));
                NuMtxRotateY(&matrix, object->apiobj.field_0x276);
                NuMtxTranslate(&matrix, reinterpret_cast<NUVEC *>(&joints[0].m30));
                matrix.m31 += 0.2f * NuTrigTable[(static_cast<i32>(fraction * 32768.0f) >> 1) & 0x7fff];
                DrawObjectOnCharacter(WORLD, object, 0xf9, NULL, 0, -1, &matrix, object->field_0x1088,
                                      object->field_0x1054, NULL, NULL, 1.0f, 1.0f);
            }
        }
    }
}

void DrawGameObjectsDraw(i32) {
    extern f32 FORCEGLOWTIME;
    EnableShadowMapRendering(0);

    GameObject_s *object = Obj;
    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index, ++object) {

        if ((object->apiobj.field_0x1f4 & 0x800) != 0) {
            if (object->field_0xcc0 != NULL && object->field_0x7a5 == 0x3b) {
                object->apiobj.model_draw_result = object->field_0xcc0->apiobj.model_draw_result;
            }
            continue;
        }

        NUMTX *tertiary_matrix = object->field_0x1088 != 0 ? &object->apiobj.field_0x138 : NULL;
        NUMTX *secondary_matrix = (object->field_0xefe & 2) != 0 ? &object->apiobj.field_0xf8 : NULL;
        const u8 drawn = static_cast<u8>(GameDrawCharacterModel(
            object->apiobj.character_model, &object->apiobj.anim_packet, &object->apiobj.field_0xb8, secondary_matrix,
            tertiary_matrix, &object->field_0x7f4, object, object->field_0x1054));

        object->apiobj.model_draw_result = static_cast<u8>(drawn);
        object->field_0xe24 =
            static_cast<u8>((object->field_0xe24 & ~8) | ((drawcharactermodel_locatorsupdated & 1) << 3));

        if (drawn != 0) {
            DrawParaphernalia(object);
        } else if (((object->field_0xe21 & 4) != 0 || object->character_context == 8) && object->field_0xd80 > 0.0f &&
                   object->field_0xd8c > 0.0f && WORLD->lev_objs[object->field_0xe1e].active != 0 &&
                   (object->apiobj.character_data->game_character->flags_090 & 0x400) == 0) {
            DrawForceGlowSprite(&object->force_glow_position, object->field_0xd8c, object->field_0xe1e,
                                object->field_0xd80 / FORCEGLOWTIME, object);
        }

        if (object->field_0x10b8 != NULL) {
            DrawSnakeBody(object);
        }

        object->apiobj.field_0x288 = 1;
        if (object->apiobj.model_draw_result != 0) {
            object->field_0xefe |= 4;
        }

        if (Paused == 0 && object->apiobj.character_data->draw_fn != NULL) {
            object->apiobj.character_data->draw_fn(object);
        }
    }

    ResetShadowMapRendering();
}

void DrawGameObjects() {
    const f32 saved_far_clip = global_camera.unknown_64;
    const f32 object_far_clip = VehicleArea != 0 ? 50.0f : 20.0f;
    if (!(global_camera.unknown_64 < object_far_clip)) {
        global_camera.unknown_64 = object_far_clip;
    }

    NuCameraSet(&global_camera);
    DrawGameObjectsDraw(0);

    global_camera.unknown_64 = saved_far_clip;
    NuCameraSet(&global_camera);
}

