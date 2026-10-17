// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "gameapi/gui/apimenu.h"
#include "gamelib/crc/crc.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nustring.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/render/fx.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/world/area.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/numtx_inline.h"

#include <string.h>
#include <stdio.h>
extern "C" void NuIOS_RecordFlurryEvent(char *);
void SetLevelLights(void *, f32);
void DrawItem(nuhspecial_s *, nuvec_s *, float, float, float, u16, u16, u16);
i32 Draw3DObject(WORLDINFO_s *, i32, nuvec_s *, u16, u16, u16, float, float, float, i32);
void *AddGameMessage(char *, NUVEC *, f32, NUVEC *, f32, u8, u8, u8, u32, f32);
extern f32 HUB_EPISODETITLEY;
extern f32 HUB_EPISODESUBTITLEY;
i32 AddToCollection(i32);
extern GAMESAVE_s TempGame;
extern i32 hub_forceshopsave;
i32 POINTS_PER_HINT;
// Shop state is file-local, matching the original _ZL symbols.
static f32 pickedbing;
static i32 charcheatix;
static i32 extracheatix;
static f32 cheattimer;
static f32 codebigscale[6];
static f32 codeshelfscale[6];
static f32 codemenuscale[6];
i32 codevalid;
i32 codechar;
char codechars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

shopitem_s TopShelf[6] = {};
shopitem_s *CharItems = NULL;
shopitem_s *HintItems = NULL;
shopitem_s *ExtraItems = NULL;
shopitem_s *CodeItems = NULL;
shopitem_s *BrickItems = NULL;
shopitem_s CutItems[128] = {};
nuhspecial_s extrasils[44] = {};
nuhspecial_s atoz0to9icon[36] = {};

u32 codelist[144] = {};
i32 SHOPCHARCOUNT = 0;
i32 SHOPHINTCOUNT = 0;
i32 SHOPEXTRACOUNT = 0;
i32 CutScenePlayCount = 0;
i16 HintTab[24] = {-1};

i32 CharShelfIds[7] = {};
i32 HintShelfIds[7] = {};
i32 ExtraShelfIds[7] = {};
i32 BrickShelfIds[7] = {};
i32 CutShelfIds[7] = {};
NUVEC CharCurPos[7] = {};
NUVEC HintCurPos[7] = {};
NUVEC ExtraCurPos[7] = {};
NUVEC BrickCurPos[7] = {};
NUVEC CutCurPos[7] = {};

static NUGSPLINE *splshelf = NULL;
static NUGSPLINE *splcharshelf = NULL;
static NUGSPLINE *splcodes = NULL;
NUVEC ShelfPos[6] = {};
NUVEC SubShelfPos[7] = {};
NUVEC CodePos[7] = {};
u16 shelfang = 0;

nuhspecial_s iconback = {};
nuhspecial_s infoblank = {};
nuhspecial_s cutblank = {};
nuhspecial_s cutfilm_unlocked = {};
nuhspecial_s cutfilm_locked = {};
nuhspecial_s toolblank = {};
nuhspecial_s codeblank = {};
nuhspecial_s question = {};
nuhspecial_s arrow1 = {};
nuhspecial_s arrow2 = {};
nuhspecial_s arrow3 = {};
nuhspecial_s arrow4 = {};
NUGSPLINE *shopcamspline = NULL;
NUVEC *shopcampos = NULL;
NUVEC *shopcamlookat = NULL;
static f32 TopShelfScale[6] = {};
static f32 TopBigScale[6] = {};
static f32 topscale[6] = {};
static f32 TopShelfPush[6] = {};
static f32 TopBigPush[6] = {};
static f32 toppush[6] = {};
i32 oldpicked = 1;
i32 picked = 1;
i32 subpicked = 0;
i32 subitemselected = 0;
static void (*drawptr)() = NULL;

static f32 scale2 = 0.0f;
static f32 scalenorm;
static f32 scalepicked;
static f32 scale3 = 0.0f;
static f32 scale4 = 0.0f;
static f32 subpush[3] = {};
static i32 moveitems = 0;
NUVEC selectedoff = {};
void *shopcutsceneplayer = NULL;

f32 ShopLockedScale = 0.0f;
f32 ShopNameAlpha = 0.0f;
i32 enteredshop = 0;
i32 SHOPACTIVE = 0;
i32 shopmenu = 0;
i32 col = 0;
char usercode[6] = {'A', 'A', 'A', 'A', 'A', 'A'};
i32 shop_from_cutsceneplayer = 0;

static f32 SubBigCharPush = 0.0f;
static f32 SubNormCharPush = 0.0f;
static ShopMenuCallback menuptr = NULL;
static ShopMenuCallback oldmenuptr = NULL;
static ShopMenuCallback menuparent[3] = {};
static ShopMenuCallback oldmenuparent[3] = {};
static ShopDrawCallback drawparent[3] = {};
static ShopDrawCallback olddrawparent[3] = {};
static ShopDrawCallback drawpanelptr = NULL;
static ShopDrawCallback olddrawpanelptr = NULL;
static ShopDrawCallback olddrawptr = NULL;
static i32 currentmenulevel = 0;
static i32 oldcurrentmenulevel = 0;
static i32 currentdrawlevel = 0;
static i32 oldcurrentdrawlevel = 0;
static f32 oldpickedscale = 0.0f;
static f32 oldpickedpush = 0.0f;
static f32 slidetimer = 0.0f;
static f32 scaleoverride[7] = {};
static i32 ExitMenu = 0;
static i32 lastitem = 0;
static f32 hintdrawwait = 0.0f;
static i32 scrollkeyhit = 0;
static i32 SHOPCUTCOUNT = 128;
static i32 easesubin = 0;
static f32 inoutscale = 0.0f;
static char *cheatname = NULL;

extern i32 ItemMenu(MENU_s *menu);
extern void DrawItemMenu2D();
extern void InitAlphaList();
extern void InitExtraList();
extern HINT_s *Hint_FindHint(i32 hint_id);

i32 DoShopMenu(MENU_s *menu);

i32 BuyShopItem(shopitem_s *items, i32 index, i32 charge);

i32 UpdateShop(MENU_s *menu);

void SelectSubItem();

i32 SelectShopItem(shopitem_s *items, i32 index);

static NUVEC HubShopPos = {-26.3f, 0.0f, -49.5f};

i32 Shop_UpdateHint(HINT_s *hint);

void BuyAllShopExtras();

void GetShopCamLookPos(nuvec_s *position);

i32 MoveSubItemsRight(i32 *ids, NUVEC *positions, i32 count);

i32 MoveSubItemsLeft(i32 *ids, NUVEC *positions, i32 count);

void Shop_CollectAllCharacters(i32 mode);

void InitShop(WORLDINFO_s *world);

i32 CheckCash(shopitem_s *items, i32 item);

i32 CodeMenu(MENU_s *);

extern void Hint_CancelCurrent(void);
extern void DrawSubItemMenu2D(void);
extern void DrawSubItemMenu3D(void);
extern void DrawCodeMenu(void);
extern void DrawCodeMenu3D(void);
static i32 SubItemMenu(MENU_s *);
extern void Hint_ResetHint(i32, i32);
extern void Hint_SetHintFromId(i32, i32, i32);
extern i16 tUNKNOWN;

static f32 ShopClamp01(f32 value) {
    return NuFmax(0.0f, NuFmin(value, 1.0f));
}

static f32 ShopSinePhase(f32 phase) {
    const i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
    return (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
}

static void Shop_GetInput(SHOPINPUT *input);

static inline i32 SubItemMenu(MENU_s *menu);


i32 ItemMenu(MENU_s *menu);
extern i16 tSELECT, tEXIT, tBACK, tBUY, tPLAY, tSELECTING;

void DrawShopPrompts();

void LoadShelfSplines();

void DrawShop3D(WORLDINFO_s *world);

void DrawCodeMenu();

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float, float y_push, u16 x_rot, u16 y_rot,
              u16 z_rot);

void DrawCodeMenu3D();

static void Shop_DrawCharacter(shopitem_s *item, NUVEC *position, f32 scale_value, f32 ypush, u16 xrot, u16 yrot,
                               u16 zrot);
extern AREADATA *ANEWHOPE_ADATA;

void DrawSubItems();


void DrawTopShelf(i32);

void DrawShopPanel();

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

f32 HUB_EPISODESUBTITLEY = -0.745f;
f32 HUB_EPISODETITLEY = -0.595f;
extern i32 draw_para;

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
extern u8 CharClipToBlobShadows;


i32 DrawGameObjectsProcess();

void DrawStatusTextFraction(i32 current, i32 total, float x, float y, u16 angle, float scale, u32 colour,
                            float animation, float animation_duration);

void DrawGameMessage_Targets(GAMEMESSAGE_s *message, nuvec_s *position, float scale);

i32 DrawPanel3DObjectNoAlpha(float x, float y, float z, float scale_x, float scale_y, float scale_z, u16 rotate_x,
                             u16 rotate_y, u16 rotate_z, nuhspecial_s *special, i32 rotate_order);


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
static __used__ i32 PauseRenderOff();

static __used__ i32 MatrixReflection_CanOverride();

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

static __used__ void BackDrop_Alpha(float *alpha);

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

extern "C" f32 MainRenderTargetTime;
extern void (*BackDrop_AlphaFn)(f32 *alpha);
extern void BackDrop_UpdateColours(i32 instant);
extern f32 PauseMenus_X;
extern i32 PauseMenus_Align;
struct CUTSCENEPLAYERCLIP;
extern CUTSCENEPLAYERCLIP *CutScenePlayer_Active();

void UpdateGameMessages();
extern "C" i32 DoubleScore;
extern f32 LevelNameTime, LevelNameMul;
extern i32 pause_rndr_on;
extern "C" TIMER BonusTimer;

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

extern f32 alert_timer;

void MakeBaddiesForgetAboutParty(i32 check_hostility);

void GameAttackInit();

extern "C" void MenuRegisterSoundFX(i32 move, i32 select, i32 back, i32 no_entry);
i32 GameAudio_GetSfxId(i32 sfx);
void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32 volume);

void GameFog_Update(WORLDINFO_s *world);

void *GameBufferAlloc(variptr_u *buf, variptr_u *buf_end, i32 size);


void GameAudio_AddSfx(i32 sfx, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx);

void GameObjectOrigin(GameObject_s *object);

void PortalGameObject(GameObject_s *object, i32 enabled, i32 update_room, i16 room, nugscn_s *scene);

void SnapCreaturePos(GameObject_s *object, NUVEC *position, i32 angle, AIPATHINFO_s *path_info, i32 set_on_surface);

i32 Game_IgnoreInput();

extern i16 id_GONKDROID;
u32 GameAI_TotalScore();

void GameDrawMenuEntry(MENU_s *menu, char *text);

void GameAnimSys_Update(GAMEANIMSYS_s *system);

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


i32 GameDrawCharacterModel(CHARACTERMODEL_s *model, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *secondary_matrix,
                           NUMTX *reflection_matrix, NUMTX *auxiliary_matrix, GameObject_s *object, u32 flags);

extern i16 id_ANAKINJEDISCARRED;

void GameBlowUpBlownUpFn_LSW(GIZMOBLOWUP_s *blowup);

void GameLoadCharacterModels(APICHARACTERMODELLIST_s *list, i32 append, VARIPTR *buf, VARIPTR *buf_end, i32 area_models,
                             i32 area);

void Game_WorldInfo_InitMenu(WORLDINFO_s *world, i32 *menu_id, i32 *);

extern NUVEC alert_pos;
extern "C" {
extern i32 party_under_cover;
}
i32 ZapTarget(GameObject_s *object);

void GameCreatureOpponentSelection(AISYS_s *system, i32 count, APIOBJECT_s **objects, i32 goody_count,
                                   APIOBJECT_s **goodies, i32 baddy_count, APIOBJECT_s **baddies, u64 awareness,
                                   float);


extern "C" i16 id_THEEMPEROR, id_IMPERIALGUARD, id_BODYGUARD;
i32 CannotKill(GameObject_s *object);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *, NUVEC *, i32, f32, NUVEC *, NUVEC *, BOLT_s *, u32, u8 *);
void AlertSurroundingCreatures(GameObject_s *, NUVEC *);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, u16, u16);
extern f32 ForceThrowSpeed, ForceThrowGravity;
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);

static void LightSabreStreakCode(GameObject_s *object, i32 blade, i32 effect);

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

static __used__ void LEGO_100PercentFn();
static __used__ void LEGO_AllGoldBricksFn();

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

void ConstantRumble(GameObject_s *object, f32 strength, f32 phase);

void PowerUp_Update(GameObject_s *object);


