#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/motion.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void GetGenericGoon(i32) {
}

i32 ObjInNarrowSock(GameObject_s *, SOCKSYS *, i32) {
    return 0;
}

i32 objInNetWaitContext(GameObject_s *object, i32 context) {
    if (LEGOCONTEXT_NETWAIT != -1 && object != NULL && LEGOCONTEXT_NETWAIT == object->character_context) {
        return object->context_animation == context;
    }
    return false;
}

namespace {
    GameObject_s *AtOnce_attackingPlayer[8][17];
    f32 AtOnce_attackerDistance[8][17];
    i32 AtOnce_attackerCount[8];
    i32 AtOnce_attackersPerRow = 32;
    f32 AtOnce_InitialRowDist = 0.75f;
    f32 AtOnce_RowDist = 0.75f;
    i32 AtOnce_maxAttackers = 1;
} // namespace

bool oneAtOnce_CanAttack(GameObject_s *object, GameObject_s *opponent) {
    if (object == NULL) {
        return false;
    }
    if ((object->field_0xf01 & 0x20) == 0) {
        return true;
    }
    return object->one_at_once_player != 0xff && Player[object->one_at_once_player] == opponent;
}

f32 oneAtOnce_GetHoldRange(GameObject_s *object) {
    if (object == NULL || object->ai.opponent == NULL) {
        return 0.75f;
    }
    GameObject_s *opponent = *object->ai.action_target_ref;
    const i8 player_index = opponent != NULL ? opponent->apiobj.field_0x27c : -1;
    if (player_index < 0 || player_index >= 8) {
        return 0.75f;
    }

    GameObject_s *candidate = AtOnce_attackingPlayer[player_index][0];
    if (candidate == NULL) {
        return AtOnce_InitialRowDist;
    }
    if (candidate == object) {
        return AtOnce_InitialRowDist;
    }

    i32 row = 0;
    i32 unassigned_in_row = 0;
    for (i32 index = 0; index <= 16; ++index) {
        candidate = AtOnce_attackingPlayer[player_index][index];
        if (candidate == NULL) {
            break;
        }
        unassigned_in_row += candidate->one_at_once_player == 0xff;
        if (unassigned_in_row >= AtOnce_attackersPerRow) {
            ++row;
            unassigned_in_row = 0;
        }
        if (candidate == object) {
            return static_cast<f32>(row) * AtOnce_RowDist + AtOnce_InitialRowDist;
        }
    }
    return AtOnce_InitialRowDist;
}

void oneAtOnce_MaintainArray() {
    GameObject_s *previous_attackers[8][4] = {};
    i32 previous_count[8] = {};

    for (i32 index = 0; index < HIGHGAMEOBJECT; ++index) {
        GameObject_s *object = &Obj[index];
        const u16 character_flags = APIOBJECT_FLAG_IN_USE | APIOBJECT_FLAG_CHARACTER;
        if ((object->apiobj.field_0x1f8 & character_flags) != character_flags || object->ai.opponent == NULL ||
            (object->field_0xf01 & 0x20) == 0) {
            continue;
        }

        const u8 old_player = object->one_at_once_player;
        if (old_player < 8 && previous_count[old_player] < 4) {
            previous_attackers[old_player][previous_count[old_player]++] = object;
        }
        object->one_at_once_player = 0xff;

        APIOBJECT *opponent_api = static_cast<APIOBJECT *>(object->ai.opponent);
        GameObject_s *opponent = opponent_api != NULL ? opponent_api->objptr : NULL;
        i32 player_index = -1;
        for (i32 player = 0; player < 8; ++player) {
            if (Player[player] == opponent) {
                player_index = player;
                break;
            }
        }
        if (player_index < 0 || AtOnce_attackerCount[player_index] >= 17) {
            continue;
        }

        const i32 slot = AtOnce_attackerCount[player_index]++;
        AtOnce_attackingPlayer[player_index][slot] = object;
        AtOnce_attackerDistance[player_index][slot] =
            NuVecDistSqr(&opponent->apiobj.collision_position, &object->apiobj.collision_position, NULL);
    }

    for (i32 player = 0; player < 8; ++player) {
        const i32 count = AtOnce_attackerCount[player];
        for (i32 slot = 0; slot < count; ++slot) {
            for (i32 previous = 0; previous < previous_count[player]; ++previous) {
                if (AtOnce_attackingPlayer[player][slot] == previous_attackers[player][previous]) {
                    AtOnce_attackerDistance[player][slot] *= 0.75f;
                    break;
                }
            }
        }

        for (i32 remaining = count; remaining > 1; --remaining) {
            for (i32 slot = 1; slot < remaining; ++slot) {
                if (AtOnce_attackerDistance[player][slot] < AtOnce_attackerDistance[player][slot - 1] &&
                    AtOnce_attackerDistance[player][slot] != AtOnce_attackerDistance[player][slot - 1]) {
                    GameObject_s *object = AtOnce_attackingPlayer[player][slot - 1];
                    AtOnce_attackingPlayer[player][slot - 1] = AtOnce_attackingPlayer[player][slot];
                    AtOnce_attackingPlayer[player][slot] = object;
                    const f32 distance = AtOnce_attackerDistance[player][slot - 1];
                    AtOnce_attackerDistance[player][slot - 1] = AtOnce_attackerDistance[player][slot];
                    AtOnce_attackerDistance[player][slot] = distance;
                }
            }
        }

        if (Player[player] != NULL) {
            const i32 attack_limit = AtOnce_maxAttackers < count ? AtOnce_maxAttackers : count;
            for (i32 slot = 0; slot < attack_limit; ++slot) {
                AtOnce_attackingPlayer[player][slot]->one_at_once_player = static_cast<u8>(player);
            }
        }
        if (count < 17) {
            AtOnce_attackingPlayer[player][count] = NULL;
            AtOnce_attackerDistance[player][count] = 1.0e9f;
        }
        AtOnce_attackerCount[player] = 0;
    }
}

void oneAtOnce_SetDistPerRow(float distance) {
    AtOnce_RowDist = distance >= 0.0f ? distance : 0.0f;
}

void NarrowSockExceptions_Init(NARROWSOCKEXCEPTION *) {
}

void oneAtOnce_SetNumAttackers(i32 attackers) {
    AtOnce_maxAttackers = MAX(0, MIN(attackers, 4));
}

void MakeBaddiesForgetAboutParty(i32) {
}

void oneAtOnce_SetInitDistPerRow(float distance) {
    AtOnce_InitialRowDist = distance >= 0.0f ? distance : 0.0f;
}

void oneAtOnce_SetAttackersPerRow(i32 attackers) {
    AtOnce_attackersPerRow = MAX(0, attackers);
}
