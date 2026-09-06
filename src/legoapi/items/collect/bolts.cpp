#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "decomp.h"
#include "nu2api/numusic/sfx.h"
#include <string.h>

extern BOLT_s Bolt[32];
extern i32 i_bolt;
extern f32 BOLT_OVERRIDE_PLAYERBOLTSPEED;
extern f32 BOLT_OVERRIDE_PLAYERBOLTDURATION;
struct spacelevel_s;
struct quickboltinfo;

static void Bolt_Debris_Default(BOLT_s *, NUVEC *, i32, NUVEC *, i32);
static void Bolt_GetShootOrigin_Default(GameObject_s *, NUVEC *);
static void Bolt_GetShootDirection_Default(GameObject_s *, NUVEC *);
BOLTTYPE_s GlobalBoltType_Default = {"null", 4.0f, 2.0f, 0.0f, 0.125f, 1.0f, 0.1f,
    -1, -1, -1, 0, -1, 0, 1, 255, 0, {}, NULL, NULL, 0, -1, -1, {}};
static BOLTSYS BoltSys_Default = {&GlobalBoltType_Default, 1, NULL, Bolt_Debris_Default,
    Bolt_GetShootOrigin_Default, Bolt_GetShootDirection_Default, NULL, NULL};
BOLTSYS *BoltSys = &BoltSys_Default;

void Bolt_Alloc() {
}

void Bolt_Shoot(GameObject_s *, i32, i32) {
}

void Bolts_Draw(WORLDINFO_s *) {
}

void Bolts_Reset() {
    memset(Bolt, 0, sizeof(Bolt));
    i_bolt = 0;
    BOLT_OVERRIDE_PLAYERBOLTSPEED = 0.0f;
    BOLT_OVERRIDE_PLAYERBOLTDURATION = 0.0f;
}

void BoltSys_Init(BOLTSYS *system) {
    for (i32 i = 0; i < system->count; ++i) {
        BOLTTYPE_s *type = &system->types[i];
        type->hit_sfx_id = -1;
        if (type->hit_sfx != NULL) type->hit_sfx_id = GetSfxId(type->hit_sfx);
        type->shoot_sfx_id = -1;
        if (type->shoot_sfx != NULL) type->shoot_sfx_id = GetSfxId(type->shoot_sfx);
    }
    BoltSys = system;
    if (system->debris == NULL) system->debris = Bolt_Debris_Default;
    if (system->shoot_origin == NULL) system->shoot_origin = Bolt_GetShootOrigin_Default;
    if (system->shoot_direction == NULL) system->shoot_direction = Bolt_GetShootDirection_Default;
}

void Bolt_Reflect(nuvec_s *, nuvec_s *, nuvec_s *) {
}

void Bolts_Update(WORLDINFO_s *) {
}

void Bolt_HitParts(BOLT_s *, nuvec_s *, nuvec_s *, nuvec_s *, float, i32) {
}

void BoltTypes_Reset(WORLDINFO_s *world) {
    memset(world->bolt_types, 0, sizeof(world->bolt_types));
}

void Bolt_Debris_LSW(BOLT_s *, nuvec_s *, i32, nuvec_s *, i32) {
}

void Bolt_HitPartMode(BOLT_s *) {
}

void Bolt_HitPart_LSW(BOLT_s *, PART_s *) {
}

BOLTTYPE_s *BoltType_FindByID(i32 id, WORLDINFO_s *world) {
    if (id >= 0 && id < BoltSys->count) {
        return &BoltSys->types[id];
    }
    if (world != NULL && id >= BoltSys->count && id <= BoltSys->count + 7) {
        return &world->bolt_types[id - BoltSys->count];
    }
    return NULL;
}

void Bolt_HitGameObject(BOLT_s *, GameObject_s *, nuvec_s *, nuvec_s *, nuvec_s *, float, unsigned char *) {
}

void Bolt_HitPlatFn_LSW(BOLT_s *) {
}

void Bolt_HitGameObjects(BOLT_s *, nuvec_s *, nuvec_s *, nuvec_s *, float, unsigned char *) {
}

void Bolt_HitCustomFn_LSW(BOLT_s *, nuvec_s *) {
}

void Bolt_HitGameObjectRC(NetMessage &) {
}

void Bolt_AddDeflectedBolt(BOLT_s *, nuvec_s *, nuvec_s *, unsigned char *) {
}

void Bolt_AlternateFire_LSW(GameObject_s *, i32) {
}

void Bolt_ObjTargetPosYAdjust(GameObject_s *) {
}

void BoltType_FindIDByCreature(GameObject_s *, i32) {
}

void Bolt_Add(GameObject_s *, nuvec_s *, numtx_s *, i32, i32) {
}

void Bolt_End(BOLT_s *, i32) {
}

void Bolt_Find(i32, nuvec_s *, GameObject_s *) {
}

void Bolt_Free(BOLT_s *) {
}

void Bolt_Init(void *, NetMessage &) {
}

static __used__ void UpdateBolt_Geonosian(BOLT_s *) {
}

static __used__ bool Bolt_RayCast(BOLT_s *, nuvec_s *, nuvec_s *, float) {
    return {};
}

static __used__ void Bolt_Debris_Default(BOLT_s *, nuvec_s *, int, nuvec_s *, int) {
}

static __used__ void Bolt_GetShootOrigin_Default(GameObject_s *, nuvec_s *) {
}

static __used__ void Bolt_GetShootDirection_Default(GameObject_s *, nuvec_s *) {
}

static __used__ unsigned int Batarang_GetTargetPos(BATARANG_s *, int, nuvec_s *) {
    return {};
}

static __used__ void CollideBoltStarFighter(BOLT_s *, starfighter_s *, _vuv_s *, _vuv_s *) {
}

static __used__ void EndBolt_EwokTorpedo(BOLT_s *) {
}

static __used__ void ProcessSpaceLevel(spacelevel_s *) {
}

static __used__ void ProcessStarFighter(starfighter_s *, quickboltinfo *) {
}

static __used__ void StarFighterAlign(starfighter_s *, _vuv_s *, f32, i32) {
}

static __used__ void TrooperTeamSetStateCode(minitrooperteam_s *) {
}

static __used__ unsigned int BoltInitSfx_LSW(GameObject_s *) {
    return {};
}

void BoltTypes_Init(WORLDINFO_s *world) {
    (void)world;
}

void BoltTypes_Configure(WORLDINFO_s *world, char *config) {
    (void)world;
    (void)config;
}

extern "C" {

    void HitParts(void) {
    }

} // extern "C"
