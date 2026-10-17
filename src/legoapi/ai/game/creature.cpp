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
#include "nu2api/numath/numtx.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/render/fx.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/ai/game/creature_types.h"
#include "legoapi/render/core/render.h"
#include "legoapi/actions/character/transform.h"
#include "legoapi/actions/movement/carrying.h"
#include "legoapi/actions/character/snake.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/render/fx/edsplines.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/numath/nutrig.h"
#include "gameapi/gui/apimenu.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/characters/motion/animlist.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/items/base/collection.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/render/core/SwipeDecalRenderer.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/android/nugscn_android.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nugcutscene.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nupostparams.h"
#include "nu2api/nu3d/nuportal.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/gizmos/transport/grapples.h"
#include "legoapi/gizmos/trigger/ai.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/props/objects/techno.h"
#include "legoapi/actions/character/specialmoves.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/characters/core/characters_types.h"
#include "legoapi/actions/combat/hits.h"
#include "legoapi/render/core/terrain_internal.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/ai/core/legoai.h"
#include "legoapi/render/core/gameliball.h"
#include <new>
#include "legoapi/gizmo/object/gizmopickup.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nuvec4.h"
#include <limits.h>
#include "legoapi/gizmo/base/gizactions.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nufile/nufpar.h"
#include <stdlib.h>
#include "legoapi/core/net/netplay.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/props/system/socksys.h"
#include "nu2api/nucore/nutime.h"
#include "legoapi/characters/core/players_types.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/render/light/shadow.h"
#include "legoapi/render/fx/spline_position.h"
#include "nu2api/nucore/nupad.h"
#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/gizmos/traps/gizforce.h"
#include "legoapi/actions/movement/jumping.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/gizmos/door/securitydoors.h"
#include <float.h>
#include "legoapi/gizmo/base/GizObstacleObjectInterface.h"
#include "legoapi/gizmo/base/GizForceObjectInterface.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include <stdarg.h>
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/actions/character/legoapi_tag.h"
#include "legoapi/actions/combat/shovesys.h"
#include "legoapi/actions/movement/climbing.h"
#include "legoapi/actions/movement/dropinout.h"
#include "legoapi/ai/core/aisysall.h"
#include "legoapi/ai/game/starwars_gameai.h"
#include "legoapi/characters/core/characters.h"
#include "legoapi/characters/motion/charplatforms.h"
#include "legoapi/characters/motion/charshadows.h"
#include "legoapi/characters/motion/move.h"
#include "legoapi/items/collect/batarang.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/items/collect/shard.h"
#include "legoapi/items/objects/game_obj.h"
#include "legoapi/lib/camera.h"
#include "legoapi/misc/utilities.h"

struct AIROW_s;
struct nunativegscene_s;
struct SHOPINPUT;

struct starfighter_s;
struct rtl_s;
struct rtlidata_s;

i32 MatrixReflection(NUMTX *matrix, i32 axis, f32 plane, f32 height, NUMTX *result);

i32 draw_para = 1;

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

extern i16 tPOWERBRICK, tLOCKED, tGOLDBRICK;
extern void (*DisguiseAdjustFn)(i32, i32, NUVEC *, NUVEC *);

u8 CharClipToBlobShadows;

static inline void DrawSetRotationY(NUMTX *matrix, NUANG angle) {
    const f32 cosine = NU_COS_LUT(angle);
    const f32 sine = NU_SIN_LUT(angle);
    NUMTX value = {cosine, 0.0f, -sine, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, sine, 0.0f, cosine, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    *matrix = value;
}

extern i16 id_ANAKINJEDISCARRED, id_WOOKIEE, id_TWOFACE;

struct AISYS_s;

extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;
extern "C" i32 waiting_for_character;

extern "C" {
    i32 hub_character_ready = -1;
}

static u32 LayerBit(u8 layer) {
    return 1u << layer;
}

#if defined(__i386__)
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone, regparm(1)))
#else
#define MOSEISLEY_LAYERS_ATTR __attribute__((noinline, noclone))
#endif
static MOSEISLEY_LAYERS_ATTR void SetLayers_MOSEISLEYCITIZEN(u32 *layers)
    __asm__("_ZL26SetLayers_MOSEISLEYCITIZENP12GameObject_s.isra.2");

struct tertype;
struct terrsitu_s;
struct PLATSKININFO;
struct GAMECAMERA_s;
struct pushblock_s;
i32 NOTERRAINSTOP;
extern "C" i16 id_SNAKE;

i32 testshadowfix = 1;

namespace {
#include "legoapi/render/core/terrain_types.h"

} // namespace

namespace {

} // namespace

namespace {

} // namespace