extern GameObject_s * player_tag_to;
extern GameObject_s * player_tag_from;
i32 TakeOverYodaSeekDistanceHack(GameObject_s *, GameObject_s *, NUVEC *);



void ScaleGameObject(GameObject_s *object);

void DestroySnakeBody(GameObject_s *obj);

void RemoveGameObject(GameObject_s *obj, i32);

extern u16 TargetDeg_Near, TargetDeg_Mid, TargetDeg_Far;
extern f32 TargetDist_Near2, TargetDist_Mid2;

extern i32 nethost;
extern i16 id_PKDROID;
void ResetAICreature(GameObject_s *, AISYS_s *);
void SpawnCreatureFromCrate(GameObject_s *, f32, f32);
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

// ==== src/legoapi/render/core/screen.cpp: declarations ====
#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/render/light/fade.h"
#include "legoapi/characters/motion.h"
#include "legoapi/menus/core/text.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "legoapi/menus/screens/shop.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/characters/motion/gameanim.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "batman.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/core/startup/main.h"
#include "legoapi/render/light/fade.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/numath/numtx.h"

#include <GLES2/gl2.h>
#include <stdio.h>
#include <string.h>



void RenderQuads(i16 *);

void InitAlphaList();


// The fade system and the four fade effects handed to FadeSystem::AddFade()
// are screen.cpp globals in the original: its static initializer registers
// their destructors (original bss @0x127bf20 / 0x127bf08 / 0x127bf00 /
// 0x127bef8 / 0x127bef0).
extern Fade fade;
extern FadeWipe fadeWipe;
extern FadeStillWipe fadeStillWipe;
extern FadeStill fadeStill;

// Material handles used by the original 2-D fade paths.
extern NUMTL * ScreenFadeMtl;
extern NUMTL * FadeMtl2;
extern NUMTL * FadeMtl;

extern i32 (*PauseRenderOffFn)();
extern i32 cut_waiting_for_new_level;
extern u32 animSetVisibilityHack[6];
extern i32 specVisibilityFlashHack;

void GameAnimSet_Draw(GAMEANIMSET_s &set);
extern f32 hackFlashTimer;
extern GAMEANIMSET_s *hackFlashingGameAnimSet;
extern nuhspecial_s *hackFlashingSpecial;

extern volatile i32 waiting_for_level;
extern "C" {
    extern f32 MainRenderTime;
    extern i32 back_rgba[2];
    extern i32 clear_screen_onstill;
    extern i32 screendump;
}
void BackDrop_Draw(f32, i32);

extern "C" void NuBackbufferCopy(i32);

extern "C" f32 NuIOS_GetAspectRatio(void);

void WidescreenCode(i32);

SAGA_HOST_WEAK void InitStillRender(variptr_u *, variptr_u);

void DrawPauseScreenWipe();

void GrabStillScreen();

void NeedScreenGrab(i32 needed);

i8 IsGrabbingScreen();

void DrawStillScreen(i32 clear);

void ClearStill();

void UpdateCutBorders();

void HandleStillRender();

void PreRenderFlashHack();

void PostRenderFlashHack();

extern nugscn_s * FadeLoop_ObjScene;
extern nuhspecial_s FadeLoop_ObjHSpecial;

void FadeLoop_SetObj(nugscn_s *scene, char *name);

void FadeLoop_DrawObj(float alpha);

i32 FadeLoop_UsingObj();

void CreateFadeMaterials();

void FadeLoop(char *text, i32 direction, float duration, void (*draw)(float));

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
void CreateUsefulMaterials();
extern "C" ripple_set_s *ripples;
extern "C" NUMTL *ShadowMat;


void CharConfig_ConfigureAll(i32 permanent, nufpcomjmp_s *game_keywords);
void ExtraCharacterFixUpAfterConfig();
extern i32 CHARPAK;

extern i32 InCollectList_Index(i32 id, COLLECTID *list, i32 count);
extern i32 Collection_Got(i32 id);
extern void Customiser_SaveModelTextureIDs(CUSTOMISER *customiser, CHARACTERMODEL_s *model);
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

void HairMovement(GameObject_s *object);

void HeadMovement(GameObject_s *object);

nuhspecial_s *CharScene_FindHSpecial(WORLDINFO_s *world, i32 character_id);

void CharScene_Draw(WORLDINFO_s *world, i32 character_id, numtx_s *matrix, numtx_s *reflection_matrix);

i32 CharIDFromName(char *name);

CHARACTERDATA *CDataFromName(char *name);

i32 RedirectAnim(char *directory, ANIMREDIRECT *redirects, ANIMLIST_s *animation_list, char *path);

void CharScenes_Init(variptr_u *buf, variptr_u *);

void CharScenes_LevelLoad(WORLDINFO *world);

extern "C" {
    extern i16 id_R2Q5;
    void PlaySfx(char *, nuvec_s *);
    void PlaySfxAndSetVolume(char *, nuvec_s *, f32);
}
void Move_CHARACTER(GameObject_s *);
void Animate_CHARACTER(GameObject_s *);

void FixUpCharacters(CHARFIXUP *fixup);

void PostAnimate_FETT(GameObject_s *object);

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

void InitPlayerAI(GameObject_s *);
void Player_ClearContext(GameObject_s *, i32);
void ResetPlayerMoves(GameObject_s *);
void Player_ResetContexts(PLAYERPACKET_s *);
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
void NewStatusRumbleBuzz(i32, f32, f32, i32);
void GameAudio_PlaySfx(i32, nuvec_s *, i32, i32);
extern "C" void PlaySfx(char *, nuvec_s *);
void Collection_Draw(COLLECTION_s *, f32, f32, f32, APICHARACTERMODELLIST_s *, f32, i32);
void Collection_GetPos(COLLECTION_s *, i32, f32 *, f32 *);
void DrawCharIcon(i32, f32, f32, f32, f32, i32, f32, f32, i32, nuhspecial_s *);
extern i32 STATUS_R, STATUS_G, STATUS_B;
extern f32 COLLECTION_Y_STATUS;

void CollectCharcters_Draw(STATUS_STAGE_s *stage, STATUSPACKET_s *packet, i32 active);

void CollectCharcters_Skip(STATUS_STAGE_s *stage, STATUSPACKET_s *packet);


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

// ==== functions in original address order ====

static void Shop_GetInput(SHOPINPUT *input) {
    memset(input, 0, sizeof(*input));
    const u32 select = GAMEPAD_MENUSELECT;
    const u32 cancel = GAMEPAD_MENUCANCEL;
    const i32 client = netclient;
    const u32 right = GAMEPAD_DRIGHT | GAMEPAD_TOGGLERIGHT;
    const u32 left = GAMEPAD_DLEFT | GAMEPAD_TOGGLELEFT;
    const u32 up = GAMEPAD_DUP;
    const u32 down = GAMEPAD_DDOWN;
    for (i32 p = 0; p < 2; ++p) {
        if (!MenuPacket.active_player[p])
            continue;
        const u32 pressed = GamePad[p].buttons_pressed;
        if ((pressed & select) != 0 && client == 0) {
            input->confirm = 1;
            return;
        }
        if ((pressed & cancel) != 0) {
            input->cancel = 1;
            return;
        }
        if (client != 0)
            continue;
        const u32 held = GamePad[p].buttons_held | GamePad[p].unknown_0c;
        const u32 edge = pressed | GamePad[p].unknown_10;
        if (held & left)
            input->left_held = 1;
        if (held & right)
            input->right_held = 1;
        if (input->left_held && input->right_held)
            input->left_held = input->right_held = 0;
        if (held & up)
            input->up_held = 1;
        if (held & down)
            input->down_held = 1;
        if (input->up_held && input->down_held)
            input->up_held = input->down_held = 0;
        if (edge & left)
            input->left_pressed = 1;
        if (edge & right)
            input->right_pressed = 1;
        if (input->left_pressed && input->right_pressed)
            input->left_pressed = input->right_pressed = 0;
        if (edge & up)
            input->up_pressed = 1;
        if (edge & down)
            input->down_pressed = 1;
        if (input->up_pressed && input->down_pressed)
            input->up_pressed = input->down_pressed = 0;
    }
}

i32 ItemMenu(MENU_s *menu) {
    static __used__ i32 itemchanged;
    static i32 movesfxlock;
    static i32 SubMenu;
    SHOPINPUT input;
    const i32 entry_picked = picked;
    Shop_GetInput(&input);

    if (slidetimer >= 0.0f) {
        slidetimer -= FRAMETIME;
        ShopNameAlpha = 0.0f;
    }
    if (ExitMenu)
        ShopNameAlpha = 0.0f;

    i32 candidate = entry_picked;
    i32 selection = -1;
    i32 cancel = 0;
    if (menu->confirm_pressed != 0 && menu->selected_row == 0) {
        candidate = static_cast<i32>(menu->selected_column) + 1;
        if (candidate == entry_picked) {
            input.value[8] = 1;
        }
    }

    itemchanged = 0;
    Hint_CancelCurrent();
    hintdrawwait = 0.2f;

    if (slidetimer < 0.0f) {
        movesfxlock = 0;

        if (input.value[0] != 0 && picked > 1) {
            candidate = picked - 1;
            if (candidate == 4 && SHOPGOLDBRICKS == 0)
                candidate = picked - 2;
            if (candidate == 3)
                candidate = 2;
        } else if (input.value[1] != 0 && picked <= 3) {
            candidate = picked + 1;
            if (candidate == 3)
                candidate = picked + 2;
            if (candidate == 4 && SHOPGOLDBRICKS == 0)
                candidate = picked;
        } else if (input.value[8] != 0) {
            if (picked == 0) {
                picked = 1;
                candidate = entry_picked;
            } else if (picked == 5) {
                picked = SHOPGOLDBRICKS != 0 ? 4 : 3;
                candidate = entry_picked;
            } else
                selection = picked;
        } else if (input.value[9] != 0) {
            cancel = 1;
        }
    }

    if (candidate != entry_picked) {
        lastitem = picked;
        picked = candidate <= 0 ? 1 : candidate > 5 ? 5 : candidate;
        SubMenu = 0;

        slidetimer = 0.125f;
        if (movesfxlock == 0) {
            movesfxlock = 1;
            GameAudio_PlaySfx(0x2f, reinterpret_cast<nuvec_s *>(SubShelfPos) + 5, 0, 0);
        }
        GameCam_Blend(GameCam, 0.3f, 0.0f, 1);
        ShopNameAlpha = 0.0f;
    }

    if (selection != -1) {
        if (selection != picked) {
            picked = selection;
            if (!movesfxlock) {
                movesfxlock = 1;
                GameAudio_PlaySfx(0x2f, &SubShelfPos[5], 0, 0);
            }
            ShopNameAlpha = 0.0f;
        }
        lastitem = picked;
        picked = -1;
        slidetimer = 0.125f;
        SubMenu = 1;
        GameAudio_PlaySfx(0x30, NULL, 0, 0);
        GameCam_Blend(GameCam, 0.6f, 0.0f, 1);
        ShopNameAlpha = 0.0f;
    }
    if (cancel) {
        ExitMenu = 1;
        slidetimer = 0.125f;
        lastitem = picked;
        picked = -1;
        SubMenu = 0;
        GameAudio_PlaySfx(0x31, NULL, 0, 0);
    }

    if (slidetimer < 0.0f)
        enteredshop = 0;
    else if ((lastitem != picked || enteredshop) && !SubMenu) {
        const f32 reverse_phase = slidetimer * 8.0f;
        f32 old_factor = 1.0f - ShopSinePhase(1.0f - reverse_phase);
        f32 forward_factor = 1.0f - ShopSinePhase(reverse_phase);

        if (lastitem != -1) {
            old_factor = ShopClamp01(old_factor);
            topscale[lastitem] = TopBigScale[lastitem] + (TopShelfScale[lastitem] - TopBigScale[lastitem]) * old_factor;
            forward_factor = ShopClamp01(forward_factor);
            toppush[lastitem] = TopBigPush[lastitem] + (TopShelfPush[lastitem] - TopBigPush[lastitem]) * forward_factor;
        }

        if (picked != -1 && lastitem != -1) {
            old_factor = ShopClamp01(old_factor);
            topscale[picked] = TopShelfScale[picked] + (TopBigScale[picked] - TopShelfScale[picked]) * old_factor;
            forward_factor = ShopClamp01(forward_factor);
            toppush[picked] = TopShelfPush[picked] + (TopBigPush[picked] - TopShelfPush[picked]) * forward_factor;
        }
    }

    if (SubMenu == 0) {
        if (ExitMenu && slidetimer < 0.0f) {
            menuptr = nullptr;
            return 5;
        }
        return 7;
    }

    if (slidetimer > 0.0f)
        return 7;

    picked = lastitem;
    SubMenu = 0;
    if (TopShelf[lastitem].type == 3) {
        lastitem = -1;
        easesubin = 1;
        inoutscale = 0.0f;
        cheatname = 0;
        slidetimer = 0.125f;
        shopmenu = 11;
        col = 0;

        ++currentmenulevel;
        menuparent[currentmenulevel] = menuptr;
        menuptr = CodeMenu;
        memcpy(usercode, "AAAAAA", 6);

        ++currentdrawlevel;
        drawparent[currentdrawlevel] = drawpanelptr;
        drawpanelptr = DrawCodeMenu;
        drawptr = DrawCodeMenu3D;
    } else {
        easesubin = 1;
        ++currentmenulevel;
        inoutscale = 0.0f;
        menuparent[currentmenulevel] = menuptr;
        menuptr = SubItemMenu;

        ++currentdrawlevel;
        slidetimer = 0.125f;
        drawparent[currentdrawlevel] = drawpanelptr;
        drawpanelptr = DrawSubItemMenu2D;
        drawptr = DrawSubItemMenu3D;
    }
    return 7;
}

