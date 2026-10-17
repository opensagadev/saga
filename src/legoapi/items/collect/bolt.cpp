// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "gamelib/util/gamelib_util_types.h"
#include "legoapi/characters/motion.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

i32 LineIntersectSphere(NUVEC *, NUVEC *, NUVEC *, f32, f32 *);
bool LineIntersectCircle(NUVEC *, NUVEC *, NUVEC *, f32);

BOLT_s *FindIncomingBolt(GameObject_s *object, i32 exclude_players, i32 mark_direct_hit) {
    BOLT_s *nearest = NULL;
    f32 radius = object->apiobj.field_0x1e0 > object->apiobj.field_0x1dc ? object->apiobj.field_0x1e0
                                                                         : object->apiobj.field_0x1dc;
    const f32 radius_squared = radius * radius;
    f32 nearest_distance = 1.0e8f;
    BOLT_s *bolts = Bolt;
    for (BOLT_s *bolt = bolts; bolt != bolts + 32; ++bolt) {
        if (bolt->active == 0)
            continue;
        if (exclude_players != 0 && bolt->owner != NULL && (bolt->owner->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            bolt->owner->apiobj.field_0x287 == 0 && bolt->owner->apiobj.field_0x27c != -1)
            continue;
        f32 time = TouchHacks::TouchControlsActive ? 1.5f : 0.5f;
        f32 travel_squared = time * bolt->speed;
        travel_squared *= travel_squared;
        f32 distance = NuVecDistSqr(&bolt->position, &object->apiobj.collision_position, NULL);
        if (distance < travel_squared &&
            LineIntersectSphere(&bolt->position, &bolt->field_0xac, &object->apiobj.collision_position, radius_squared,
                                NULL) &&
            distance < nearest_distance) {
            nearest = bolt;
            nearest_distance = distance;
        }
    }
    if (nearest != NULL && mark_direct_hit != 0) {
        radius = object->apiobj.field_0x1dc * 0.75f;
        if (LineIntersectCircle(&nearest->position, &nearest->field_0xac, &object->apiobj.collision_position,
                                radius * radius))
            object->field_0xe21 |= 0x20;
    }
    return nearest;
}

void GuidedMissile_Kill(PART_s *part, i32 reason);

void GuidedMissile_Move(PART_s *part, float time);

void GuidedMissile_Deflect(PART_s *part);

extern "C" i16 id_SPEEDERBIKE;

// ==== src/legoapi/items/collect/bolts.cpp: declarations ====
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/motion.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/light/surfaces.h"
#include "legoapi/render/fx/parts.h"
#include "globals.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/collect/bolts.h"
#include "legoapi/items/collect/torpedo.h"
#include "decomp.h"
#include "legoapi/characters/motion/chris.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuspecial.h"
#include "gameapi/edtools/edfile.h"
#include "gamelib/util/gamelib_util_types.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/numtx_inline.h"
#include "nu2api/numath/nuvec.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include <string.h>

extern BOLT_s Bolt[32];
extern i32 i_bolt;
extern f32 BOLT_OVERRIDE_PLAYERBOLTSPEED;
extern f32 BOLT_OVERRIDE_PLAYERBOLTDURATION;
struct spacelevel_s;
struct quickboltinfo;

static void UpdateBolt_Geonosian(BOLT_s *);
static void EndBolt_EwokTorpedo(BOLT_s *);
void BoltTypes_Reset(WORLDINFO_s *);

#include "legoapi/items/collect/bolttypes_lsw.inc"

extern BOLTSYS * BoltSys;


// Configuration entry point and parser callbacks share the original BT state.



void BoltTypes_Configure(WORLDINFO_s *world, char *config);


f32 Bolt_ObjTargetPosYAdjust(GameObject_s *);


void Bolt_End(BOLT_s *, i32);
extern i32 Paused;





extern "C" {
    void KillPart(PART_s *, i32);
}

void NewRumble(nupad_s *, f32, i32);


void BoltTypes_Reset(WORLDINFO_s *world);

extern "C" void PlaySfx(char *, NUVEC *);
i32 Player_HasDoubleBoltDamage_FromBolt(BOLT_s *);
void GameCam_NewShake(GAMECAMERA_s *, f32, f32, f32);
void GameCam_HitJudder();
void NewRumbleAllPlayers(f32, f32, i32, i32);
EXPLOSION *AddExplosion(NUVEC *, f32, f32, GameObject_s *, i32, i32);

void Bolt_Debris_LSW(BOLT_s *bolt, nuvec_s *points, i32 point, nuvec_s *, i32 no_explosion);


i32 Bolt_HitPart_LSW(BOLT_s *, PART_s *part);

BOLTTYPE_s *BoltType_FindByID(i32 id, WORLDINFO_s *world);

void Bolt_HitGameObjectRC(NetMessage &);



i32 Bolt_HitGameObjects(BOLT_s *bolt, NUVEC *points, NUVEC *minimum, NUVEC *maximum, f32 radius, u8 *hit_flags);


extern i32 addbolt_nosfx;
void NewBuzz(nupad_s *, f32, i32);

__attribute__((force_align_arg_pointer)) void Bolt_AddDeflectedBolt(BOLT_s *bolt, nuvec_s *direction, nuvec_s *normal,
                                                                    unsigned char *processed);
void Bolt_HitGameObjectRC(NetMessage &message);

// Retail only has the IPA-SRA clone of Bolt_HitPlat(BOLT_s *, u8 *, WORLDINFO_s *), which drops the
// unused world argument.
static __attribute__((noinline, noclone)) i32 Bolt_HitPlat(BOLT_s *bolt, u8 *hit_flags)
    __asm__("_ZL12Bolt_HitPlatP6BOLT_sPhP11WORLDINFO_s.isra.38");


i32 Bolt_AlternateFire_LSW(GameObject_s *object, i32 index);

f32 Bolt_ObjTargetPosYAdjust(GameObject_s *object);

extern AREADATA *DOGFIGHT_ADATA;
i32 BoltType_FindIDByCreature(GameObject_s *object, i32 fallback);

void Bolt_Init(void *, NetMessage &);
BOLT_s *Bolt_Add(GameObject_s *object, nuvec_s *position, numtx_s *matrix, i32 type_id, i32 flags);

void Bolt_End(BOLT_s *bolt, i32 run_callback);

BOLT_s *Bolt_Find(i32 type_id, NUVEC *position, GameObject_s *owner);


// The original caller appears to treat this u16-returning function as i32;
// correcting this declaration lowers its match.
extern i32 (*BoltInitSfxFn)(GameObject_s *);
void Bolt_Init(void *storage, NetMessage &message);

static __used__ void UpdateBolt_Geonosian(BOLT_s *bolt);

i32 GameRayCast(NUVEC *, NUVEC *, f32, i32);




EXPLOSION *Detonate(NUVEC *, u16);
static __used__ void EndBolt_EwokTorpedo(BOLT_s *bolt);

void BoltTypes_Init(WORLDINFO_s *world);

extern "C" {
    void NewTerrHitInfo(u8 *);
    void NewRayCastGetImpactNormal(NUVEC *);
    i32 NewShadowOnPlatform();
}
f32 GameShadow(GameObject_s *, NUVEC *, f32, i32);



i8 BoltType_FindIDByName(char *name, WORLDINFO *world);

// ==== src/legoapi/core/config/saveload.cpp: declarations ====
#include "decomp.h"
#include <stdio.h>
#include <string.h>

#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "globals.h"
#include "legoapi/core/config/fileselect.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nu3d/nuspecial.h"

extern i32 memcard_deleteneeded, memcard_deletestarted, memcard_deletefailed;
extern i32 memcard_formatme, memcard_formatting, memcard_formatfailed;
extern i16 (*memcard_hashfn)(void);

extern char FS_LastFileName[64];
extern char FS_Path[256];
extern char FS_Title[64];
extern char FS_Filter[256];
extern char FS_FilterOut[256];
extern f32 FS_X;
extern f32 FS_Y;
extern f32 FS_Width;
extern f32 FS_W;
extern f32 FS_H;
extern u8 FS_Active;
extern u8 FS_RefreshDir;
extern u8 FS_ShowVolumes;
extern void (*FS_Callback)(char *, char *);

void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_SortStrings(char *, char *, i32);
i32 FS_FileNameFilter(char *);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);