#if defined(__i386__)
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone, regparm(2)))
#else
#define TERRAIN_KILL_PLAYER_ATTR __attribute__((noinline, noclone))
#endif
static TERRAIN_KILL_PLAYER_ATTR i32 TerrainKillPlayer(GameObject_s *object, i32 surface, const f32 *normal_y)
    __asm__("_ZL17TerrainKillPlayerP12GameObject_siP7nuvec_s.isra.4");

namespace {

} // namespace

namespace {

} // namespace

// Original 0x378fb0: the optimized rotating scan shares the terrain engine state.

namespace {

} // namespace

namespace {

} // namespace

struct MechTouchUIElement;
struct PLAYERITEM_s;
struct PLAYERITEMTYPE_s;
struct PLAYERPACKET_s;
struct SOCKPOSITION_s;

extern i32 complexsockposition_forcesock;
extern i32 movegamecamera_forcesock;
extern i32 LevFlag[4];

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

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

static inline PlayerAIFlagBits *PlayerAIFlags(u8 *object, i32 offset) {
    return reinterpret_cast<PlayerAIFlagBits *>(object + offset);
}

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

extern i16 id_GAMORREANGUARD;
extern i16 id_MOUSEDROID;

extern "C" {
    extern i16 id_SNAKE;
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

    static_assert(sizeof(lego_aiconditiondefs) / sizeof(lego_aiconditiondefs[0]) == 178,
                  "complete game AI condition registry");

}

struct CUTSCENEPLAYERCLIP;

extern i16 id_BUGGY, id_RADIOCAR, id_POLICECAR, id_MOWER;
extern i16 id_ANAKINJEDISCARRED;

// ThingManager objects return to the theMemoryManager pool (D0 @0x425a50).
inline void ThingManager::operator delete(void *thing, usize size) {
    theMemoryManager.FreePool(thing, size);
}

extern f32 Hub_PadSpeed[2];
void UpdateRumble(RUMBLEPACKET *);

GameObject_s *FindNearestGameObject(NUVEC *position, GameObject_s *exclude, u32 required_flags, f32 radius,
                                    f32 extra_radius, i32 animation, i32 character_id, i32 player_index,
                                    f32 *distance_squared, i32 horizontal_only, i32 (*filter)(GameObject_s *),
                                    bool first_match);

extern "C" i16 id_LANDSPEEDER;
extern i32 CUTCAMONLY, GUNSHIPAHACK, netcamera, complexsockposition_forcesock, movegamecamera_forcesock;
GameObject_s *FindNearestGameObject(NUVEC *, GameObject_s *, u32, f32, f32, i32, i32, i32, f32 *, i32,
                                    i32 (*)(GameObject_s *), bool);

extern MechWeakPtr<MechObjectInterface, 4> nextShootTarget;
extern MechWeakPtr<MechObjectInterface, 4> forceNextAttackOpponent;
extern MechWeakPtr<MechObjectInterface, 4> lungeTarget;

extern EXTRAACTIONDATA_s ExtraActionData[33];

float spline_xoffset_speed_roll = 16384.0f;
float spline_xoffset_speed_seek = 4.0f;
float spline_seek_ang = 10.0f;
float spline_seek_pos = 10.0f;

extern i16 id_BUZZDROID;
extern i16 id_NAFFDROID1, id_NAFFDROID2, id_NAFFDROID4, id_MOUSEDROID;
extern i16 id_SNAKE;
extern "C" i16 id_MINIATST, id_MINIATTE;
extern i16 id_PKDROID, id_PITDROID;

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

extern "C" i16 id_LANDSPEEDER;

extern "C" {
    extern i16 id_MINIATST, id_MINIATTE;
}

extern i16 id_MINIATST, id_MINIATTE;

extern i16 id_PENGUIN, id_UMBRELLA, id_GLIDEPACK;

i32 GizmoFileReadGameAnimSet(GAMEANIMSET_s *set, void *world_ptr,
                             void (*read_object_data)(GAMEANIMOBJ_s *, unsigned char), unsigned char version,
                             char *prefix, char *suffix);

extern "C" {
    void *AIPAthFindPathCnx(AISYS_s *, AIPATH_s *, char *, char *, i32 *);
}

extern i16 id_WOOKIEE;

extern speedup_s DogFightSpeedList[9];

extern "C" void NuRndrShadowDirCol(const NUVEC *direction, u32 colour, f32 near_distance, f32 far_distance);

extern "C" {
    extern "C++" FadeSystem FadeSys;
}

void UpdateRumble(RUMBLEPACKET *packet);

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

extern "C" {
    extern i16 id_SNAKE;
    extern i16 id_LANDSPEEDER;
}

extern "C" {
extern AICONDITIONDEF lego_aiconditiondefs[178];

}

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
        LEVEL_PROGRESS_s *progress = WORLD->level_progress;
        if (progress != NULL && ((((static_cast<u64>(progress->disabled_ai_object_mask[1]) << 32) |
                                    progress->disabled_ai_object_mask[0]) >>
                                   object_index) &
                                  1) != 0) {
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