void DrawItemMenu2D() {
    if (0.0f < ShopNameAlpha) {
        i16 text_id;
        if (picked == 0) {
            text_id = tHINTS;
        } else if (picked == 1) {
            text_id = tCHARACTERS;
        } else if (picked == 2) {
            text_id = tEXTRAS;
        } else if (picked == 4) {
            text_id = tGOLDBRICKS;
        } else if (picked == 5) {
            text_id = tSTORYCLIPS;
        } else {
            text_id = tENTERCODE;
        }

        const i32 alpha = static_cast<i32>(ShopNameAlpha * 128.0f);
        const f32 y = (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f;
        SmartTextEx(TTab[text_id], 0.0f, y, 1.0f, 0.8f, 0.8f, 0.8f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                    static_cast<u32>(alpha));
    }
}

void DrawCodeMenu() {
    if (cheattimer > 0.0f) {
        if (cheatname && cheattimer < 3.1499998569488525f && NuFmod(cheattimer, 0.35f) < 0.175f) {
            SmartTextEx(cheatname, 0.0f, (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f, 1.0f, 0.8f, 0.8f, 0.8f, 0,
                        0, 255, 0, 1.7f, 1, NULL, 0, 128);
        }
        cheattimer -= FRAMETIME;
    }
}

i32 CodeMenu(MENU_s *) {
    static f32 timer;
    static __used__ i32 itemchanged;
    static i32 movesfxlock;
    i32 down = 0, up = 0, left = 0, right = 0, confirm = 0, cancel = 0;
    SHOPINPUT input;
    Shop_GetInput(&input);
    if (slidetimer >= 0.0f)
        slidetimer -= FRAMETIME;
    if (timer >= 0.0f)
        timer -= FRAMETIME;
    itemchanged = 0;
    if (__builtin_expect(!!(easesubin), 1)) {
        f32 factor = 1.0f - ShopSinePhase(1.0f - 8.0f * slidetimer);
        if (easesubin == 1)
            inoutscale = 0.0f + ShopClamp01(factor);
        else if (easesubin == -1)
            inoutscale = 1.0f - ShopClamp01(factor);
    }
    if (slidetimer <= 0.0f) {
        easesubin = 0;
        codevalid = 0;
        if (input.left_held)
            left = 1;
        else if (input.right_held)
            right = 1;
        else if (input.down_held) {
            if (__builtin_expect(timer <= 0.0f, 0))
                down = 1;
            else if (__builtin_expect(!!(input.down_pressed), 0))
                down = 1;
        } else if (input.up_held) {
            if (timer <= 0.0f)
                up = 1;
            else if (input.up_pressed)
                up = 1;
        }
        if (input.confirm)
            confirm = 1;
        else if (input.cancel)
            cancel = 1;
        if (down) {
            timer = 0.125f;
            movesfxlock = 1;
            const i32 next = codechar - 1;
            codechar = next < 0 ? 35 : next;
            usercode[col] = codechars[codechar];
            GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
        }
        if (up) {
            timer = 0.125f;
            movesfxlock = 1;
            const i32 next = codechar + 1;
            codechar = next < 36 ? next : 0;
            codevalid = 0;
            usercode[col] = codechars[codechar];
            GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
        }
        if (left) {
            lastitem = col;
            --col;
            if (__builtin_expect(col < 0, 0))
                col = 0;
            codevalid = 0;
            if (!usercode[col])
                codechar = 0;
            else
                for (i32 i = 0; i < 36; ++i) {
                    if (__builtin_expect(usercode[col] != codechars[i], 1)) {
                        continue;
                    }
                    codechar = i;
                    break;
                }
            if (lastitem != col) {
                slidetimer = 0.125f;
                if (!movesfxlock) {
                    movesfxlock = 1;
                    GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
                }
            }
        }
        if (right) {
            lastitem = col;
            ++col;
            if (col > 5)
                col = 5;
            codevalid = 0;
            if (!usercode[col])
                codechar = 0;
            else
                for (i32 i = 0; i < 36; ++i) {
                    if (usercode[col] == codechars[i]) {
                        codechar = i;
                        break;
                    }
                }
            if (lastitem != col) {
                slidetimer = 0.125f;
                if (!movesfxlock) {
                    movesfxlock = 1;
                    GameAudio_PlaySfx(0x2f, &CodePos[2], 0, 0);
                }
            }
        }
        if (cancel) {
            easesubin = -1;
            slidetimer = 0.125f;
            codevalid = 0;
            ExitMenu = 1;
            codechar = 0;
            GameAudio_PlaySfx(0x31, NULL, 0, 0);
        }
        if (confirm) {
            char code[7];
            code[6] = 0;
            usercode[col] = codechars[codechar];
            for (i32 i = 0; i < 6; ++i)
                code[i] = usercode[i];
            u32 crc = CRC_ProcessStringIgnoreCase(code);
            i32 i;
            for (i = 0; i < 144; ++i)
                if (__builtin_expect(codelist[i] && codelist[i] == crc, 0))
                    break;
            if (i == 144) {
                PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
                cheatname = NULL;
            } else {
                codevalid = 1;
                if (i < charcheatix) {
                    if (Game_CharacterSave &&
                        !(Game_CharacterSave[static_cast<u16>(CharItems[i].item_id)] & SAVE_CHARACTER_UNLOCKED)) {
                        Game_CharacterSave[static_cast<u16>(CharItems[i].item_id)] |= SAVE_CHARACTER_UNLOCKED;
                        AddToCompletionPoints(POINTS_PER_CHARACTER);
                        PlaySfx("Shop_BuyCheat", &SubShelfPos[3]);
                    } else
                        PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
                    cheatname = TTab[CDataList[static_cast<u16>(CharItems[i].item_id)].name_id];
                } else if (i < extracheatix) {
                    i -= charcheatix;
                    if (!(Game.extra_purchased_bits[i / 32] & (1U << (i & 31)))) {
                        Game.extra_purchased_bits[i / 32] |= 1U << (i & 31);
                        ExtraItems[i].unlocked = SAVE_ON;
                        AddToCompletionPoints(POINTS_PER_CHEAT);
                        i8 area = static_cast<i8>(Cheat[i].area);
                        if (area != -1 && !Game.area_save[area].red_brick_collected) {
                            Game.area_save[area].red_brick_collected = SAVE_COMPLETE;
                            AddToCompletionPoints(POINTS_PER_REDBRICK);
                        }
                        Game.extra_unlocked_bits[i / 32] |= static_cast<u32>(1ULL << (i % 32));
                        PlaySfx("Shop_BuyCheat", &SubShelfPos[3]);
                    } else
                        PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
                    cheatname = TTab[Cheat[i].text_id ? *Cheat[i].text_id : tUNKNOWN];
                }
                pickedbing = 0.35f;
                cheattimer = 3.1499998569488525f;
            }
        }
    } else
        movesfxlock = 0;

    if (slidetimer >= 0.0f) {
        f32 factor = 1.0f - ShopSinePhase(1.0f - 8.0f * slidetimer);
        if (lastitem != -1) {
            if (__builtin_expect(easesubin == 1, 0)) {
                factor = ShopClamp01(factor);
                codemenuscale[lastitem] =
                    codeshelfscale[lastitem] + (codebigscale[lastitem] - codeshelfscale[lastitem]) * factor;
                factor = ShopClamp01(factor);
                subpush[2] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * factor;
            } else {
                factor = ShopClamp01(factor);
                codemenuscale[lastitem] =
                    codebigscale[lastitem] + (codeshelfscale[lastitem] - codebigscale[lastitem]) * factor;
                factor = ShopClamp01(factor);
                subpush[2] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * factor;
            }
        }
        if (col != -1) {
            if (__builtin_expect(easesubin == -1, 0)) {
                factor = ShopClamp01(factor);
                codemenuscale[col] = codebigscale[col] + (codeshelfscale[col] - codebigscale[col]) * factor;
                factor = ShopClamp01(factor);
                subpush[1] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * factor;
            } else {
                factor = ShopClamp01(factor);
                codemenuscale[col] = codeshelfscale[col] + (codebigscale[col] - codeshelfscale[col]) * factor;
                factor = ShopClamp01(factor);
                subpush[1] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * factor;
            }
        }
    } else
        lastitem = -1;
    if (ExitMenu && slidetimer < 0.0f) {
        ExitMenu = 0;
        ShopNameAlpha = 0.0f;
        easesubin = -1;
        lastitem = -1;
        subpush[1] = 0.05f;
        slidetimer = 0.125f;
        GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
        return 6;
    }
    return 7;
}

void DrawSubItemMenu2D() {
    shopitem_s *items;
    i32 *ids;
    if (picked == 0) {
        items = HintItems;
        ids = HintShelfIds;
    } else if (picked == 1) {
        items = CharItems;
        ids = CharShelfIds;
    } else if (picked == 2) {
        items = ExtraItems;
        ids = ExtraShelfIds;
    } else if (picked == 4) {
        items = BrickItems;
        ids = BrickShelfIds;
    } else if (picked == 5) {
        items = CutItems;
        ids = CutShelfIds;
    } else {
        return;
    }
    if (items) {
        char title[128], subtitle[128];
        char *name = items[ids[3]].name;
        i32 buyable = 0;
        i32 show_name = 0;
        if (items[ids[3]].type == 1) {
            if (__builtin_expect(items[ids[3]].unlocked == 1, 1))
                show_name = 1;
            else if (__builtin_expect(!!(CollectIDUnlocked(static_cast<u16>(items[ids[3]].item_id))), 0))
                buyable = show_name = 1;
            else {
                f32 scale = 0.7f * ShopLockedScale;
                SmartTextEx(TTab[tLOCKED], 0.0f, (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f, 1.0f, scale, scale,
                            scale, 0, 255, 0, 0, 1.7f, 1, NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
        } else if (items[ids[3]].type == 0) {
            buyable = items[ids[3]].unlocked != 1 && items[ids[3]].price != 0;
        } else if (items[ids[3]].type == 2) {
            const i32 id = static_cast<u16>(items[ids[3]].item_id);
            NuStrCpy(subtitle, TTab[Cheat[id].text_id ? *Cheat[id].text_id : tUNKNOWN]);
            name = subtitle;
            i8 area = static_cast<i8>(Cheat[id].area);
            if (items[ids[3]].unlocked == 1)
                show_name = 1;
            else if (id <= 7 || area == -1 || Game.area_save[area].red_brick_collected)
                buyable = show_name = 1;
            else {
                sprintf(title, "%s %i", TTab[tPOWERBRICK], id - 7);
                f32 scale = 0.6f * ShopLockedScale;
                SmartTextEx(title, 0.0f, HUB_EPISODETITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                            static_cast<i32>(128.0f * ShopNameAlpha));
                SmartTextEx(TTab[tLOCKED], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, 255, 0, 0, 1.7f, 1,
                            NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
        } else if (items[ids[3]].type == 4) {
            const i32 id = static_cast<u16>(items[ids[3]].item_id);
            sprintf(title, "%s %i", TTab[tGOLDBRICK], id + 1);
            name = title;
            if (items[ids[3]].unlocked == 1)
                show_name = 1;
            else if (!(static_cast<f32>(id * 3600) > Game.field30_0x7c2c))
                buyable = show_name = 1;
            else {
                SmartTextEx(title, 0.0f, HUB_EPISODETITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                            static_cast<i32>(128.0f * ShopNameAlpha));
                f32 scale = 0.6f * ShopLockedScale;
                SmartTextEx(TTab[tLOCKED], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, 255, 0, 0, 1.7f, 1,
                            NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
        } else if (items[ids[3]].type == 5) {
            const i32 id = static_cast<u16>(items[ids[3]].item_id);
            i32 available = CutScenePlayer_CanStart(id);
            CutScenePlayer_GetText(id, title, subtitle, available);
            SmartTextEx(title, 0.0f, HUB_EPISODETITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                        static_cast<i32>(128.0f * ShopNameAlpha));
            if (available && shopcutsceneplayer)
                SmartTextEx(subtitle, 0.0f, HUB_EPISODESUBTITLEY, 1.0f, 0.6f, 0.6f, 0.6f, 0, 255, 191, 0, 1.7f, 1, NULL,
                            0, static_cast<i32>(128.0f * ShopNameAlpha));
            else {
                f32 scale = 0.6f * ShopLockedScale;
                SmartTextEx(TTab[tLOCKED], 0.0f, HUB_EPISODESUBTITLEY, 1.0f, scale, scale, scale, 0, 255, 0, 0, 1.7f, 1,
                            NULL, 0, static_cast<i32>(128.0f * ShopNameAlpha));
            }
        }
        i32 price;
        switch (items[ids[3]].type) {
            case 0:
                price = items[HintShelfIds[3]].price;
                break;
            case 2:
                price = items[ExtraShelfIds[3]].price;
                break;
            case 4:
                price = items[BrickShelfIds[3]].price;
                break;
            case 5:
                return;
            case 1:
                price = items[CharShelfIds[3]].price;
                break;
            default:
                price = items[CharShelfIds[3]].price;
                break;
        }
        if (subitemselected > 0) {
            if (!buyable) {
                ShopLockedScale = 1.5f;
                GameAudio_PlaySfx(0x32, NULL, 0, 0);
                subitemselected = 0;
            } else if (Game.coins < price) {
                CoinTotalScale = 1.5f;
                PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
                subitemselected = 0;
            }
        }
        if (!(ShopNameAlpha > 0.0f))
            return;
        if (show_name) {
            f32 y = buyable ? HUB_EPISODETITLEY : (HUB_EPISODETITLEY + HUB_EPISODESUBTITLEY) * 0.5f;
            f32 scale = buyable ? 0.6f : 0.8f;
            SmartTextEx(name, 0.0f, y, 1.0f, scale, scale, scale, 0, 255, 255, 255, 1.7f, 1, NULL, 0,
                        static_cast<i32>(128.0f * ShopNameAlpha));
        }
        if (buyable)
            CoinTotal_Draw(price, HUB_EPISODESUBTITLEY, 1.2f, 0, ShopNameAlpha, 255, 191, 0);
        if (subitemselected == 2) {
            f32 pulse = 1.0f;
            if (!TestForController()) {
                f32 since_touch = GlobalTimer.time_elapsed - (LastTouchTime + 2.0f);
                if (since_touch > 4.0f) {
                    i32 angle = static_cast<i32>(NuFmod(since_touch, 4.0f) * 0.25f * 65536.0f);
                    f32 value = NuTrigTable[(angle >> 1) & 0x7fff] - 0.8f;
                    if (!(value < 0.0f))
                        pulse = value + 1.0f;
                }
            }
            f32 size = ICONSIZE * pulse;
            DrawPanel3DObject(-0.3f, -0.3f, 1.0f, size, size, size, 0, 0, 0, &WORLD->lev_objs[165].special, 0,
                              ShopNameAlpha);
            DrawPanel3DObject(0.3f, -0.3f, 1.0f, size, size, size, 0, 0, 0, &WORLD->lev_objs[165].special, 0,
                              ShopNameAlpha);
            f32 text_size = pulse * 0.6f;
            f32 alpha = static_cast<i32>(128.0f * ShopNameAlpha);
            u8 text_alpha = static_cast<i32>(alpha);
            Text3DEx("$", -0.3f, -0.3f, 1.0f, 1.3f * text_size, text_size, text_size, 0, 0, 255, 0, text_alpha);
            Text3DEx("X", 0.3f, -0.31f, 1.0f, 1.3f * text_size, text_size, text_size, 0, 255, 0, 0, text_alpha);
            MENU_s *menu = &GameMenu[GameMenuLevel];
            menu->item_x[11] = -0.3f;
            menu->item_y[11] = -0.3f;
            menu->item_x[12] = 0.3f;
            menu->item_y[12] = -0.3f;
            menu->item_width[11] = size;
            menu->item_column[11] = 0;
            menu->item_row[11] = 2;
            menu->item_column[12] = 1;
            menu->item_width[12] = size;
            menu->item_row[12] = 2;
        }
    }
}

static void Shop_DrawCharacter(shopitem_s *item, NUVEC *position, f32 scale_value, f32 ypush, u16 xrot, u16 yrot,
                               u16 zrot) {
    if (!(!NuSpecialExistsFn(&iconback))) {
        i32 top_shelf_character = 0;
        if (item == &TopShelf[1]) {
            top_shelf_character = 1;
            item = &CharItems[static_cast<i32>(NuFmod(GameTimer.time_elapsed, static_cast<f32>(SHOPCHARCOUNT) * 0.2f) /
                                               0.2f)];
        }

        const i32 character_id = static_cast<u16>(item->item_id);
        const bool unlocked = CollectIDUnlocked(character_id) != NULL;
        const i32 unavailable = !unlocked || item->unlocked != 1;
        f32 alpha = 1.0f;
        if (!CollectIDUnlocked(character_id))
            alpha = 0.5f;

        NUVEC scale;
        scale.x = scale.y = scale.z = scale_value;
        NUMTX_ALIGNED16 matrix;
        NuMtxSetScale(&matrix, &scale);
        NuMtxRotateXInline(&matrix, xrot);
        NuMtxRotateYInline(&matrix, yrot);
        NuMtxRotateZInline(&matrix, zrot);
        NuMtxTranslate(&matrix, position);
        matrix.m31 += ypush;

        NuSpecialDrawAtAlpha(&iconback, &matrix, top_shelf_character ? 1.0f : alpha);

        i32 icon_object_id = CDataList[character_id].field20_0x42;
        if (icon_object_id != -1) {
            if (unavailable != 0)
                ++icon_object_id;
            WORLDINFO_s *world = WORLD;
            LEVEL_OBJECT_RUNTIME_s *icon = &world->lev_objs[icon_object_id];
            if (icon->active != 0) {
                NuSpecialDrawAtAlpha(&icon->special, &matrix, alpha);
            }
        }
    }
}

i32 Shop_UpdateHint(HINT_s *hint) {
    if (!Game.coins || !WORLD || WORLD->current_level != HUB_LDATA)
        return 0;
    if (GameCam->sock_position.location.sock != 0 || !player)
        return 0;
    if (!(NuVecXZDistSqr(&player->apiobj.position, &HubShopPos, NULL) < 1.0f))
        return 0;

    if (hint->control_mode_ids[0] == 0x5ea) {
        for (i32 i = 0; i < ShopCollection.count_y && i < 100; ++i) {
            if (CollectIDUnlocked(ShopCollection.list[i].id) && !Collection_Got(ShopCollection.list[i].id) &&
                Game.coins >= static_cast<u32>(ShopCollection.list[i].field3_0x4))
                return 1;
        }
    } else if (hint->control_mode_ids[0] == 0x5eb) {
        for (i32 i = 0; i < 44; ++i) {
            if (!(Game.extra_purchased_bits[i >> 5] >> (i & 31) & 1) &&
                (static_cast<i8>(Cheat[i].area) == -1 ||
                 Game.area_save[static_cast<i8>(Cheat[i].area)].red_brick_collected) &&
                Game.coins >= static_cast<u32>(Cheat[i].extra_price))
                return 1;
        }
    } else if (hint->control_mode_ids[0] == 0x5ec) {
        if (SHOPGOLDBRICKS > 0) {
            shopitem_s *item = BrickItems;
            i32 time_required = 0;
            for (i32 i = 0; i != SHOPGOLDBRICKS; ++i, time_required += 3600, ++item) {
                if (!(static_cast<u64>((&Game.shop_gold_brick_purchased_bits)[i >> 5]) >> (i & 31) & 1) &&
                    static_cast<f32>(time_required) <= Game.field30_0x7c2c &&
                    Game.coins >= static_cast<u32>(item->price))
                    return 1;
            }
        }
    }
    return 0;
}

void LoadShelfSplines() {
    splshelf = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shelf_top1"));
    splcharshelf = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shelf_bottom"));
    splcodes = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shelf_6"));

    if (splshelf == NULL || splcharshelf == NULL || splcodes == NULL) {
        return;
    }
    const i32 shelf_count = static_cast<i16>(splshelf->length);
    const i32 character_count = static_cast<i16>(splcharshelf->length);
    const i32 code_count = static_cast<i16>(splcodes->length);

    memset(SubShelfPos, 0, sizeof(SubShelfPos));
    memset(ShelfPos, 0, sizeof(ShelfPos));
    memset(CodePos, 0, 6 * sizeof(*CodePos));

    for (i32 i = 0; i < shelf_count; ++i) {
        ShelfPos[i] = splshelf->pts[i];
    }
    ShelfPos[2].x += (ShelfPos[3].x - ShelfPos[2].x) * 0.5f;
    ShelfPos[2].z += (ShelfPos[3].z - ShelfPos[2].z) * 0.5f;
    for (i32 i = 0; i < character_count; ++i) {
        SubShelfPos[i] = splcharshelf->pts[i];
    }
    for (i32 i = 0; i < code_count; ++i) {
        CodePos[i] = splcodes->pts[i];
    }

    NUVEC diff;
    NuVecSub(&diff, &CodePos[5], &CodePos[4]);
    NuVecAdd(&CodePos[6], &CodePos[5], &diff);
    CodePos[6].y += 0.02f;
    NuVecScale(&diff, &diff, 0.5f);
    NuVecSub(&CodePos[0], &CodePos[0], &diff);
    NuVecSub(&CodePos[1], &CodePos[1], &diff);
    NuVecSub(&CodePos[2], &CodePos[2], &diff);
    NuVecSub(&CodePos[3], &CodePos[3], &diff);
    NuVecSub(&CodePos[4], &CodePos[4], &diff);
    NuVecSub(&CodePos[5], &CodePos[5], &diff);
    NuVecSub(&CodePos[6], &CodePos[6], &diff);

    NUVEC start = splshelf->pts[0];
    NUVEC end = splshelf->pts[shelf_count];
    NuVecSub(&diff, &end, &start);
    shelfang = static_cast<u16>(NuAtan2D(diff.x, diff.z));
}

void InitExtraList() {
    for (i32 i = 0; i < 44; ++i) {
        NuStrCpy(ExtraItems[i].special_name, Cheat[i].extra_name);
        NuSpecialFind(WORLD->current_gscn, &ExtraItems[i].special, ExtraItems[i].special_name, 1);
        ExtraItems[i].type = 2;
        ExtraItems[i].unlocked = 0;
        ExtraItems[i].item_id = i;

        u32 unlocked = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&Game) + 0x7bf0)[i >> 5];
        if (((static_cast<u64>(unlocked) >> (i & 0x1f)) & 1) != 0) {
            ExtraItems[i].unlocked = 1;
            reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&Game) + 0x7c00)[i >> 5] |= 1u << (i & 0x1f);
        }
        ExtraItems[i].price = Cheat[i].extra_price;
    }

    SHOPEXTRACOUNT = 44;
    for (i32 i = 0; i < SHOPEXTRACOUNT; ++i) {
        char name[32];
        NuStrCpy(name, ExtraItems[i].special_name);
        NuStrCat(name, "b");
        NuSpecialFind(WORLD->current_gscn, &extrasils[i], name, 1);
    }
}

void InitAlphaList() {
    char name[16];

    for (i32 i = 0; i < 26; ++i) {
        memset(name, 0, sizeof(name));
        sprintf(name, "shop_%c", i + 'a');
        NuSpecialFind(WORLD->current_gscn, &atoz0to9icon[i], name, 1);
    }

#define FIND_SHOP_DIGIT(index_, digit_)                                                                                \
    memset(name, 0, sizeof(name));                                                                                     \
    sprintf(name, "shop_%c", digit_);                                                                                  \
    NuSpecialFind(WORLD->current_gscn, &atoz0to9icon[index_], name, 1)

    FIND_SHOP_DIGIT(26, '0');
    FIND_SHOP_DIGIT(27, '1');
    FIND_SHOP_DIGIT(28, '2');
    FIND_SHOP_DIGIT(29, '3');
    FIND_SHOP_DIGIT(30, '4');
    FIND_SHOP_DIGIT(31, '5');
    FIND_SHOP_DIGIT(32, '6');
    FIND_SHOP_DIGIT(33, '7');
    FIND_SHOP_DIGIT(34, '8');
    FIND_SHOP_DIGIT(35, '9');

#undef FIND_SHOP_DIGIT
}

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

void InitShop(WORLDINFO_s *world) {
    CharItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 100 * sizeof(shopitem_s)));
    HintItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 64 * sizeof(shopitem_s)));
    ExtraItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 64 * sizeof(shopitem_s)));
    CodeItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 64 * sizeof(shopitem_s)));
    BrickItems =
        static_cast<shopitem_s *>(GameBufferAlloc(&world->giz_buffer, &world->unknown_0108, 14 * sizeof(shopitem_s)));

    if (CharItems == NULL || HintItems == NULL || ExtraItems == NULL || CodeItems == NULL || BrickItems == NULL) {
        return;
    }

    TopShelf[0].type = 0;
    NuStrCpy(TopShelf[0].name, "Hint");
    NuStrCpy(TopShelf[0].special_name, "info");
    NuSpecialFind(things_scene, &TopShelf[0].special, "info", 1);

    TopShelf[1].type = 1;
    TopShelf[1].name[0] = '\0';
    TopShelf[1].special_name[0] = '\0';
    memset(&TopShelf[1].special, 0, sizeof(TopShelf[1].special));

    TopShelf[2].type = 2;
    NuStrCpy(TopShelf[2].name, "Extra");
    NuStrCpy(TopShelf[2].special_name, "tool_box");
    NuSpecialFind(WORLD->current_gscn, &TopShelf[2].special, "tool_box", 1);

    TopShelf[3].type = 3;
    NuStrCpy(TopShelf[3].name, "Code");
    NuStrCpy(TopShelf[3].special_name, "shop_question");
    NuSpecialFind(WORLD->current_gscn, &TopShelf[3].special, "shop_question", 1);

    TopShelf[4].type = 4;
    NuStrCpy(TopShelf[4].name, "Gold Bricks");
    NuStrCpy(TopShelf[4].special_name, "gold_brick");
    NuSpecialFind(things_scene, &TopShelf[4].special, "gold_brick", 1);

    TopShelf[5].type = 5;
    NuStrCpy(TopShelf[5].name, "Cut Scenes");
    NuStrCpy(TopShelf[5].special_name, "FMV");
    NuSpecialFind(world->current_gscn, &TopShelf[5].special, "fmv", 1);

    charcheatix = 0;
    memset(codelist, 0, sizeof(codelist));
    extracheatix = 0;
    SHOPCHARCOUNT = 0;
    i32 code_count = 0;
    for (i32 i = 0; i < ShopCollection.count_y && i < 100; ++i) {
        shopitem_s *item = &CharItems[SHOPCHARCOUNT];
        COLLECTID *collect = &ShopCollection.list[i];
        item->unlocked = 0;
        item->type = 1;

        if (((static_cast<u64>(Game.shop_character_purchased_bits[SHOPCHARCOUNT >> 5]) >> (SHOPCHARCOUNT & 0x1f)) &
             1) != 0) {
            item->unlocked = 1;
        }

        const i32 character_id = collect->id;
        if (CDataList[character_id].name_id == -1) {
            NuStrCpy(item->name, CDataList[character_id].dir);
        } else {
            NuStrCpy(item->name, TTab[CDataList[character_id].name_id]);
        }
        item = &CharItems[SHOPCHARCOUNT];
        memset(&item->special, 0, sizeof(item->special));

        if (code_count <= 143) {
            codelist[code_count] = CRC_ProcessStringIgnoreCase(collect->cheat_code);
            ++code_count;
        }
        ++SHOPCHARCOUNT;
    }

    UpdateCharacterIDs();
    charcheatix = code_count;
    for (i32 i = 0; i < 44; ++i) {
        if (code_count < 144)
            codelist[code_count++] = CRC_ProcessStringIgnoreCase(Cheat[i].code);
    }
    extracheatix = code_count;
    NuSpecialFind(things_scene, &iconback, "icon_back_neutral", 1);

    NuSpecialFind(WORLD->current_gscn, &infoblank, "info_blank", 1);
    NuSpecialFind(WORLD->current_gscn, &cutblank, "fmv_blank", 1);
    NuSpecialFind(WORLD->current_gscn, &cutfilm_unlocked, "shop_film1", 1);
    NuSpecialFind(WORLD->current_gscn, &cutfilm_locked, "shop_film1b", 1);
    NuSpecialFind(WORLD->current_gscn, &toolblank, "tool_blank", 1);
    NuSpecialFind(WORLD->current_gscn, &codeblank, "code_blank", 1);
    NuSpecialFind(things_scene, &question, "question_icon", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow1, "shop_arrow1", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow2, "shop_arrow2", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow3, "shop_arrow3", 1);
    NuSpecialFind(WORLD->current_gscn, &arrow4, "shop_arrow4", 1);
    NuSpecialSetVisibility(&arrow1, 0);
    NuSpecialSetVisibility(&arrow2, 0);
    NuSpecialSetVisibility(&arrow3, 0);
    NuSpecialSetVisibility(&arrow4, 0);

    InitExtraList();
    InitAlphaList();

    for (i32 i = 0; i < SHOPHINTCOUNT; ++i) {
        HINT_s *hint = Hint_FindHint(HintTab[i]);
        shopitem_s *item = &HintItems[i];
        item->type = 0;
        item->item_id = i;
        item->price = hint != NULL ? hint->shop_price : 0;
        item->unlocked = SAVE_OFF;
        if ((static_cast<u64>(Game.shop_hint_purchased_bits[i >> 5]) >> (i & 31)) & 1)
            item->unlocked = SAVE_ON;
        if (item->price == 0) {
            item->unlocked = 1;
        }
        item->special = atoz0to9icon[i];
    }

    shopcamspline = NuSplineFind(WORLD->current_gscn, const_cast<char *>("shop_cam"));
    shopcamlookat = shopcamspline != NULL ? shopcamspline->pts + 1 : NULL;
    shopcampos = shopcamspline != NULL ? shopcamspline->pts : NULL;
    LoadShelfSplines();

    if (SHOPCHARCOUNT > 0) {
        for (i32 i = 0; i < 7; ++i) {
            CharShelfIds[i] = (SHOPCHARCOUNT + i - 3) % SHOPCHARCOUNT;
            CharCurPos[i] = SubShelfPos[i];
        }
    }
    if (SHOPHINTCOUNT > 0) {
        for (i32 i = 0; i < 7; ++i) {
            HintShelfIds[i] = (SHOPHINTCOUNT + i - 3) % SHOPHINTCOUNT;
            HintCurPos[i] = SubShelfPos[i];
        }
    }
    if (SHOPEXTRACOUNT > 0) {
        for (i32 i = 0; i < 7; ++i) {
            ExtraShelfIds[i] = (SHOPEXTRACOUNT + i - 3) % SHOPEXTRACOUNT;
            ExtraCurPos[i] = SubShelfPos[i];
        }
    }

    const i32 brick_shelf_ids[7] = {11, 12, 13, 0, 1, 2, 3};
    for (i32 i = 0; i < 7; ++i) {
        BrickShelfIds[i] = brick_shelf_ids[i];
        BrickCurPos[i] = SubShelfPos[i];
    }

    for (i32 i = 0; i < SHOPGOLDBRICKS; ++i) {
        shopitem_s *item = &BrickItems[i];
        item->item_id = i;
        item->type = 4;
        item->price = 10000 + i * 5000;
        item->unlocked = SAVE_OFF;
        if ((static_cast<u64>((&Game.shop_gold_brick_purchased_bits)[i >> 5]) >> (i & 31)) & 1)
            item->unlocked = SAVE_ON;
        item->special = TopShelf[4].special;
    }

    SHOPCUTCOUNT = CutScenePlayCount;
    for (i32 i = 0; i < CutScenePlayCount && i < 128; ++i) {
        shopitem_s *item = &CutItems[i];
        item->item_id = i;
        item->type = 5;
        item->price = 0;
        item->unlocked = 0;
        item->special = TopShelf[5].special;
    }

    if (shop_from_cutsceneplayer == 0 && CutScenePlayCount > 0) {
        for (i32 i = 0; i < 7; ++i) {
            CutShelfIds[i] = (CutScenePlayCount + i - 3) % CutScenePlayCount;
            CutCurPos[i] = SubShelfPos[i];
        }
    }

    const f32 shelf_scale[6] = {0.9f, 0.28f, 0.9f, 0.95f, 0.8f, 0.9f};
    const f32 shelf_push[6] = {0.045f, 0.045f, 0.045f, 0.05f, 0.015f, 0.045f};
    const f32 current_push[6] = {0.0f, 0.0f, -0.005f, 0.0f, -0.01f, 0.0f};
    memcpy(TopShelfScale, shelf_scale, sizeof(TopShelfScale));
    memcpy(topscale, shelf_scale, sizeof(topscale));
    memcpy(TopShelfPush, shelf_push, sizeof(TopShelfPush));
    memcpy(TopBigPush, current_push, sizeof(TopBigPush));
    memcpy(toppush, current_push, sizeof(toppush));
    const f32 big_scale[6] = {1.44f, 0.448f, 1.44f, 1.5675f, 1.35f, 1.44f};
    memcpy(TopBigScale, big_scale, sizeof(TopBigScale));
    SubBigCharPush = 0.05f;
    SubNormCharPush = 0.02f;
    subpush[0] = subpush[2] = SubNormCharPush;
    subpush[1] = SubBigCharPush;
    for (i32 i = 0; i < 6; ++i) {
        codebigscale[i] = 1.254f;
        codeshelfscale[i] = codemenuscale[i] = 0.76f;
    }
    picked = oldpicked;
    subpicked = 0;
    scalenorm = 1.0f;
    scalepicked = 1.4f;
    scale2 = scale4 = scalenorm;
    scale3 = scalepicked;
    shopcutsceneplayer = CutScenePlayer_Available();
}