void InitMemCard();







void FS_SortStrings(char *start, char *end, i32 mode);








void SerialiseString(EdStream &stream, void *data, i32 count);


i32 FS_FileNameFilter(char *name);





// Retail keeps the whole body of RenderFileSel3 as the split-out .part clone;
// the exported function only tests FS_Active.
static __attribute__((noinline, noclone)) void RenderFileSel3Body(i32 draw_title)
    __asm__("_Z14RenderFileSel3i.part.38");

static void RenderFileSel3Body(i32 draw_title);

void RenderFileSel3(i32 draw_title);

void RenderFileSel();








void FS_BuildFilterBlocks(char *input);

void FS_MakeDateTimeString(FS_FILEENTRYHDR *entry, char *output);

void FS_BuildFilterOutBlocks(char *input);


i32 ProcessFileSel3(f32 elapsed, nupad_s *pad);

extern "C" i32 ProcessFileSel2(f32 elapsed, nupad_s *pad);

extern "C" void FileSelKill(void);

void ProcessFileSel(f32 elapsed, nupad_s *pad);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

extern "C" void RenderFileSel2(i32 x, i32 y, i32 *width, i32 *height);

i32 LoadState(i32, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *, variptr_u *);

extern "C" {
extern void (*savesuccessfn)();
extern void * memcard_headerdata;
extern i32 memcard_headerdatasize;
extern void * memcard_headerdatabuffer;

void FS_SetFileSelPathFromName(char *name);

void SaveSystemInitialiseEx(i32 slots, void *makeSaveHash, void *save, i32 saveSize, void *header, i32 headerSize,
                                i32 autosave, void (*drawSaveIcon)(void));

void SetSaveSuccessFn(void (*callback)(void));

void UpdateSaveSlots(void);

void loadsaveCallEachFrame(void);

i32 TriggerAutoSave(void);

} // extern "C"

