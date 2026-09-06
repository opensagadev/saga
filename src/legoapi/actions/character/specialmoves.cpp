#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/apiobject.h"

struct SPECIALMOVE_s {
    i16 attacker_animation;
    i16 attacker_id;
    i8 attacker_action_type;
    i8 victim_action_type;
    i16 victim_id;
    i16 victim_animation;
    u16 flags;
    f32 distance;
};
DECOMP_ASSERT(sizeof(SPECIALMOVE_s) == 0x10, "SPECIALMOVE size");
DECOMP_ASSERT(offsetof(SPECIALMOVE_s, victim_animation) == 8, "SPECIALMOVE victim animation offset");
DECOMP_ASSERT(offsetof(SPECIALMOVE_s, flags) == 10, "SPECIALMOVE flags offset");
SPECIALMOVE_s *SpecialMove;
i32 SpecialMoveCount;
i32 LEGOCONTEXT_SPECIALMOVE_ATTACKER = -1;
i32 LEGOCONTEXT_SPECIALMOVE_VICTIM = -1;

i32 SpecialMove_Check(GameObject_s *attacker, GameObject_s *victim) {
    if (SpecialMove != NULL &&
        ((attacker->apiobj.flags_low & 0x80) == 0 || attacker->field_0xda8 <= 0.0f) &&
        LEGOCONTEXT_SPECIALMOVE_ATTACKER != -1 && LEGOCONTEXT_SPECIALMOVE_VICTIM != -1) {
        for (i32 i = 0; i < SpecialMoveCount; ++i) {
            SPECIALMOVE_s *move = &SpecialMove[i];
            GAMECHARACTERDATA *a = static_cast<GAMECHARACTERDATA *>(attacker->apiobj.character_data->field11_0x24);
            GAMECHARACTERDATA *v = static_cast<GAMECHARACTERDATA *>(victim->apiobj.character_data->field11_0x24);
            if ((move->attacker_id == attacker->id ||
                 (move->attacker_action_type != -1 && move->attacker_action_type == a->field275_0x116)) &&
                attacker->apiobj.character_model->model_data_b[move->attacker_animation] != NULL &&
                (move->victim_id == victim->id ||
                 (move->victim_action_type != -1 && move->victim_action_type == v->field275_0x116)) &&
                victim->apiobj.character_model->model_data_b[move->victim_animation] != NULL &&
                ((victim->apiobj.flags_low & 0x80) == 0 ||
                 (victim->spawn_protection_timer <= 0.0f && (victim->field_0xefe & 0x40) == 0))) return i;
        }
    }
    return -1;
}

void SpecialMove_Cancel(GameObject_s *) {
}

u32 SpecialMove_GetFlags(i32 index, u32 mask) {
    if (index == -1) return 0;
    u32 flags = SpecialMove[index].flags;
    return mask == 0 ? flags : flags & mask;
}

void SpecialMove_VictimCode(GameObject_s *) {
}

void SpecialMoves_Configure(char *, variptr_u *, variptr_u *) {
}

void SpecialMove_ReleaseVictim(GameObject_s *) {
}

void SpecialMove_GetVictimAction(i32) {
}

void SpecialMove_GetDistanceApart(i32) {
}

void SpecialMove_GetAttackerAction(i32) {
}

void SpecialMove_Attacker_SetTargetMom(GameObject_s *) {
}

static __used__ void JediBKilledCallback(GameObject_s *) {
}

void BackFlipCode(GameObject_s *) {
}

void SetSpecialMove(GameObject_s *, AIPATHNODE_s *, AIPATHNODE_s *, char) {
}

void ClearSpecialMove(GameObject_s *) {
}