void GetShopCamLookPos(nuvec_s *position) {
    if (menuptr == ItemMenu && splshelf != NULL && picked != -1)
        *position = splshelf->pts[picked];
    else
        *position = *shopcamlookat;
}

i32 CheckCash(shopitem_s *items, i32 item) {
    return Game.coins >= items[item].price;
}

i32 BuyShopItem(shopitem_s *items, i32 index, i32 charge) {
    shopitem_s *item = &items[index];
    u32 *bits = NULL;
    if (item->type == 0)
        bits = Game.shop_hint_purchased_bits;
    else if (item->type == 1)
        bits = Game.shop_character_purchased_bits;
    else if (item->type == 2)
        bits = Game.extra_unlocked_bits;
    else if (item->type == 4)
        bits = &Game.shop_gold_brick_purchased_bits;
    else if (item->type == 5) {
        PlaySfx("MenuSelect", 0);
        return 1;
    }
    if (bits != NULL)
        bits[index / 32] |= static_cast<u32>(u64(1) << (index % 32));
    if (charge != 0)
        Game.coins -= item->price;
    pickedbing = 0.35f;
    item->unlocked = 1;
    if (charge != 0)
        PlaySfx("Shop_BuyCheat", &SubShelfPos[3]);
    return 1;
}

i32 SelectShopItem(shopitem_s *items, i32 index) {
    shopitem_s *item = &items[index];
    char event[128];
    i32 id, area;
    switch (item->type) {
        case 0: {
            id = static_cast<u16>(item->item_id);
            if (item->unlocked == 1 || item->price == 0)
                return 1;
            if (!CheckCash(items, index))
                break;
            AddToCompletionPoints(POINTS_PER_HINT);
            if (HINT_s *hint = Hint_FindHint(HintTab[id]))
                hint->flags |= HINT_SHOP_PURCHASED;
            goto buy;
        }
        case 1: {
            id = static_cast<u16>(item->item_id);
            if (item->unlocked == 1 || id >= CHARCOUNT)
                return 0;
            if (!CollectIDUnlocked(id))
                goto locked;
            if (!CheckCash(items, index))
                break;
            if (!AddToCollection(id))
                return 1;
            AddToCompletionPoints(POINTS_PER_CHARACTER);
            sprintf(event, "hubshop_buychar_%s", CDataList[id].file);
            NuIOS_RecordFlurryEvent(event);
            BuyShopItem(items, index, 1);
            return 1;
        }
        case 2: {
            id = static_cast<u16>(item->item_id);
            if (item->unlocked == 1 || ((Game.extra_purchased_bits[id >> 5] >> (id & 31)) & 1))
                return 0;
            area = static_cast<i8>(Cheat[id].area);
            if (area != -1 && !Game.area_save[area].red_brick_collected)
                goto locked;
            if (!CheckCash(items, index))
                break;
            Game.extra_purchased_bits[id >> 5] |= 1U << (id & 31);
            AddToCompletionPoints(POINTS_PER_CHEAT);
            sprintf(event, "hubshop_buyextra_%s", Cheat[id].name);
            NuIOS_RecordFlurryEvent(event);
            BuyShopItem(items, index, 1);
            return 1;
        }
        case 4:
            if (item->unlocked == 1 || item->price == 0)
                return 0;
            if (static_cast<f32>(index * 3600) > Game.field30_0x7c2c)
                goto locked;
            if (!CheckCash(items, index))
                break;
            AddToCompletionPoints(POINTS_PER_GOLDBRICK);
            AddToGoldBricks();
            sprintf(event, "hubshop_buygoldbrick_%i", index + 1);
            NuIOS_RecordFlurryEvent(event);
        buy:
            BuyShopItem(items, index, 1);
            return 1;
        case 5:
            if (!CutScenePlayer_CanStart(index)) {
                PlaySfx("MenuNoEntry", 0);
                ShopLockedScale = 1.5f;
                return 0;
            }
            oldmenuptr = menuptr;
            for (i32 i = 0; i < 3; ++i) {
                olddrawparent[i] = drawparent[i];
                oldmenuparent[i] = menuparent[i];
            }
            olddrawpanelptr = drawpanelptr;
            olddrawptr = drawptr;
            oldcurrentmenulevel = currentmenulevel;
            oldcurrentdrawlevel = currentdrawlevel;
            oldpickedscale = topscale[5];
            oldpickedpush = toppush[5];
            menuptr = NULL;
            for (i32 i = 0; i < 3; ++i) {
                drawparent[i] = NULL;
                menuparent[i] = NULL;
            }
            currentmenulevel = currentdrawlevel = 0;
            oldpicked = picked;
            SHOPACTIVE = 0;
            BuyShopItem(items, index, 0);
            TempGame.field30_0x7c2c = Game.field30_0x7c2c;
            pickedbing = 0.0f;
            if (memcmp(&TempGame, &Game, sizeof(Game)))
                hub_forceshopsave = 1;
            CutScenePlayer_Start(index, -1);
            return 1;
        default:
            return -1;
    }
    CoinTotalScale = 1.5f;
    return -1;
locked:
    GameAudio_PlaySfx(0x32, NULL, 0, 0);
    ShopLockedScale = 1.5f;
    return 0;
}

