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
#include "nu2api/numath/nuvec.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/area.h"
#include "legoapi/world/world_shared.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/gizmos/fx/gizmopickups.h"
#include "legoapi/items/collect/torpedo.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/sfx.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmo/base/GizBlowupObjectInterface.h"
#include <string.h>
#include <stdio.h>
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameframework/saveload.h"
#include "legoapi/world/mission.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nufile/nufile.h"
#include "legoapi/items/fx/explosions.h"
#include "legoapi/props/objects/zipup.h"

struct AIROW_s;
struct nunativegscene_s;

struct spacelevel_s;

static void UpdateBolt_Geonosian(BOLT_s *);
static void EndBolt_EwokTorpedo(BOLT_s *);

#include "legoapi/items/collect/bolttypes_lsw.inc"

extern "C" void PlaySfx(char *, NUVEC *);

__attribute__((force_align_arg_pointer)) void Bolt_AddDeflectedBolt(BOLT_s *bolt, nuvec_s *direction, nuvec_s *normal,
                                                                    unsigned char *processed);

static __attribute__((noinline, noclone)) i32 Bolt_HitPlat(BOLT_s *bolt, u8 *hit_flags)
    __asm__("_ZL12Bolt_HitPlatP6BOLT_sPhP11WORLDINFO_s.isra.38");

static __used__ void UpdateBolt_Geonosian(BOLT_s *bolt);
static __used__ void EndBolt_EwokTorpedo(BOLT_s *bolt);
extern void (*FS_Callback)(char *, char *);

extern "C" void StartFileSel(char *title, char *path, char *filter, char *filter_out,
                             void (*callback)(char *, char *));

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

