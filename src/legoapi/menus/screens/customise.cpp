#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "batman.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/characters/core/character.h"

f32 CustomiseMenuTime[2];
GAMESAVE_s OldCustomiseGame = {};
i32 customiser_save_done = 0;
i32 customiser_quit = 0;
i32 customiser_changed = 0;
extern "C" void ResetAnimPacket(void *, i32);

void Customiser_Init(CUSTOMISER *) {
}

void Customiser_Reset(CUSTOMISER *customiser) {
    if (customiser == NULL) {
        return;
    }

    CustomiseMenuTime[0] = 0.0f;
    ResetAnimPacket(&customiser->animation_packets[0], -1);
    customiser->animation_values[0] = 0;
    customiser->animation_active[0] = 0;
    customiser->animation_state[0] = 2;

    CustomiseMenuTime[1] = 0.0f;
    ResetAnimPacket(&customiser->animation_packets[1], -1);
    customiser->animation_values[1] = 0;
    customiser->animation_active[1] = 0;
    customiser->animation_state[1] = 2;
}

void Customiser_Draw3D(CUSTOMISER *) {
}

void Customiser_Update(CUSTOMISER *, WORLDINFO_s *) {
}

void CustomiserMenu_End() {
}

void Customiser_DumpAll(CUSTOMISER *, WORLDINFO_s *) {
}

void Customiser_GetIcon(CUSTOMISER *, CUSTOMISESAVE_s *, i32) {
}

void Customiser_LoadAll(CUSTOMISER *, WORLDINFO_s *) {
}

void CustomiserMenu_Draw(MENU_s *) {
}

void Customiser_Configure(char *, variptr_u *, variptr_u *, i32, i32, i32 (*)(CUSTOMPIECE *),
                          void (*)(CUSTOMPIECE *, nufpar_s *), i32 (*)(char *), CUSTOMISESAVE_s *, i16 *) {
}

void Customiser_InitNames(CUSTOMISER *) {
}

void CustomiserMenu_Update(MENU_s *) {
}

void Customiser_PieceConfig(CUSTOMPIECE *, nufpar_s *) {
}

i32 Customiser_MenuAvailable(CUSTOMISER *customiser) {
    if (customiser == NULL || APICharacterLoaded(customiser->character_ids[0]) == NULL) {
        return 0;
    }
    const i16 first_animation = customiser->animation_packets[0].requested_animation;
    if (first_animation != 99 && first_animation != 190) {
        return 0;
    }
    if (APICharacterLoaded(customiser->character_ids[1]) == NULL) {
        return 0;
    }
    const i16 second_animation = customiser->animation_packets[1].requested_animation;
    return second_animation == 99 || second_animation == 190;
}

void Customiser_NextPieceLeft(CUSTOMISER *, i32, i32, i32, i32) {
}

void Customiser_NextPieceRight(CUSTOMISER *, i32, i32, i32, i32) {
}

void Customiser_PieceAvailable(CUSTOMPIECE *) {
}

void Customiser_SetAnimsToLoad(CUSTOMISER *, i32) {
}

void Customiser_SetNameAndIcon(CUSTOMISER *, i32) {
}

void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, numtx_s *) {
}

void Customiser_DumpAccessories(CUSTOMISER *) {
}

void Customiser_FindPieceByName(CUSTOMISER *, char *, i32 *, i32 *) {
}

void Customiser_LoadAccessories(CUSTOMISER *, APICHARACTERMODELLIST_s *) {
}

void Customiser_TransformToPanel(CUSTOMISER *) {
}

void Customiser_AddPartAccessories(CUSTOMISER *, GameObject_s *, i32, i32, float) {
}

void Customiser_SetUpCharacterData(CUSTOMISER *) {
}

void Customiser_SaveModelTextureIDs(CUSTOMISER *, CHARACTERMODEL_s *) {
}

void Customiser_Set100PercentPieces(CUSTOMISER *) {
}

void Customiser_GetActiveWeirdoIndex(i32 *, i32 *) {
}

void Customiser_ResetModelTextureIDs(CUSTOMISER *customiser) {
    if (customiser == NULL) {
        return;
    }

    customiser->model_texture_ids[0] = 0;
    customiser->model_texture_ids[1] = 0;
    customiser->model_texture_ids[2] = 0;
    customiser->model_texture_ids[3] = 0;
    customiser->model_texture_ids[4] = 0;
    customiser->model_texture_ids[5] = 0;
    customiser->model_texture_ids[6] = 0;
    customiser->model_texture_ids[7] = 0;
    customiser->model_texture_ids[8] = 0;
    customiser->model_texture_ids[9] = 0;
    customiser->model_texture_ids[10] = 0;
    customiser->model_texture_ids[11] = 0;
    customiser->model_texture_ids[12] = 0;
    customiser->model_texture_ids[13] = 0;
    customiser->model_texture_ids[14] = 0;
    customiser->model_texture_ids[15] = 0;
    customiser->model_texture_ids[16] = 0;
    customiser->model_texture_ids[17] = 0;
}

void Customiser_RestoreModelTextureIDs(CUSTOMISER *) {
}

void Customiser_CopyDefaultPiecesToSave(CUSTOMISER *, CUSTOMISESAVE_s *) {
}

static __used__ bool Customiser_PieceAvailable_Default(CUSTOMPIECE *) {
    return {};
}

void Customise_GetToggleString(i32) {
}
