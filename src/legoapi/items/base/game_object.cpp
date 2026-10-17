#include "legoapi/world/world_shared.h"

#include "decomp.h"
#include "globals.h"
#include "legoapi/actions/character/suit.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/qrand.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/gizmos/object/gizbuildits.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nuvec.h"

#include <string.h>
#include <new>

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
extern void ResetCharacterIdle(GameObject_s *obj, i32 mode, i32 idle);
extern void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *data);
extern void Hub_ResetPanel();
extern f32 VehicleTurnOrLoopOffset(GameObject_s *object);
extern void StartTurn(GameObject_s *object);

extern "C" {
    extern i16 id_MOSEISLEYCITIZEN;
    extern i16 id_CANTINAALIEN;
    extern i16 id_CLOUDCITYCITIZEN;
    extern i16 id_GEONOSIAN;
    extern i16 id_BOB;
    extern i16 id_SPEEDERBIKE;
    extern i16 id_SPEEDERBIKESNOW;
    extern i16 id_STAP;
    extern i16 id_STAP2;
    extern i16 id_TROOPERCANNON;
    extern i16 id_CANNON;
    extern i16 id_MOSCANNON;
    extern i16 id_ATST;
    extern i16 id_BASKETCANNON;
    extern i16 id_BIGGUN;
    extern i16 id_IMPERIALGUARD;
    extern i16 id_GAMORREANGUARD;
    extern i16 id_ZAMSSPEEDER;
}

i32 addcreature_override_id_check;
f32 default_mover_extra = 0.05f;
extern f32 trench_max_height_move;
extern f32 trench_roll_f;
extern f32 trench_seek_z;
extern f32 trench_seek_y;
extern f32 trench_seek_x;
extern f32 TURNTIME;
extern f32 LOOPTIME;

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
    object->field_0x1086 = 2;
    object->apiobj.flags_low |= APIOBJECT_FLAG_IN_USE;
    object->apiobj.character = 1;
    object->field_0x1054 = 1;
    object->apiobj.collision_identity_mask = u64(1) << object_index;
    object->apiobj.field_0xa8 = 1.0f;
    object->field_0x1004 = 1.0f;
    object->field_0x1020 = 2000000.0f;
    object->apiobj.field_0x281 = 0xff;
    object->apiobj.field_0x218 = object->apiobj.water_height = object->apiobj.field_0x220 = 2000000.0f;

    HIGHGAMEOBJECT = 0;
    for (i32 i = 0; i < 64; i++) {
        if ((Obj[i].apiobj.flags_low & APIOBJECT_FLAG_IN_USE) != 0) {
            HIGHGAMEOBJECT = i + 1;
        }
    }

    object->ai.owner = object;
    object->apiobj.field_0x2a8 = 0;
    object->apiobj.field_0x2ac = 0;
    object->apiobj.objptr = object;
    object->apiobj.ai = &object->ai;

    MechAddonCollection *addons = object->GetAddons(true);
    if (addons != NULL) {
        MechEdgeStopAddon *edge_stop = new MechEdgeStopAddon(*object->GetMechObjectInterface());
        addons->Add(*edge_stop);
        if (VehicleArea != 0 || id == id_SPEEDERBIKE || id == id_SPEEDERBIKESNOW || id == id_STAP || id == id_STAP2 ||
            id == id_TROOPERCANNON || id == id_CANNON || id == id_MOSCANNON || id == id_ATST || id == id_BASKETCANNON ||
            id == id_BIGGUN) {
            MechObjectInterface *target = object->GetMechObjectInterface();
            MechAutofireAddon *addon = NU_ALLOC_T(MechAutofireAddon, 1, "", 0);
            if (addon != NULL)
                new (addon) MechAutofireAddon(*target);
            addons->Add(*addon);
        }
    }
    return object;
}
void TrenchMove(GameObject_s *) asm("_ZL10TrenchMoveP12GameObject_s") __attribute__((visibility("hidden")));
void TrenchKilledCallback(GameObject_s *) asm("_ZL20TrenchKilledCallbackP12GameObject_s")
    __attribute__((visibility("hidden")));

__used__ void TrenchMove(GameObject_s *object);

__used__ void TrenchKilledCallback(GameObject_s *object);

static __used__ void SurfaceInfo_ExtraReflect(GameObject_s *object);

static __used__ void PauseGame_ExtraCode();

static __used__ i32 SpecialObjectFilter(void *object) {
    return theSceneObjectHelper.scene_id == static_cast<SpecialObject *>(object)->scene_id;
}