i32 MoveSubItemsRight(i32 *ids, NUVEC *positions, i32 count) {
    if (slidetimer > 0.0f) {
        const f32 phase = 1.0f - slidetimer * 8.0f;
        const i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
        f32 t = 1.0f - (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
        for (i32 i = 1; i < 7; ++i) {
            t = NuFmax(0.0f, NuFmin(t, 1.0f));
            positions[i].x = SubShelfPos[i].x + (SubShelfPos[i - 1].x - SubShelfPos[i].x) * t;
            positions[i].y = SubShelfPos[i].y + (SubShelfPos[i - 1].y - SubShelfPos[i].y) * t;
            positions[i].z = SubShelfPos[i].z + (SubShelfPos[i - 1].z - SubShelfPos[i].z) * t;
        }
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale3 = scalepicked + (scalenorm - scalepicked) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale4 = scalenorm + (scalepicked - scalenorm) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[1] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[2] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * t;
        scaleoverride[6] = phase;
        scaleoverride[0] = 1.0f - phase;
        return 0;
    }
    for (i32 i = 0; i < 6; ++i)
        ids[i] = ids[i + 1];
    ids[6] = ids[5] + 1;
    if (ids[6] >= count)
        ids[6] = 0;
    for (i32 i = 0; i < 7; ++i)
        positions[i] = SubShelfPos[i];
    slidetimer = 0.0f;
    scale3 = scalepicked;
    scale2 = scalenorm;
    scale4 = scalenorm;
    subpush[0] = SubNormCharPush;
    subpush[1] = SubBigCharPush;
    subpush[2] = SubNormCharPush;
    --moveitems;
    scaleoverride[0] = 0.0f;
    scaleoverride[1] = 0.4f;
    scaleoverride[2] = 0.8f;
    scaleoverride[4] = 0.8f;
    scaleoverride[5] = 0.4f;
    scaleoverride[6] = 0.0f;
    return 1;
}

i32 MoveSubItemsLeft(i32 *ids, NUVEC *positions, i32 count) {
    if (slidetimer > 0.0f) {
        const f32 phase = 1.0f - slidetimer * 8.0f;
        const i32 angle = static_cast<i32>(phase * 32768.0f + 16384.0f);
        f32 t = 1.0f - (NuTrigTable[(angle >> 1) & 0x7fff] + 1.0f) * 0.5f;
        for (i32 i = 0; i < 6; ++i) {
            t = NuFmax(0.0f, NuFmin(t, 1.0f));
            positions[i].x = SubShelfPos[i].x + (SubShelfPos[i + 1].x - SubShelfPos[i].x) * t;
            positions[i].y = SubShelfPos[i].y + (SubShelfPos[i + 1].y - SubShelfPos[i].y) * t;
            positions[i].z = SubShelfPos[i].z + (SubShelfPos[i + 1].z - SubShelfPos[i].z) * t;
        }
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale2 = scalenorm + (scalepicked - scalenorm) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        scale3 = scalepicked + (scalenorm - scalepicked) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[0] = SubNormCharPush + (SubBigCharPush - SubNormCharPush) * t;
        t = NuFmax(0.0f, NuFmin(t, 1.0f));
        subpush[1] = SubBigCharPush + (SubNormCharPush - SubBigCharPush) * t;
        scaleoverride[0] = phase;
        scaleoverride[6] = 1.0f - phase;
        return 0;
    }
    for (i32 i = 6; i > 0; --i)
        ids[i] = ids[i - 1];
    ids[0] = ids[1] - 1;
    if (ids[0] < 0)
        ids[0] = count - 1;
    for (i32 i = 0; i < 7; ++i)
        positions[i] = SubShelfPos[i];
    slidetimer = 0.0f;
    scale2 = scalenorm;
    scale3 = scalepicked;
    scale4 = scalenorm;
    subpush[0] = SubNormCharPush;
    subpush[1] = SubBigCharPush;
    subpush[2] = SubNormCharPush;
    ++moveitems;
    scaleoverride[0] = 0.0f;
    scaleoverride[1] = 0.4f;
    scaleoverride[2] = 0.8f;
    scaleoverride[4] = 0.8f;
    scaleoverride[5] = 0.4f;
    scaleoverride[6] = 0.0f;
    return 1;
}

void SelectSubItem() {
    shopitem_s *items;
    i32 index;
    switch (picked) {
        case 0:
            items = HintItems;
            index = HintShelfIds[3];
            break;
        case 1:
            items = CharItems;
            index = CharShelfIds[3];
            break;
        case 2:
            items = ExtraItems;
            index = ExtraShelfIds[3];
            break;
        case 4:
            items = BrickItems;
            index = BrickShelfIds[3];
            break;
        case 5:
            items = CutItems;
            index = CutShelfIds[3];
            break;
        default:
            return;
    }
    if (index != -1 && items != NULL) {
        const i32 result = SelectShopItem(items, index);
        if (result == -1)
            PlaySfx("Shop_NotEnufMuny", &SubShelfPos[3]);
        if (result == 1)
            ShopNameAlpha = 0.0f;
    }
}

static inline i32 SubItemMenu(MENU_s *menu) {
    static i32 movesfxlock;
    SHOPINPUT input;
    Shop_GetInput(&input);
    if (hintdrawwait > 0.0f) {
        hintdrawwait -= FRAMETIME;
        ShopNameAlpha = 0.0f;
    }
    if (ExitMenu)
        ShopNameAlpha = 0.0f;

    shopitem_s *items = NULL;
    i32 *ids = NULL, *count = NULL;
    f32 ypush = 0.0f;
    NUVEC *positions = NULL;
    if (picked == 0) {
        items = HintItems;
        ids = HintShelfIds;
        positions = HintCurPos;
        count = &SHOPHINTCOUNT;
        if (hintdrawwait <= 0.0f && (items[ids[3]].unlocked == 1 || items[ids[3]].price == 0)) {
            Hint_ResetHint(HintTab[static_cast<u16>(items[ids[3]].item_id)], 0);
            Hint_SetHintFromId(HintTab[static_cast<u16>(items[ids[3]].item_id)], 1, 0);
        } else
            Hint_CancelCurrent();
        ypush = SubNormCharPush;
    } else if (picked == 1) {
        items = CharItems;
        ids = CharShelfIds;
        positions = CharCurPos;
        count = &SHOPCHARCOUNT;
        ypush = SubNormCharPush;
    } else if (picked == 2) {
        items = ExtraItems;
        ids = ExtraShelfIds;
        positions = ExtraCurPos;
        count = &SHOPEXTRACOUNT;
        ypush = SubNormCharPush;
    } else if (picked == 4) {
        items = BrickItems;
        ids = BrickShelfIds;
        positions = BrickCurPos;
        count = &SHOPGOLDBRICKS;
        ypush = SubNormCharPush;
    } else if (picked == 5) {
        items = CutItems;
        ids = CutShelfIds;
        positions = CutCurPos;
        count = &SHOPCUTCOUNT;
        ypush = SubNormCharPush;
    }
    if (__builtin_expect(slidetimer >= 0.0f, 1)) {
        slidetimer -= FRAMETIME;
        if (__builtin_expect(slidetimer < 0.0f, 0))
            ShopNameAlpha = 0.0f;
    }
    if (easesubin != 0) {
        const f32 factor = 1.0f - ShopSinePhase(1.0f - slidetimer * 8.0f);
        if (easesubin == 1)
            inoutscale = 0.0f + ShopClamp01(factor);
        else if (easesubin == -1)
            inoutscale = 1.0f - ShopClamp01(factor);
    }

    i32 left = 0, right = 0;
    if (moveitems != 0) {
        if (movesfxlock == 0) {
            movesfxlock = 1;
            GameAudio_PlaySfx(0x2f, positions, 0, 0);
        }
        const i32 previous = moveitems;
        i32 moved = 0;
        if (previous < 0) {
            moved = MoveSubItemsLeft(ids, positions, *count);
            left = moveitems != 0 && previous != moveitems;
        } else if (__builtin_expect(previous > 0, 1)) {
            moved = MoveSubItemsRight(ids, positions, *count);
            right = moveitems != 0 && previous != moveitems;
        }
        if (__builtin_expect(moved && items != NULL, 0)) {
            shopitem_s *selected = &items[ids[3]];
            if (selected->type == 0) {
                if (selected->unlocked == 1)
                    SelectSubItem();
            }
        }
    } else
        movesfxlock = 0;

    i32 cancel = 0, confirm = 0;
    if (__builtin_expect(slidetimer < 0.0f, 0)) {
        easesubin = 0;
        if (input.left_held)
            left = 1;
        else if (input.right_held)
            right = 1;
        else if (input.confirm && !ExitMenu)
            confirm = 1;
        else if (input.cancel)
            cancel = 1;
    }
    if (subitemselected == 1)
        subitemselected = 2;
    if (menu->confirm_pressed) {
        if (menu->selected_row == 1) {
            if (menu->selected_column == 3)
                confirm = 1;
            else {
                moveitems = menu->selected_column - 3;
                if (moveitems < 0)
                    left = 1;
                else if (moveitems > 0)
                    right = 1;
            }
        } else if (menu->selected_row == 2 && subitemselected > 0) {
            if (menu->selected_column == 0)
                confirm = 1;
            else {
                subitemselected = 0;
                PlaySfx("MenuSelect", 0);
            }
        }
    }
    if (menu->left_pressed && subitemselected == 0) {
        moveitems = -static_cast<i32>(menu->horizontal_scroll_distance / 0.15f);
        left = 1;
    }
    if (__builtin_expect(menu->right_pressed && subitemselected == 0, 0)) {
        moveitems = static_cast<i32>(menu->horizontal_scroll_distance / 0.15f);
        right = 1;
    }

    i32 scrolling = 0;
    if (!ExitMenu) {
        if (left || right) {
            if (left) {
                scaleoverride[0] = 0.0f;
            } else {
                scaleoverride[0] = 1.0f;
            }
            scaleoverride[1] = 0.4f;
            scaleoverride[2] = 0.8f;
            scaleoverride[4] = 0.8f;
            scaleoverride[5] = 0.4f;
            scaleoverride[6] = left ? 1.0f : 0.0f;
            NUVEC pos = SubShelfPos[left ? 0 : 6];
            pos.y += ypush;
            AddGameDebris(WORLD->debris_sys, qrand() / 10923 + 76, &pos);
            pos = SubShelfPos[left ? 6 : 0];
            pos.y += ypush;
            AddGameDebris(WORLD->debris_sys, qrand() / 10923 + 76, &pos);
            if (moveitems == 0)
                moveitems = left ? -1 : 1;
            slidetimer = 0.125f;
            hintdrawwait = 0.2f;
            scrolling = 1;
        }
        if (cancel) {
            easesubin = -1;
            slidetimer = 0.125f;
            ExitMenu = 1;
            GameAudio_PlaySfx(0x31, NULL, 0, 0);
        }
        if (confirm) {
            if (subitemselected > 0) {
                SelectSubItem();
                subitemselected = 0;
            } else
                subitemselected = 1;
        }
    }
    if (__builtin_expect(ExitMenu && slidetimer < 0.0f, 0)) {
        ExitMenu = 0;
        easesubin = -1;
        lastitem = -1;
        slidetimer = 0.125f;
        GameCam_Blend(GameCam, 0.6f, 0.0f, 1);
        ShopNameAlpha = 0.0f;
        return 6;
    }
    scrollkeyhit = scrolling;
    return 0;
}

i32 DoShopMenu(MENU_s *menu) {
    i32 done = 0;
    if (menuptr != NULL) {
        i32 result = menuptr(menu);
        if (menu->cancel_pressed != 0) {
            result = currentmenulevel == 0 ? 5 : 6;
            for (i32 i = 4; i < 13; ++i) {
                menu->item_width[i] = 0.0f;
            }
            subitemselected = 0;
        }

        if (result == 5) {
            done = 1;
        } else if (result == 6) {
            GameCam_Blend(GameCam, 0.6f, 0.0f, 1);
            drawptr = NULL;

            menuptr = menuparent[currentmenulevel];
            menuparent[currentmenulevel] = NULL;
            --currentmenulevel;
            if (currentmenulevel < 0) {
                currentmenulevel = 0;
            }

            drawpanelptr = drawparent[currentdrawlevel];
            drawparent[currentdrawlevel] = NULL;
            --currentdrawlevel;
            if (currentdrawlevel < 0) {
                currentdrawlevel = 0;
            }
        }
    }
    return done;
}

i32 UpdateShop(MENU_s *menu) {
    UpdateCharacterIDs();

    scaleoverride[0] = 0.0f;
    scaleoverride[1] = 0.4f;
    scaleoverride[2] = 0.8f;
    scaleoverride[4] = 0.8f;
    scaleoverride[5] = 0.4f;
    scaleoverride[6] = 0.0f;

    if (1.0f > ShopNameAlpha) {
        ShopNameAlpha += FRAMETIME + FRAMETIME;
        if (ShopNameAlpha > 1.0f) {
            ShopNameAlpha = 1.0f;
        }
    }

    ShopLockedScale = SeekLinearF(ShopLockedScale, 1.0f, FRAMETIME * 3.0f);
    SubNormCharPush = 0.02f;
    SubBigCharPush = 0.05f;

    if (menuptr == NULL) {
        if (shop_from_cutsceneplayer == 0) {
            menuptr = ItemMenu;
            ExitMenu = 0;
            slidetimer = 0.125f;
            enteredshop = 1;
            currentmenulevel = 0;
            currentdrawlevel = 0;
            menuparent[0] = NULL;
            drawparent[0] = NULL;
            picked = oldpicked;
            drawpanelptr = DrawItemMenu2D;
        } else {
            shop_from_cutsceneplayer = 0;
            topscale[5] = oldpickedscale;
            menuptr = oldmenuptr;
            toppush[5] = oldpickedpush;
            drawparent[0] = olddrawparent[0];
            menuparent[0] = oldmenuparent[0];
            drawparent[1] = olddrawparent[1];
            menuparent[1] = oldmenuparent[1];
            drawparent[2] = olddrawparent[2];
            menuparent[2] = oldmenuparent[2];
            drawpanelptr = olddrawpanelptr;
            drawptr = olddrawptr;
            currentmenulevel = oldcurrentmenulevel;
            currentdrawlevel = oldcurrentdrawlevel;
            picked = oldpicked;
        }
    }

    return DoShopMenu(menu) != 0;
}

void DrawItem(nuhspecial_s *special, nuvec_s *position, float scale_value, float, float y_push, u16 x_rot, u16 y_rot,
              u16 z_rot) {
    NUVEC scale;
    NUANGVEC rotation;
    NUMTX_ALIGNED16 matrix;
    if (position != NULL && NuSpecialExistsFn(special) != 0) {
        rotation.y = y_rot;
        rotation.x = x_rot;
        rotation.z = z_rot;
        NuMtxSetRotateXYZVU0(&matrix, &rotation);

        scale.x = scale.y = scale.z = scale_value;
        NuMtxScaleVU0(&matrix, &scale);
        *reinterpret_cast<NUVEC *>(&matrix.m30) = *position;
        matrix.m31 += y_push;
        NuSpecialDrawAt(special, &matrix);
    }
}

void DrawCodeMenu3D() {
    i32 letters[6];
    u16 angle = shelfang;
    for (i32 i = 0; i < 6; ++i) {
        if (usercode[i] >= 'A' && usercode[i] <= 'Z')
            letters[i] = usercode[i] - 'A';
        else if (usercode[i] >= '0' && usercode[i] <= '9')
            letters[i] = usercode[i] - '0' + 26;
    }
    f32 bing = 0.0f;
    if (pickedbing > 0.0f) {
        bing = pickedbing / 0.35f * 0.5f;
        pickedbing -= FRAMETIME;
    } else
        pickedbing = 0.0f;
    for (i32 i = 0; i < 6; ++i) {
        f32 scale = 0.95f * codemenuscale[i] * inoutscale;
        f32 push = SubNormCharPush;
        if (col == i) {
            scale += bing * 0.5f;
            push = subpush[1];
        } else if (lastitem == i) {
            scale += bing * 0.5f;
            push = subpush[2];
        }
        DrawItem(&codeblank, &CodePos[i], scale, 1.0f, push, 0, angle, 0);
        DrawItem(&atoz0to9icon[letters[i]], &CodePos[i], scale, 1.0f, push, 0, angle, 0);
    }
    static f32 arrowbing;
    arrowbing = 0.5f * NuTrigTable[(static_cast<i32>(inoutscale * 32768.0f) >> 1) & 0x7fff];
    f32 scale = inoutscale * 0.76f;
    Draw3DObject(WORLD, 317, &CodePos[6], 0, shelfang, 0, scale, scale, scale, 2);
    enum { SHOP_CODE_ARROW_MESSAGE_FLAGS = 0x83 };
    AddGameMessage(">", &CodePos[6], 0.65f * inoutscale, NULL, 0.0f, 255, 255, 255, SHOP_CODE_ARROW_MESSAGE_FLAGS,
                   0.0f);
}

void DrawQuestion(nuvec_s *position, float scale_value, float y_push) {
    if (position && NuSpecialExistsFn(&question)) {
        NUMTX_ALIGNED16 matrix;
        NuMtxSetIdentity(&matrix);
        NUVEC scale;
        scale.x = scale.y = scale.z = scale_value;
        NuMtxSetScale(&matrix, &scale);
        *reinterpret_cast<NUVEC *>(&matrix.m30) = *position;
        matrix.m31 += y_push;
        NuSpecialDrawAt(&question, &matrix);
    }
}

void DrawArrow(nuhspecial_s *special, float scale_value) {
    if (NuSpecialExistsFn(special)) {
        NUVEC scale = {scale_value, scale_value, scale_value};
        NUMTX matrix = *NuSpecialGetDrawMtx(special);
        NuMtxPreScale(&matrix, &scale);
        NuSpecialDrawAt(special, &matrix);
    }
}

void DrawTopShelf(i32) {
    NUMTX_ALIGNED16 item_matrix;
    NUMTX_ALIGNED16 brick_matrix;
    f32 alpha_scale = 1.0f;
    if (GetMenuID() == 13 && subitemselected == 0 && TestForController() == 0) {
        const f32 elapsed_since_touch = GlobalTimer.time_elapsed - (LastTouchTime + 1.32f);
        if (elapsed_since_touch > 4.0f) {
            const f32 phase = NuFmod(elapsed_since_touch, 4.0f);
            const i32 angle = static_cast<i32>(phase * 0.25f * 65536.0f);
            const f32 pulse = NuTrigTable[(angle >> 1) & 0x7fff] - 0.8f;
            alpha_scale = pulse < 0.0f ? 1.0f : pulse + 1.0f;
        }
    }

    Shop_DrawCharacter(&TopShelf[1], &ShelfPos[1], topscale[1] * alpha_scale, toppush[1] + 0.0f, 0, shelfang, 0);

    const f32 item_push = toppush[2];
    const f32 item_scale = topscale[2];
    const u16 item_angle = shelfang;
    if (NuSpecialExistsFn(&TopShelf[2].special) != 0) {
        NUMTX &matrix = item_matrix;
        NUANGVEC rotation = {0, item_angle, 0};
        NuMtxSetRotateXYZVU0(&matrix, &rotation);
        const f32 scale_value = item_scale * alpha_scale;
        NUVEC scale = {scale_value, scale_value, scale_value};
        NuMtxScaleVU0(&matrix, &scale);
        matrix.m30 = ShelfPos[2].x;
        matrix.m31 = item_push + 0.005f + ShelfPos[2].y;
        matrix.m32 = ShelfPos[2].z;
        NuSpecialDrawAt(&TopShelf[2].special, &matrix);
    }

    if (SHOPGOLDBRICKS > 0) {
        const f32 ypush = toppush[4];
        f32 scale_value = topscale[4];
        const u16 angle = shelfang;
        if (NuSpecialExistsFn(&TopShelf[4].special) != 0) {
            scale_value *= alpha_scale;
            NUVEC scale = {scale_value, scale_value, scale_value};
            NUMTX &matrix = brick_matrix;
            NuMtxSetScale(&matrix, &scale);
            NuMtxRotateXInline(&matrix, 0);
            NuMtxRotateYInline(&matrix, static_cast<u16>(angle + 0x2000));
            NuMtxRotateZInline(&matrix, 0);
            NuMtxTranslate(&matrix, &ShelfPos[4]);
            matrix.m31 += ypush - 0.0325f;
            NuSpecialDrawAt(&TopShelf[4].special, &matrix);
        }
    }

    if (__builtin_expect(GetMenuID() != 13, 0)) {
        return;
    }

    MENU *menu = &GameMenu[GameMenuLevel];
    for (i32 shelf_index = 1; shelf_index <= 4; ++shelf_index) {
        const i32 menu_index = shelf_index - 1;
        if (shelf_index == 3) {
            menu->item_width[menu_index] = 0.0f;
            continue;
        }

        NUVEC world_position;
        world_position.x = ShelfPos[shelf_index].x;
        world_position.y = ShelfPos[shelf_index].y;
        world_position.z = ShelfPos[shelf_index].z;
        world_position.y += toppush[shelf_index];
        const f32 size = topscale[shelf_index] / TopShelfScale[shelf_index] * 0.15f;
        const f32 aspect_ratio = GetAspectRatio();
        const f32 width = size / aspect_ratio;
        NUVEC screen_position;
        NuCameraTransformScreenClip(&screen_position, &world_position, 1, NULL);
        menu->item_x[menu_index] = screen_position.x;
        menu->item_y[menu_index] = screen_position.y;
        menu->item_width[menu_index] = size;
        menu->item_height[menu_index] = width;
        menu->item_column[menu_index] = menu_index;
        menu->item_row[menu_index] = 0;
    }
}

void DrawSubItems() {
    NUMTX_ALIGNED16 item_matrix_a;
    NUMTX_ALIGNED16 item_matrix_b;
    f32 alpha = 1.0f;
    if (GetMenuID() == 13 && !TestForController()) {
        const f32 since_touch = GlobalTimer.time_elapsed - (LastTouchTime + 2.64f);
        if (__builtin_expect(since_touch > 4.0f, 1)) {
            const f32 phase = NuFmod(since_touch, 4.0f);
            const i32 angle = static_cast<i32>(phase * 0.25f * 65536.0f);
            const f32 pulse = NuTrigTable[(angle >> 1) & 0x7fff] - 0.8f;
            alpha = pulse < 0.0f ? 1.0f : pulse + 1.0f;
        }
    }

    f32 bing = 0.0f;
    i32 phase_angle = 0x2000;
    if (__builtin_expect(pickedbing > 0.0f, 1)) {
        bing = pickedbing / 0.35f;
        pickedbing -= FRAMETIME;
        phase_angle = (static_cast<i32>(pickedbing * 32768.0f + 16384.0f) >> 1) & 0x7fff;
    } else {
        pickedbing = 0.0f;
    }
    f32 bing_scale = 1.0f - 0.5f * (NuTrigTable[phase_angle] + 1.0f);

    shopitem_s *items = NULL;
    NUVEC *positions = NULL;
    f32 item_scale;
    i32 *shelf_ids = NULL;
    f32 normal_push;
    switch (picked) {
        case 0:
            items = HintItems;
            shelf_ids = HintShelfIds;
            positions = HintCurPos;
            item_scale = 0.9f;
            normal_push = SubNormCharPush;
            break;
        case 1:
            items = CharItems;
            shelf_ids = CharShelfIds;
            positions = CharCurPos;
            item_scale = 0.28f;
            normal_push = SubNormCharPush;
            break;
        case 2:
            items = ExtraItems;
            shelf_ids = ExtraShelfIds;
            positions = ExtraCurPos;
            item_scale = 0.9f;
            normal_push = SubNormCharPush;
            break;
        case 3:
            return;
        case 4:
            items = BrickItems;
            shelf_ids = BrickShelfIds;
            positions = BrickCurPos;
            item_scale = 0.8f;
            normal_push = SubNormCharPush;
            break;
        case 5:
            items = CutItems;
            shelf_ids = CutShelfIds;
            positions = CutCurPos;
            item_scale = 0.9f;
            normal_push = SubNormCharPush;
            break;
        default:
            return;
    }
    MENU *menu = &GameMenu[GameMenuLevel];
    for (i32 index = 0; index < 7; ++index) {
        i32 slot = index;
        if (__builtin_expect(index == 6, 0)) {
            slot = 3;
        } else if (__builtin_expect(index >= 3, 1)) {
            slot = index - 1;
        }

        f32 scale = item_scale * inoutscale;
        f32 ypush = normal_push;
        u16 rotation = 0;
        switch (slot) {
            case 0:
                if (moveitems > 0)
                    continue;
                scale = item_scale * inoutscale * scaleoverride[0];
                ypush = normal_push * scaleoverride[0];
                break;
            case 1:
                if (moveitems > 0) {
                    scale = item_scale * inoutscale * scaleoverride[0];
                    ypush = normal_push * scaleoverride[0];
                }
                break;
            case 2:
                scale = item_scale * scale2 * inoutscale;
                ypush = subpush[0];
                break;
            case 3:
                if (pickedbing >= 0.0f)
                    rotation = static_cast<i32>(-bing * 65536.0f);
                scale = item_scale * scale3 * inoutscale;
                scale += 0.5f * bing_scale;
                ypush = subpush[1];
                break;
            case 4:
                scale = item_scale * scale4 * inoutscale;
                ypush = subpush[2];
                break;
        }
        if (__builtin_expect(!subitemselected, 0))
            scale *= alpha;
        shopitem_s *item = &items[shelf_ids[slot]];
        NUVEC position = positions[slot];
        if (subitemselected == 2 && slot == 3) {
            NuVecAdd(&position, &position, &selectedoff);
            scale *= 1.25f;
        }
        NUVEC hit_position = position;
        NUVEC screen;
        hit_position.y += ypush;
        f32 aspect = GetAspectRatio();
        NuCameraTransformScreenClip(&screen, &hit_position, 1, NULL);
        menu->item_x[slot + 4] = screen.x;
        menu->item_y[slot + 4] = screen.y;
        f32 hit_size = 0.0f;
        f32 hit_width = 0.0f;
        if (subitemselected < 1) {
            hit_size = (scale / item_scale) * 0.15f;
            hit_width = hit_size / aspect;
        }
        menu->item_width[slot + 4] = hit_size;
        menu->item_height[slot + 4] = hit_width;
        menu->item_column[slot + 4] = slot;
        menu->item_row[slot + 4] = 1;

        switch (picked) {
            case 0: {
                if (items[shelf_ids[slot]].unlocked == 1) {
                    if (pickedbing > 0.0f && slot == 3)
                        rotation += static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
                    else
                        rotation = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
                }
                const u16 angle = shelfang + (item->unlocked == 1 ? rotation : 0);
                if (items[shelf_ids[slot]].unlocked != 1) {
                    DrawItem(&infoblank, &position, scale, 1.0f, ypush, 0, angle, 0);
                    DrawItem(&TopShelf[3].special, &position, scale, 1.0f, ypush, 0, angle, 0);
                } else {
                    DrawItem(&TopShelf[0].special, &position, scale, 1.0f, ypush, 0, angle, 0);
                }
                break;
            }
            case 1: {
                u16 phase = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 1.75f) / 1.75f * 65536.0f);
                f32 oscillation = NuTrigTable[((phase + 0x4000) >> 1) & 0x7fff];
                rotation = item->unlocked == 1 ? static_cast<i32>(oscillation * 4369.0f) : 0;
                const u16 angle = shelfang + rotation;
                Shop_DrawCharacter(item, &position, scale, ypush, 0, angle, 0);
                break;
            }
            case 2: {
                u16 spin = rotation;
                if (items[shelf_ids[slot]].unlocked == 1) {
                    if (pickedbing > 0.0f && slot == 3)
                        spin += static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
                    else
                        spin = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
                    const u16 angle = shelfang + (item->unlocked == 1 ? spin : 0);
                    DrawItem(&toolblank, &position, scale, 1.0f, ypush, 0, angle, 0);
                    shopitem_s *current_item = &items[shelf_ids[slot]];
                    if (__builtin_expect(current_item != NULL, 1)) {
                        if (NuSpecialExistsFn(&current_item->special) != 0) {
                            NUANGVEC item_rotation;
                            
                            NUVEC item_scale;
                            item_rotation.y = angle;
                            item_rotation.x = 0;
                            item_rotation.z = 0;
                            NuMtxSetRotateXYZVU0(&item_matrix_a, &item_rotation);
                            item_scale.x = item_scale.y = item_scale.z = scale;
                            NuMtxScaleVU0(&item_matrix_a, &item_scale);
                            *reinterpret_cast<NUVEC *>(&item_matrix_a.m30) = position;
                            item_matrix_a.m31 += ypush;
                            NuSpecialDrawAt(&current_item->special, &item_matrix_a);
                        }
                    }
                } else {
                    const u16 angle = shelfang + (item->unlocked == 1 ? spin : 0);
                    DrawItem(&toolblank, &position, scale, 1.0f, ypush, 0, angle, 0);
                    const i32 current_id = shelf_ids[slot];
                    shopitem_s *current_item = &items[current_id];
                    const i32 id = static_cast<u16>(current_item->item_id);
                    if (!(Game.extra_purchased_bits[id >> 5] >> (id & 31) & 1) && id > 7 &&
                        static_cast<i8>(Cheat[id].area) != -1 &&
                        !Game.area_save[static_cast<i8>(Cheat[id].area)].red_brick_collected) {
                        if (NuSpecialExistsFn(&extrasils[current_id]) != 0)
                            DrawItem(&extrasils[shelf_ids[slot]], &position, scale, 1.0f, ypush, 0, angle, 0);
                    } else {
                        if (NuSpecialExistsFn(&current_item->special) != 0) {
                            NUANGVEC item_rotation;
                            
                            NUVEC item_scale;
                            item_rotation.y = angle;
                            item_rotation.x = 0;
                            item_rotation.z = 0;
                            NuMtxSetRotateXYZVU0(&item_matrix_b, &item_rotation);
                            item_scale.x = item_scale.y = item_scale.z = scale;
                            NuMtxScaleVU0(&item_matrix_b, &item_scale);
                            *reinterpret_cast<NUVEC *>(&item_matrix_b.m30) = position;
                            item_matrix_b.m31 += ypush;
                            NuSpecialDrawAt(&current_item->special, &item_matrix_b);
                        }
                    }
                }
                break;
            }
            case 4: {
                if (items[shelf_ids[slot]].unlocked == 1) {
                    if (pickedbing > 0.0f && slot == 3)
                        rotation += static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
                    else
                        rotation = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f);
                    rotation += static_cast<u16>(items[shelf_ids[slot]].item_id * 0x1249) - 0x2000;
                }
                const u16 angle = shelfang + (item->unlocked == 1 ? rotation : 0);
                nuhspecial_s *special = &items[shelf_ids[slot]].special;
                if (NuSpecialExistsFn(special) != 0) {
                    NUMTX_ALIGNED16 matrix;
                    NUVEC size = {scale, scale, scale};
                    NuMtxSetScale(&matrix, &size);
                    NuMtxRotateXInline(&matrix, 0);
                    NuMtxRotateYInline(&matrix, angle);
                    NuMtxRotateZInline(&matrix, 0);
                    NuMtxTranslate(&matrix, &position);
                    matrix.m31 += ypush;
                    NuSpecialDrawAt(special, &matrix);
                }
                break;
            }
            case 5: {
                nuhspecial_s *blank = &cutblank;
                nuhspecial_s *film = &cutfilm_locked;
                const i32 id = static_cast<u16>(items[shelf_ids[slot]].item_id);
                if (shopcutsceneplayer != NULL && CutScenePlayer_CanStart(id) != 0) {
                    blank = &toolblank;
                    CUTSCENEPLAYER_s *clips = static_cast<CUTSCENEPLAYER_s *>(shopcutsceneplayer);
                    LEVELDATA *level = &LDataList[clips->clips[id].level_id];
                    if (level->episode_index != -1) {
                        if (level->episode_index & 1)
                            blank = &codeblank;
                    } else if (level->area_index != -1 && ANEWHOPE_ADATA && level->area_index == ANEWHOPE_ADATA->index)
                        blank = &codeblank;
                    rotation = static_cast<i32>(NuFmod(GameTimer.time_elapsed, 4.0f) * 0.25f * 65536.0f) +
                               static_cast<u16>(id * 0x1555);
                    film = &cutfilm_unlocked;
                }
                const u16 angle = shelfang + (item->unlocked == 1 ? rotation : 0);
                DrawItem(blank, &position, scale, 1.0f, ypush, 0, angle, 0);
                DrawItem(film, &position, scale, 1.0f, ypush, 0, angle, 0);
                break;
            }
            default:
                break;
        }
    }
}

