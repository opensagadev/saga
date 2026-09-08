#include "legoapi/world/world_shared.h"

#include "decomp.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>

// Forward declarations for local (static) game-object helper stubs.
struct GameObject_s;
struct nuvec_s;
struct WORLDINFO_s;
struct BOLT_s;
struct debinftype;

extern void SetGameObjectCharacterData(GameObject_s *obj);
extern void GetTopBot(GameObject_s *obj);
extern void GameObjectDimensions(GameObject_s *obj);
extern void GameObjectOrigin(GameObject_s *obj);
extern i32 GetDefaultIdle(GameObject_s *obj);
extern void ResetCharacterIdle(GameObject_s *obj, i32 mode, i32 idle);
extern void *Suit_GetDefault(i32 id);
extern void ResetLights(NUVEC *position, rtldata_s *data, void *set);
extern "C" void ResetAnimPacket(void *packet, i32 enabled);
extern void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);

extern "C" {
    extern i16 id_MOSEISLEYCITIZEN;
    extern i16 id_CANTINAALIEN;
    extern i16 id_CLOUDCITYCITIZEN;
    extern i16 id_GEONOSIAN;
    extern i16 id_BOB;
}

i32 addcreature_override_id_check;
f32 default_mover_extra = 0.05f;

void ClearGameObjects(APIOBJECTSYS_s *api_object_sys) {
    for (i32 i = 0; i < 64; i++) {
        Obj[i].KillTasks();
        Obj[i].ClearAddons();
        Obj[i].ClearMechObjectInterface();
    }
    memset(Obj, 0, sizeof(GameObject_s) * 64);
    APIObjectDestroyAll(api_object_sys);
    HIGHGAMEOBJECT = 0;
}

GameObject_s *AddGameObject(i32 id) {
    GameObject_s *object = reinterpret_cast<GameObject_s *>(APIObjectCreate(WORLD->api_object_sys));
    if (object == NULL) {
        return NULL;
    }

    const u8 object_index = object->apiobj.field_0x289;
    object->field_0x661 = 0xff;
    object->apiobj.field_0x27f = 0xff;
    object->apiobj.field_0x280 = 0xff;
    object->apiobj.field_0x281 = 0xff;
    object->field_0x1086 = 2;
    object->apiobj.field_0x1f8 |= 0x1000 | APIOBJECT_FLAG_IN_USE;
    object->field_0x1054 = 1;
    object->apiobj.field_0x1e4 = object_index < 32 ? 1u << object_index : 0;
    object->apiobj.field_0x1e8 = object_index < 32 ? 0 : 1u << (object_index - 32);
    object->apiobj.field_0xa8 = 1.0f;
    object->field_0x1004 = 1.0f;
    object->field_0x1020 = 2000000.0f;
    object->apiobj.field_0x218 = 2000000.0f;
    object->apiobj.water_height = 2000000.0f;
    object->apiobj.field_0x220 = 2000000.0f;

    HIGHGAMEOBJECT = 0;
    for (i32 i = 0; i < 64; i++) {
        if ((Obj[i].apiobj.field_0x1f8 & APIOBJECT_FLAG_IN_USE) != 0) {
            HIGHGAMEOBJECT = i + 1;
        }
    }

    object->ai.owner = object;
    object->apiobj.field_0x2a8 = 0;
    object->apiobj.field_0x2ac = 0;
    object->apiobj.objptr = object;
    object->apiobj.ai = &object->ai;

    // The remainder of the original function attaches the Android touch
    // edge-stop/autofire addons.  The gameplay object itself is complete at
    // this point; those addon constructors are reconstructed separately.
    (void)id;
    return object;
}
void InitGameObjectLights(void) {
    for (i32 i = 0; i < 64; ++i)
        Obj[i].dynamic_light_id = -1;
    for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
        GameObject_s *object = &Obj[i];
        if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001)
            continue;
        object->dynamic_light_id = rtlDynamicAlloc();
        if (object->dynamic_light_id == -1)
            continue;
        rtlDynamicSetType(object->dynamic_light_id, 2);
        rtlDynamicEnable(object->dynamic_light_id, 0);
    }
}

// Local (static) game-object behaviour codes and per-object helpers. Stubbed
// as local `t` symbols matching res/libTTapp.so.

static __used__ void ShieldCode(GameObject_s *) {
}

static __used__ void TrenchMove(GameObject_s *) {
}

static __used__ void ZapCode(GameObject_s *, i32, i32) {
}

static __used__ void Punch_HitHold(GameObject_s *, GameObject_s *) {
}

static __used__ i32 Punch_GetDamage_LSW(GameObject_s *, GameObject_s *) {
    return 0;
}

static __used__ void Punch_HitExtraCode_LSW(GameObject_s *, nuvec_s *) {
}

static __used__ void TrenchKilledCallback(GameObject_s *) {
}

static __used__ void SurfaceInfo_ExtraReflect(GameObject_s *) {
}

static __used__ void PauseGame_ExtraCode() {
}

static __used__ void UpdateTotalPtls(debinftype *) {
}

static __used__ i32 SpecialObjectFilter(void *) {
    return 0;
}

static __used__ void KilledTrooperCannon(GameObject_s *) {
}

static __used__ void FireCode(GameObject_s *, int, int, float, int) {
}
