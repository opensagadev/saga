#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/world.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutex.h"

#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern EXPLOSION Explosion[8];
extern i32 i_explosion;

u16 ObjHitObj_Flags(GameObject_s *object);

EXPLOSION *AddExplosion(nuvec_s *position, float radius, float strength, GameObject_s *object, i32 effect, i32 flags) {
    EXPLOSION *explosion = &Explosion[i_explosion];
    explosion->position = *position;
    explosion->field_0x1c = 0;
    explosion->field_0x18 = radius;
    explosion->object = object;
    explosion->field_0x20 = strength;
    explosion->field_0x00 = 0;
    explosion->field_0x04 = 0;
    explosion->field_0x2a = qrand();
    explosion->field_0x2c = effect;
    explosion->field_0x24 = flags;
    explosion->field_0x2e = ObjHitObj_Flags(object);
    explosion->field_0x32 = 1;
    explosion->field_0x33 = 0xff;
    explosion->field_0x30 = 0;
    if (++i_explosion == 8) {
        i_explosion = 0;
    }
    return explosion;
}

void SetupBlowupSfx(WORLDINFO_s *, specialsfx_s *) {
    // The original Android function has no behavior (padding followed by ret).
}

void ResetExplosions() {
    memset(Explosion, 0, sizeof(Explosion));
    i_explosion = 0;
}

extern f32 FRAMETIME;
void UpdateExplosion_Generic(EXPLOSION *);

void UpdateExplosions() {
    for (i32 i = 0; i < 8; ++i) {
        EXPLOSION *explosion = &Explosion[i];
        if (explosion->field_0x1c < explosion->field_0x20) {
            explosion->field_0x1c += FRAMETIME;
            if (explosion->field_0x1c >= explosion->field_0x20) {
                explosion->field_0x1c = explosion->field_0x20;
            } else {
                if (explosion->object != NULL && (explosion->object->apiobj.flags_low & 1) == 0)
                    explosion->object = NULL;
                UpdateExplosion_Generic(explosion);
            }
        }
    }
}

extern WORLDINFO_s *WORLD;
extern "C" i32 ParticlesPerSecond(f32, f32);
extern "C" i32 AddGameDebrisRot(APIDEBRISSYS_s *, i32, NUVEC *, i32, i16, i16);
i32 SphereSphereOverlapScaleY(NUVEC *, f32, f32, NUVEC *, f32, f32);
BOLTTYPE_s *BoltType_FindByID(i32, WORLDINFO_s *);
PART_s *Bolt_HitParts(BOLT_s *, NUVEC *, NUVEC *, NUVEC *, f32, i32);
i32 Arcade_GetMode(u32 *);
i32 CannotKill(GameObject_s *);
i32 Player_HasInvincibility(GameObject_s *);
i32 ObjHitObj(GameObject_s *, GameObject_s *, i32, u16, i32, i32);
void ObjHitShield(GameObject_s *, GameObject_s *, i32, BOLT_s *);
void NewBuzz(nupad_s *, f32, i32);
void NewRumble(nupad_s *, f32, i32);
void NewRumbleAllPlayers(f32, f32, i32, i32);
void GameCam_HitJudder();
void Arcade_AIKilled(i32);
void Arcade_PlayerKilled(i32, i32);
GIZMOBLOWUP_s *GizmoBlowUp_Hit(GameObject_s *, NUVEC *, i32, f32, NUVEC *, NUVEC *, BOLT_s *, u32, u8 *);