void DrawSubItemMenu3D() {
    DrawSubItems();
}

void DrawShop3D(WORLDINFO_s *world) {
    if (drawptr != NULL) {
        drawptr();
    }
    DrawTopShelf(picked);
    SetLevelLights(world->rtl_set, 1.0f);
}

void DrawShopPanel() {
    if (SHOPACTIVE && drawpanelptr)
        drawpanelptr();
}

void DrawShopPrompts() {
    i32 select = -1;
    i32 back = -1;
    f32 alpha = 1.0f;
    if (menuptr == ItemMenu) {
        select = tSELECT;
        back = tEXIT;
    } else if (menuptr == SubItemMenu) {
        back = tBACK;
        select = picked == 5 ? tPLAY : tBUY;
        alpha = 0.25f;
        switch (picked) {
            case 0: {
                shopitem_s *item = &HintItems[HintShelfIds[3]];
                if (item->price <= Game.coins && item->unlocked != 1)
                    alpha = 1.0f;
                break;
            }
            case 1: {
                shopitem_s *item = &CharItems[CharShelfIds[3]];
                if (item->price <= Game.coins && CollectIDUnlocked(static_cast<u16>(item->item_id)) &&
                    item->unlocked != 1)
                    alpha = 1.0f;
                break;
            }
            case 2: {
                shopitem_s *item = &ExtraItems[ExtraShelfIds[3]];
                u16 id = item->item_id;
                i8 area = static_cast<i8>(Cheat[id].area);
                if (item->price <= Game.coins && (id <= 7 || area == -1 || Game.area_save[area].red_brick_collected) &&
                    !(Game.extra_purchased_bits[id >> 5] >> (id & 31) & 1) && item->unlocked != 1)
                    alpha = 1.0f;
                break;
            }
            case 4: {
                i32 id = BrickShelfIds[3];
                if (BrickItems[id].price <= Game.coins && static_cast<f32>(id * 3600) <= Game.field30_0x7c2c)
                    alpha = 1.0f;
                break;
            }
            case 5:
                if (CutScenePlayer_CanStart(CutShelfIds[3]))
                    alpha = 1.0f;
                break;
        }
    } else if (menuptr == CodeMenu) {
        select = tSELECT;
        back = tBACK;
    }
    if (netclient)
        select = -1;
    if (select != -1 || back != -1)
        DrawPlayerIconPrompts(MenuPacket.active_player[0], select, alpha, -1, back, -1, tSELECTING,
                              MenuPacket.active_player[1], select, alpha, -1, back, -1, tSELECTING);
}

void Shop_CollectAllCharacters(i32 mode) {
    if (mode != 0)
        return;
    const i32 count = ShopCollection.count_y;
    for (i32 i = 0; i != count && i != 100; ++i) {
        Game.shop_character_purchased_bits[i >> 5] |= static_cast<u32>(u64(1) << (i & 31));
        if (WORLD->current_level == HUB_LDATA)
            CharItems[i].unlocked = SAVE_ON;
    }
}