// ==== functions in original address order ====

static __used__ void EndBolt_EwokTorpedo(BOLT_s *bolt) {
    Detonate(&bolt->position, 0);
}

static __used__ void UpdateBolt_Geonosian(BOLT_s *bolt) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    i32 effect = world->debris_sys->entries[85].effect;
    if (effect != -1)
        AddVariableShotDebrisEffectTimed1(effect, &bolt->position, 60, FRAMETIME, 0, 0, NULL);
}

i32 BoltType_FindIDByCreature(GameObject_s *object, i32 fallback) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (object == NULL)
        return fallback;
    i32 id = object->id;
    if (id == id_MILLENNIUMFALCON) {
        if (world->area != VEHICLES_ADATA && object->bolt_fire_phase != 0)
            return 9;
        return 7 + (world->current_level == ASTEROIDCHASED_LDATA);
    }
    if (id == id_NABOOSTARFIGHTER || id == id_NABOOSTARFIGHTERLIME)
        return 34;
    if (id == id_ANAKINSSPEEDER || id == id_ANAKINSSPEEDER_GREEN || id == id_ZAMSSPEEDER)
        return 11;
    if (id == id_FLASHSPEEDER)
        return 20;
    if (id == id_XWING || id == id_SLAVE1)
        return 6;
    if (id == id_SPEEDERBIKE || id == id_SPEEDERBIKESNOW)
        return 10;
    if (id == id_PROBEDROID || id == id_SNOWSPEEDER)
        return 11;
    if (id == id_TIEFIGHTER || id == id_TIEINTERCEPTOR || id == id_TIEFIGHTERDARTH || id == id_TIEBOMBER ||
        id == id_IMPERIALSHUTTLE) {
        if (world == NULL)
            return 12;
        if (world->current_level == DEATHSTARBATTLED_LDATA)
            return 13;
        return 12 + 2 * (world->current_level == DOGFIGHTA_LDATA);
    }
    if (id == id_MINITIEFIGHTER || id == id_MINITIEINTERCEPTOR || id == id_MINITIEADVANCED || id == id_MINITIEBOMBER ||
        id == id_MINIIMPERIALSHUTTLE || id == id_MINISITHINFILTRATOR || id == id_MINISOLARSAILOR || id == id_MINIATAT ||
        id == id_MINISTARDESTROYER)
        return 23;
    if (id == id_MINIMILLENNIUMFALCON)
        return object->bolt_fire_phase == 0 ? 21 : 22;
    if (id == id_JEDISTARFIGHTERYELLOWEP3 || id == id_JEDISTARFIGHTERREDEP3) {
        if (WORLD->area != NULL && WORLD->area == DOGFIGHT_ADATA)
            return WORLD->lev_objs[0x12d].active == 0 ? 33 : 32;
        return 33;
    }
    if (id == id_CATAPULT)
        return 29;
    if (object->apiobj.character_data->move_fn == Move_CANNON)
        return 20;
    if (GCDataList[id].uses_weapon_action == 8 || GCDataList[id].weapon_model == 22)
        return 2;
    if (id == id_BIGGUN)
        return 20;
    if (id == id_BOBAFETT || id == id_4LOM)
        return 26;
    if (id == id_TRAININGREMOTE)
        return 3;
    if (id == id_ATAT)
        return 24;
    if (id == id_ATST || id == id_CLONEWALKER)
        return 4;
    if (id == id_ATST_LOWRES)
        return 25;
    if (id == id_DROIDEKA || id == id_MINIDROIDEKA)
        return 17;
    if (id == id_STAP || id == id_STAP2)
        return 0;
    if (id == id_EWOK)
        return Cheat_IsOn(0x29) != 0 ? 30 : 27;
    if (id == id_WICKET)
        return Cheat_IsOn(0x29) != 0 ? 31 : 28;
    if (id == id_REPUBLICGUNSHIP || id == id_REPUBLICGUNSHIP_GREEN) {
        if (WORLD->current_level == BONUS_GUNSHIPA_LDATA)
            return 35;
        if (WORLD->current_level == BONUS_GUNSHIPB_LDATA)
            return 36;
        return fallback;
    }
    if (id == id_NEW_REPUBLIC_GUNSHIP || id == id_NEW_REPUBLIC_GUNSHIP_GREEN) {
        if (WORLD->current_level == GUNSHIPA_LDATA)
            return 37;
        if (WORLD->current_level == GUNSHIPB_LDATA)
            return 38;
        return fallback;
    }
    if (id == id_SENTRYDROID || id == id_KAMINOANDROID)
        return 0;
    return fallback;
}