void UpdateExplosion_Generic(EXPLOSION *explosion) {
    const f32 progress = explosion->field_0x1c / explosion->field_0x20;
    const f32 radius = explosion->field_0x18 * progress;
    u16 angle = explosion->field_0x2a;
    if ((explosion->field_0x24 & 0x10) != 0) {
        for (i32 i = 0; i < 5; ++i) {
            const i32 count = ParticlesPerSecond(40.0f, FRAMETIME);
            NUVEC position;
            position.x = radius * NU_SIN_LUT(angle) + explosion->position.x;
            position.y = progress * 0.1f + explosion->position.y;
            position.z = radius * NU_COS_LUT(angle) + explosion->position.z;
            AddGameDebrisRot(WORLD->debris_sys, static_cast<i16>(explosion->field_0x2c), &position, count, 0, 0);
            angle += 0x3333;
        }
    }
    NUVEC minimum = {explosion->position.x - radius, explosion->position.y - radius, explosion->position.z - radius};
    NUVEC maximum = {explosion->position.x + radius, explosion->position.y + radius, explosion->position.z + radius};
    bool hit_character = false;
    if ((explosion->field_0x24 & 0x100) == 0) {
        for (i32 i = 0; i < HIGHGAMEOBJECT; ++i) {
            GameObject_s *target = &Obj[i];
            APIOBJECT_s *object = &target->apiobj;
            if ((object->field_0x1f8 & 0x1001) != 0x1001 || object->field_0x287 != 0)
                continue;
            if ((CInfo[static_cast<i8>(target->character_context)].flags & 0x20008000) != 0)
                continue;
            if ((static_cast<GAMECHARACTERDATA_s *>(object->character_data->field11_0x24)->flags_090 & 0x8000) != 0)
                continue;
            if ((explosion->field_0x24 & 0x80) != 0 && (object->character_data->model_flags & 0x10) != 0)
                continue;
            if ((explosion->field_0x24 & 0x4000) != 0 && object->field_0x27c != -1)
                continue;
            if ((explosion->field_0x24 & 8) == 0 &&
                (((object->field_0x1e4 & explosion->field_0x00) | (object->field_0x1e8 & explosion->field_0x04)) != 0 ||
                 explosion->object == target))
                continue;
            if (object->collision_min.x > maximum.x || minimum.x > object->collision_max.x ||
                object->collision_min.y > maximum.y || minimum.y > object->collision_max.y ||
                object->collision_min.z > maximum.z || minimum.z > object->collision_max.z)
                continue;
            if (!SphereSphereOverlapScaleY(&object->collision_position, object->field_0x1dc, object->field_0x1e0,
                                           &explosion->position, radius, radius))
                continue;
            if ((explosion->field_0x24 & 8) != 0) {
                f32 x = object->collision_position.x - explosion->position.x;
                f32 z = object->collision_position.z - explosion->position.z;
                if (z == 0.0f && x == 0.0f) {
                    x = static_cast<f32>(qrand()) * (1.0f / 65535.0f) - 0.5f;
                    z = static_cast<f32>(qrand()) * (1.0f / 65535.0f) - 0.5f;
                }
                const f32 inverse = 1.0f / NuFsqrt(x * x + z * z);
                object->field_0x1fc = (x * inverse) + (x * inverse);
                object->field_0x204 = (z * inverse) + (z * inverse);
            }
            if (hit_character || (explosion->field_0x24 & 1) == 0)
                continue;
            if (((object->field_0x1e4 & explosion->field_0x00) | (object->field_0x1e8 & explosion->field_0x04)) != 0 ||
                explosion->object == target || target->spawn_protection_timer > 0.0f ||
                (target->field_0xefe & 0x40) != 0)
                continue;
            if (object->field_0x27c != -1 && target->field_0x1024 > 0.0f && (explosion->field_0x24 & 0x2000) == 0)
                continue;
            GameObject_s *source = explosion->object;
            if (source != NULL && !(Arcade_GetMode(NULL) == 99 && (explosion->field_0x24 & 0x10010) != 0)) {
                const bool target_player = object->field_0x27c != -1;
                const bool source_player = source->apiobj.field_0x27c != -1;
                if (target_player == source_player && (!target_player || target->field_0xd24 != 1.0f) &&
                    ((object->field_0x1f4 ^ source->apiobj.field_0x1f4) & 1) == 0)
                    continue;
            }
            explosion->field_0x00 |= object->field_0x1e4;
            explosion->field_0x04 |= object->field_0x1e8;
            if (target->field_0xd24 >= 0.99f) {
                ObjHitShield(explosion->object, target, target->field_0xe37, NULL);
                if ((explosion->field_0x24 & 0x20) == 0) {
                    hit_character = true;
                    continue;
                }
            }
            if (CannotKill(target)) {
                hit_character = true;
                continue;
            }
            i32 damage;
            if ((object->flags_low & 0x80) != 0 && Player_HasInvincibility(target))
                damage = 0;
            else if ((explosion->field_0x24 & 0x800) != 0 && (target->field_0xefb & 8) == 0)
                damage = -1;
            else if ((explosion->field_0x24 & 0x20) != 0 && (target->field_0xefb & 8) == 0 &&
                     static_cast<i8>(target->current_hp) > 0)
                damage = -1;
            else
                damage = explosion->field_0x32;
            source = explosion->object;
            if (source == NULL) {
                if (explosion->field_0x33 == 0)
                    source = Player[0];
                else if (explosion->field_0x33 == 1)
                    source = Player[1];
            }
            const u16 hit_flags = explosion->field_0x2e | ((explosion->field_0x24 & 0x200) != 0 ? 0x4200 : 0x200);
            if (ObjHitObj(source, target, damage, hit_flags, 0, 1) == 2) {
                if (source != NULL)
                    NewRumble(source->pad_gamepad->pad, 0.5f, 0);
                ++explosion->field_0x30;
                if ((explosion->field_0x24 & 0x200) != 0 && explosion->field_0x33 <= 1 && Arcade != 0) {
                    if (static_cast<u8>(object->field_0x27c) <= 1)
                        Arcade_PlayerKilled((object->field_0x27c ^ 1) & 1, 0);
                    else if (object->field_0x27c == -1)
                        Arcade_AIKilled(static_cast<i8>(explosion->field_0x33));
                }
            }
            if ((explosion->field_0x24 & 0x40) != 0) {
                NewRumbleAllPlayers(0.5f, 0.1f, 0, 0);
                GameCam_HitJudder();
            } else if (explosion->object != NULL)
                NewBuzz(explosion->object->pad_gamepad->pad, 0.1f, 0);
            hit_character = true;
        }
    }
    if (!hit_character && (explosion->field_0x24 & 2) != 0) {
        u32 hit_type;
        if ((explosion->field_0x24 & 0x10) != 0)
            hit_type = 3;
        else if ((explosion->field_0x24 & 0x100) != 0)
            hit_type = 7;
        else if ((explosion->field_0x24 & 0x200) != 0)
            hit_type = 8;
        else
            hit_type = (explosion->field_0x24 & 0x8000) != 0 ? 9 : 2;
        if (GizmoBlowUp_Hit(explosion->object, &explosion->position, 1, radius, &minimum, &maximum, NULL, hit_type,
                            NULL)) {
            if ((explosion->field_0x24 & 0x40) != 0) {
                NewRumbleAllPlayers(0.5f, 0.1f, 0, 0);
                GameCam_HitJudder();
            } else if (explosion->object != NULL)
                NewBuzz(explosion->object->pad_gamepad->pad, 0.1f, 0);
            return;
        }
    }
    if ((explosion->field_0x24 & 0x100) == 0 && (explosion->field_0x24 & 4) != 0) {
        BOLT_s bolt;
        NUVEC points[3];
        bolt.owner = explosion->object;
        points[1] = explosion->position;
        bolt.position = explosion->position;
        bolt.active = 0;
        bolt.flags = 0x200;
        bolt.type_id = 1;
        bolt.type = BoltType_FindByID(1, WORLD);
        if (Bolt_HitParts(&bolt, points, &minimum, &maximum, radius, 0) != NULL) {
            if ((explosion->field_0x24 & 0x40) != 0) {
                NewRumbleAllPlayers(0.5f, 0.1f, 0, 0);
                GameCam_HitJudder();
            } else if (explosion->object != NULL)
                NewBuzz(explosion->object->pad_gamepad->pad, 0.1f, 0);
        }
    }
}