void Bolt_Debris_LSW(BOLT_s *bolt, nuvec_s *points, i32 point, nuvec_s *, i32 no_explosion) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    i32 effect;
    if (world->current_level == DOGFIGHTA_LDATA) {
        if (bolt->field_0x104 != -1)
            points = &bolt->dogfight_hit_position;
        effect = 0x2e;
    } else {
        effect = bolt->hit_debris;
    }
    if (((bolt->flags & 0x40000200) == 0 && point == -1) || point == 0) {
        AddGameDebris(world->debris_sys, effect, &points[0]);
        if (!(bolt->flags & 0x10000000) && Player_HasDoubleBoltDamage_FromBolt(bolt))
            AddGameDebris(world->debris_sys, effect, &points[0]);
    }
    if (point == 1 || point == -1) {
        AddGameDebris(world->debris_sys, effect, &points[1]);
        if (!(bolt->flags & 0x10000000) && Player_HasDoubleBoltDamage_FromBolt(bolt))
            AddGameDebris(world->debris_sys, effect, &points[1]);
    }
    if (((bolt->flags & 0x40000200) == 0 && point == -1) || point == 2) {
        AddGameDebris(world->debris_sys, effect, &points[2]);
        if (!(bolt->flags & 0x10000000) && Player_HasDoubleBoltDamage_FromBolt(bolt))
            AddGameDebris(world->debris_sys, effect, &points[2]);
    }
    if (bolt->hit_part_debris != -1)
        AddPartDebris(world->part_debris_sys, bolt->hit_part_debris, &bolt->position);
    if (bolt->type_id == 0x1d) {
        AddGameDebris(world->debris_sys, 0x7f, &bolt->position);
        AddPartDebris(world->part_debris_sys, 6, &bolt->position);
        GameCam_NewShake(GameCam, 0.75f, 0.75f, 1.0f);
        NewRumbleAllPlayers(0.5f, 0.0f, 0, 0);
    }
    if (no_explosion == 0 && bolt->owner != NULL && bolt->owner->apiobj.player_controlled && Cheat_IsOn(0x12) &&
        (world->area == NULL || !(world->area->flags & 1))) {
        AddGameDebris(WORLD->debris_sys, 0x49, &bolt->position);
        NewRumble(bolt->owner->pad_gamepad->pad, 0.3f, 0);
        PlaySfx("exp_thermalDet", points);
        GameCam_HitJudder();
        EXPLOSION *explosion = AddExplosion(&bolt->position, 0.5f * AreaPickupScale, 0.25f, NULL, -1, 0xa067);
        if (explosion != NULL && Arcade != 0 && bolt->owner != NULL &&
            (bolt->owner == Player[0] || bolt->owner == Player[1]) &&
            (bolt->owner->apiobj.field_0x1f8 & 0x1001) == 0x1001 &&
            static_cast<u8>(bolt->owner->apiobj.field_0x27c) <= 1) {
            explosion->object = bolt->owner;
            explosion->field_0x24 |= 0x10000;
        }
    }
}

i32 Bolt_HitPart_LSW(BOLT_s *, PART_s *part) {
    switch (part->force_player_mask) {
        case 1:
            if (part->scale_time < 2.0f)
                return 1;
            break;
        case 2:
            if (part->scale_time < 1.0f)
                return 1;
            break;
    }
    return 0;
}

i32 Bolt_AlternateFire_LSW(GameObject_s *object, i32 index) {
    i16 id = object->id;
    if (id == id_XWING || id == id_MINIXWING || id == id_MINITIEINTERCEPTOR || id == id_MINIATAT ||
        id == id_MINIROYALSTARSHIP || id == id_MINIIMPERIALSHUTTLE) {
        if (object->bolt_fire_phase == 0) {
            if (index == 0 || index == 2)
                return 0;
        } else if (object->bolt_fire_phase == 1) {
            if (index == 1 || index == 3)
                return 0;
        }
    } else if (id == id_MILLENNIUMFALCON || id == id_MINIMILLENNIUMFALCON || id == id_ATST ||
               id == id_JEDISTARFIGHTERREDEP3 || id == id_JEDISTARFIGHTERYELLOWEP3) {
        if (object->bolt_fire_phase != index)
            return 0;
    }
    return 1;
}

void InitMemCard() {
}

