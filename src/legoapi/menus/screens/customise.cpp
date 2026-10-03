#include "decomp.h"
#include <stdio.h>
#include <string.h>
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nu3d/nuspecial.h"
#include "legoapi/world/world.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/numath/nutrig.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "batman.h"
#include "gameapi/gui/apimenu.h"
#include "legoapi/items/base/animpacket.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/characters/motion.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/render/core/rtl.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/world/area.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/core/render.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/menus/core/panel.h"
#include "gameframework/saveload.h"
#include "legoapi/world/level.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/nucamera.h"

f32 CustomiseMenuTime[2];
GAMESAVE_s OldCustomiseGame = {};
i32 customiser_save_done = 0;
i32 customiser_quit = 0;
i32 customiser_changed = 0;

struct CUSTOMPIECEANIM {
    union {
        f32 duration;
        f32 field_00;
    };
    union {
        f32 elapsed;
        f32 field_04;
    };
    union {
        f32 hold_time;
        f32 delay;
    };
    union {
        i16 angles[3];
        struct {
            u16 start_angle, target_angle, current_angle;
        };
    };
    u8 reserved_12[2];
};
DECOMP_ASSERT(sizeof(CUSTOMPIECEANIM) == 0x14, "Customise motion size");
static i32 CustomiseMode[2];
static u16 CustomiseYRot[2];
static CUSTOMPIECEANIM HeadAnim[2], ArmsAnim[2], LegsAnim[2];
static NUVEC NoHatPos[2];
static u16 CustomiseBob[2];
static f32 Customise_NameAlpha;
static f32 CustomiseNameBoardTMul[2], CustomiseNameBoardMul[2], CustomiseNameLetterBlipScale[2];
static NUVEC CustomiseScreenPos[2];
static i32 CustomiseMenuY[2], CustomiseNameX[2], CustomiseNameY[2];
static const char CustomiseLetter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ- 0123456789";
u16 CustomiseRotY[2], CustomiseTiltX[2], CustomiseTiltZ[2];
void Customiser_SetNameAndIcon(CUSTOMISER *, i32);
void Hub_ClearStats();
void Text_FillInExtendedSaveInfo();
extern i16 tPLAYER1, tPLAYER2;

static void UpdateCustomPieceAnim(CUSTOMPIECEANIM *anim, u16 minimum, u16 maximum) {
    if (anim->duration > anim->elapsed) {
        anim->elapsed += FRAMETIME;
        if (anim->elapsed >= anim->duration) {
            anim->elapsed = anim->duration;
            anim->hold_time = static_cast<f32>(qrand()) * 1.5259021893143654e-05f * 0.5f + 0.5f;
        }
        i32 difference = RotDiff(anim->start_angle, anim->target_angle);
        f32 blend =
            1.0f - (NU_SIN_LUT(static_cast<i32>(anim->elapsed / anim->duration * 32768.0f + 16384.0f)) + 1.0f) * 0.5f;
        anim->current_angle = static_cast<i32>(anim->start_angle + static_cast<f32>(difference) * blend);
    } else {
        anim->hold_time -= FRAMETIME;
        if (anim->hold_time <= 0.0f) {
            anim->start_angle = anim->current_angle;
            i32 difference = RotDiff(minimum, maximum);
            anim->target_angle = static_cast<i32>(minimum + static_cast<f32>(difference) *
                                                                (static_cast<f32>(qrand()) * 1.5259021893143654e-05f));
            anim->elapsed = 0.0f;
            anim->duration = static_cast<f32>(qrand()) * 1.5259021893143654e-05f + 1.0f;
        }
    }
}

// Animations the customiser cycles through on the game's character preview, terminated by -1.
i16 CustomiserActionList_Game[] = {97, 190, 98, 99, -1};

struct CUSTOMISER_GAMESETTING {
    char *name;
    u32 model_flags;
    u32 gameplay_flags;
};
static CUSTOMISER_GAMESETTING Customiser_GameSetting[] = {
    {"bountyhunter", 0x1000000, 0},
    {"jedi", 8, 0},
    {"sith", 0xc, 2},
    {"blaster", 0x100080, 0x40000000},
    {"alreadygothat", 0, 0x10},
    {"stormtrooperhelmet", 0, 0x40000},
    {NULL, 0, 0},
};

void Customiser_Init(CUSTOMISER *customiser) {
    if (customiser == NULL)
        return;
    for (i32 i = 0; i < 2; i++) {
        ResetAnimPacket(&customiser->animation_packets[i], -1);
        customiser->animation_values[i] = 0;
        customiser->animation_active[i] = 0;
        customiser->animation_state[i] = 2;
        CustomiseMode[i] = 2;
    }
    if (WORLD->camera_splines[18] != NULL) {
        {

            NUVEC *points = WORLD->camera_splines[18]->pts;
            CustomisePos[0] = points[0 * 2];
            CustomiseYRot[0] =
                NuAtan2D(points[0 * 2 + 1].x - CustomisePos[0].x, points[0 * 2 + 1].z - CustomisePos[0].z);
            HeadAnim[0].angles[2] = 0;
            HeadAnim[0].delay = ((static_cast<f32>(qrand()) * 1.5259021893143654e-05f) * 0.5f) * 0.5f;
            HeadAnim[0].angles[0] = 0;
            HeadAnim[0].angles[1] = 0;
            HeadAnim[0].field_04 = 0.0f;
            HeadAnim[0].field_00 = 0.0f;
            ArmsAnim[0].angles[2] = 0;
            ArmsAnim[0].delay = ((static_cast<f32>(qrand()) * 1.5259021893143654e-05f) * 0.5f) * 0.5f;
            ArmsAnim[0].angles[0] = 0;
            ArmsAnim[0].angles[1] = 0;
            ArmsAnim[0].field_04 = 0.0f;
            ArmsAnim[0].field_00 = 0.0f;
            LegsAnim[0].angles[2] = 0;
            LegsAnim[0].delay = ((static_cast<f32>(qrand()) * 1.5259021893143654e-05f) * 0.5f) * 0.5f;
            LegsAnim[0].angles[0] = 0;
            LegsAnim[0].angles[1] = 0;
            LegsAnim[0].field_04 = 0.0f;
            LegsAnim[0].field_00 = 0.0f;
        }
        {

            NUVEC *points = WORLD->camera_splines[18]->pts;
            CustomisePos[1] = points[1 * 2];
            CustomiseYRot[1] =
                NuAtan2D(points[1 * 2 + 1].x - CustomisePos[1].x, points[1 * 2 + 1].z - CustomisePos[1].z);
            HeadAnim[1].angles[2] = 0;
            HeadAnim[1].delay = ((static_cast<f32>(qrand()) * 1.5259021893143654e-05f) * 0.5f) * 0.5f;
            HeadAnim[1].angles[0] = 0;
            HeadAnim[1].angles[1] = 0;
            HeadAnim[1].field_04 = 0.0f;
            HeadAnim[1].field_00 = 0.0f;
            ArmsAnim[1].angles[2] = 0;
            ArmsAnim[1].delay = ((static_cast<f32>(qrand()) * 1.5259021893143654e-05f) * 0.5f) * 0.5f;
            ArmsAnim[1].angles[0] = 0;
            ArmsAnim[1].angles[1] = 0;
            ArmsAnim[1].field_04 = 0.0f;
            ArmsAnim[1].field_00 = 0.0f;
            LegsAnim[1].angles[2] = 0;
            LegsAnim[1].delay = ((static_cast<f32>(qrand()) * 1.5259021893143654e-05f) * 0.5f) * 0.5f;
            LegsAnim[1].angles[0] = 0;
            LegsAnim[1].angles[1] = 0;
            LegsAnim[1].field_04 = 0.0f;
            LegsAnim[1].field_00 = 0.0f;
        }

        CustomiseRotY[0] = qrand();
        CustomiseRotY[1] = CustomiseRotY[0] + 0x4000;
        CustomiseTiltX[0] = qrand();
        CustomiseTiltX[1] = CustomiseTiltX[0] + 0x8000;
        CustomiseTiltZ[0] = qrand();
        CustomiseTiltZ[1] = CustomiseTiltZ[0] + 0x8000;
        CustomiseBob[0] = qrand();
        CustomiseBob[1] = CustomiseBob[0] + 0x8000;
    }
    Customiser_SetNameAndIcon(customiser, -1);
    Customise_NameAlpha = 0.0f;
    if (customiser->piece_counts[0] > 0)
        Game.customizer.pieces[0] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[0]) - 1, customiser->piece_counts[0], 0, 0);
    if (customiser->piece_counts[1] > 0)
        Game.customizer.pieces[1] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[1]) - 1, customiser->piece_counts[1], 0, 1);
    if (customiser->piece_counts[2] > 0)
        Game.customizer.pieces[2] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[2]) - 1, customiser->piece_counts[2], 0, 2);
    if (customiser->piece_counts[3] > 0)
        Game.customizer.pieces[3] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[3]) - 1, customiser->piece_counts[3], 0, 3);
    if (customiser->piece_counts[4] > 0)
        Game.customizer.pieces[4] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[4]) - 1, customiser->piece_counts[4], 0, 4);
    if (customiser->piece_counts[5] > 0)
        Game.customizer.pieces[5] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[5]) - 1, customiser->piece_counts[5], 0, 5);
    if (customiser->piece_counts[6] > 0)
        Game.customizer.pieces[6] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[6]) - 1, customiser->piece_counts[6], 0, 6);
    if (customiser->piece_counts[7] > 0)
        Game.customizer.pieces[7] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[7]) - 1, customiser->piece_counts[7], 0, 7);
    if (customiser->piece_counts[8] > 0)
        Game.customizer.pieces[8] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.pieces[8]) - 1, customiser->piece_counts[8], 0, 8);
    if (customiser->piece_counts[0] > 0)
        Game.customizer.secondary_pieces[0] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[0]) - 1, customiser->piece_counts[0], 1, 0);
    if (customiser->piece_counts[1] > 0)
        Game.customizer.secondary_pieces[1] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[1]) - 1, customiser->piece_counts[1], 1, 1);
    if (customiser->piece_counts[2] > 0)
        Game.customizer.secondary_pieces[2] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[2]) - 1, customiser->piece_counts[2], 1, 2);
    if (customiser->piece_counts[3] > 0)
        Game.customizer.secondary_pieces[3] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[3]) - 1, customiser->piece_counts[3], 1, 3);
    if (customiser->piece_counts[4] > 0)
        Game.customizer.secondary_pieces[4] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[4]) - 1, customiser->piece_counts[4], 1, 4);
    if (customiser->piece_counts[5] > 0)
        Game.customizer.secondary_pieces[5] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[5]) - 1, customiser->piece_counts[5], 1, 5);
    if (customiser->piece_counts[6] > 0)
        Game.customizer.secondary_pieces[6] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[6]) - 1, customiser->piece_counts[6], 1, 6);
    if (customiser->piece_counts[7] > 0)
        Game.customizer.secondary_pieces[7] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[7]) - 1, customiser->piece_counts[7], 1, 7);
    if (customiser->piece_counts[8] > 0)
        Game.customizer.secondary_pieces[8] = Customiser_NextPieceRight(
            customiser, static_cast<u16>(Game.customizer.secondary_pieces[8]) - 1, customiser->piece_counts[8], 1, 8);
    CustomiseNameBoardTMul[1] = 0.0f;
    CustomiseNameBoardMul[0] = 0.0f;
    CustomiseNameLetterBlipScale[1] = 1.0f;
    CustomiseNameLetterBlipScale[0] = 1.0f;
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

i32 Customiser_PieceAvailable(CUSTOMPIECE *piece) {
    if (GAMEDEMO != 0) {
        if ((piece->availability_flags & 0x10) != 0)
            return 0;
    } else {
        if ((piece->availability_flags & 0x180) != 0 && Game_100PercentComplete() == 0)
            return 0;
        if (piece->character_id != -1) {
            if (InCollectList_Index(piece->character_id, NULL, 0) == -1)
                return 1;
            if (Collection_Got(piece->character_id) == 0)
                return 0;
        }
        const u32 required_flags = piece->model_flags & 0x0c;
        if (required_flags != 0 && Collection_GotAnyOfType(-1, required_flags) == 0)
            return 0;
        if (piece->collection_type != -1 && Collection_GotAnyOfType(piece->collection_type, 0) == 0)
            return 0;
    }
    return 1;
}

void Customiser_TransformToPanel(CUSTOMISER *) {
    NuCameraTransformScreenClip(&CustomiseScreenPos[0], &CustomisePos[0], 1, NULL);
    NuCameraTransformScreenClip(&CustomiseScreenPos[1], &CustomisePos[1], 1, NULL);
}

static const i16 *Customiser_GetSelection(i32 side) {
    return side == 0 ? Game.customizer.pieces : Game.customizer.secondary_pieces;
}

static CUSTOMPIECE *Customiser_GetSelectedPiece(CUSTOMISER *customiser, const i16 *selection, i32 category) {
    if (customiser->piece_sets[category] == NULL || customiser->piece_counts[category] <= 0) {
        return NULL;
    }
    i32 index = selection[category];
    if (index < 0 || index >= customiser->piece_counts[category]) {
        index = 0;
    }
    return &customiser->piece_sets[category][index];
}

static u32 Customiser_GetLayerMask(CUSTOMISER *customiser, GAMECHARACTERDATA *runtime, const i16 *selection) {
    u32 layer_mask = 0;
    for (i32 category = 0; category < 9; ++category) {
        // Slot five is the cape selection; its hierarchy layer is supplied by the character data.
        if (category != 5 && customiser->layer_indices[category] != -1) {
            layer_mask |= 1u << (static_cast<u8>(customiser->layer_indices[category]) & 31);
        }
    }
    if (layer_mask == 0) {
        layer_mask = 1;
    }
    if (runtime->cape_layer != -1) {
        const u32 cape_mask = 1u << (static_cast<u8>(runtime->cape_layer) & 31);
        layer_mask |= cape_mask;
        CUSTOMPIECE *cape = Customiser_GetSelectedPiece(customiser, selection, 5);
        if (cape != NULL && (cape->layer_flags & 0x40) != 0) {
            layer_mask &= ~cape_mask;
        }
    }
    return layer_mask;
}

#define CUSTOM_SETUP_LAYER(category)                                                                                   \
    if (customiser->layer_indices[category] != -1)                                                                     \
        layer_mask |= 1u << (static_cast<u8>(customiser->layer_indices[category]) & 31);

#define CUSTOM_SETUP_PIECE(category)                                                                                   \
    {                                                                                                                  \
        CUSTOMPIECE *piece = Customiser_GetSelectedPiece(customiser, selection, category);                             \
        if (piece != NULL) {                                                                                           \
            character->model_flags |= piece->model_flags;                                                              \
            if ((character->model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0)                                 \
                character->model_flags |= 0x10000000;                                                                  \
            runtime->flags_090 |= piece->gameplay_flags;                                                               \
        }                                                                                                              \
    }

#define CUSTOM_SETUP_SIDE(side, saved_selection)                                                                       \
    do {                                                                                                               \
        const i32 character_id = customiser->character_ids[side];                                                      \
        if (character_id < 0 || character_id >= CHARCOUNT)                                                             \
            break;                                                                                                     \
        CHARACTERDATA *character = &CDataList[character_id];                                                           \
        GAMECHARACTERDATA *runtime = character->game_character;                                                        \
        if (runtime == NULL)                                                                                           \
            break;                                                                                                     \
        runtime->flags_090 = 0;                                                                                        \
        const i16 *selection = saved_selection;                                                                        \
        u32 layer_mask = 0;                                                                                            \
        CUSTOM_SETUP_LAYER(0)                                                                                          \
        CUSTOM_SETUP_LAYER(1)                                                                                          \
        CUSTOM_SETUP_LAYER(2)                                                                                          \
        CUSTOM_SETUP_LAYER(3)                                                                                          \
        CUSTOM_SETUP_LAYER(4)                                                                                          \
        CUSTOM_SETUP_LAYER(6)                                                                                          \
        CUSTOM_SETUP_LAYER(7)                                                                                          \
        CUSTOM_SETUP_LAYER(8)                                                                                          \
        if (layer_mask == 0)                                                                                           \
            layer_mask = 1;                                                                                            \
        if (runtime->cape_layer != -1) {                                                                               \
            const u32 cape_mask = 1u << (static_cast<u8>(runtime->cape_layer) & 31);                                   \
            layer_mask |= cape_mask;                                                                                   \
            CUSTOMPIECE *cape = Customiser_GetSelectedPiece(customiser, selection, 5);                                 \
            if (cape != NULL && (cape->layer_flags & 0x40) != 0)                                                       \
                layer_mask &= ~cape_mask;                                                                              \
        }                                                                                                              \
        runtime->layer_mask_special = layer_mask;                                                                      \
        runtime->layer_mask = layer_mask;                                                                              \
        runtime->layer_mask_medium = layer_mask;                                                                       \
        runtime->layer_mask_low = layer_mask;                                                                          \
        runtime->layer_mask_dead = layer_mask;                                                                         \
        character->model_flags &= 3;                                                                                   \
        CUSTOMPIECE *torso = Customiser_GetSelectedPiece(customiser, selection, 1);                                    \
        const bool torso_replaces_base = torso != NULL && (torso->layer_flags & 1) != 0;                               \
        CUSTOMPIECE *head = Customiser_GetSelectedPiece(customiser, selection, 0);                                     \
        if (head != NULL) {                                                                                            \
            if (!torso_replaces_base)                                                                                  \
                character->model_flags |= head->model_flags;                                                           \
            if ((character->model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0)                                 \
                character->model_flags |= 0x10000000;                                                                  \
            runtime->flags_090 = head->gameplay_flags;                                                                 \
        }                                                                                                              \
        CUSTOM_SETUP_PIECE(1)                                                                                          \
        CUSTOM_SETUP_PIECE(2)                                                                                          \
        CUSTOM_SETUP_PIECE(3)                                                                                          \
        CUSTOM_SETUP_PIECE(4)                                                                                          \
        CUSTOM_SETUP_PIECE(5)                                                                                          \
        CUSTOM_SETUP_PIECE(6)                                                                                          \
        CUSTOM_SETUP_PIECE(7)                                                                                          \
        CUSTOM_SETUP_PIECE(8)                                                                                          \
        if (torso_replaces_base)                                                                                       \
            runtime->flags_090 |= 0x10;                                                                                \
        CUSTOMPIECE *weapon = Customiser_GetSelectedPiece(customiser, selection, 2);                                   \
        if (weapon != NULL) {                                                                                          \
            runtime->weapon_model = weapon->weapon_model;                                                              \
            runtime->field_0x117 = static_cast<u8>(LightSabre_ColourFromObj(runtime->weapon_model, NULL));             \
        }                                                                                                              \
    } while (0)

void Customiser_SetUpCharacterData(CUSTOMISER *customiser) {
    if (customiser == NULL)
        return;
    // The reference has two fixed previews and nine fixed piece categories.
    CUSTOM_SETUP_SIDE(0, Game.customizer.pieces);
    CUSTOM_SETUP_SIDE(1, Game.customizer.secondary_pieces);
    Customiser_SetNameAndIcon(customiser, -1);
}
#undef CUSTOM_SETUP_SIDE
#undef CUSTOM_SETUP_PIECE
#undef CUSTOM_SETUP_LAYER

void Customiser_Draw3D(CUSTOMISER *customiser) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (customiser == NULL || world == NULL || world->camera_splines == NULL || world->camera_splines[18] == NULL)
        return;
    i32 active_side, active_count;
    Customiser_GetActiveWeirdoIndex(&active_side, &active_count);
    i32 preview_side = -1;
    if (active_count == 1 && active_side >= 0 && active_side < 2)
        preview_side = MenuPacket.customise_other_player[active_side] != 0 ? 1 - active_side : active_side;
    for (i32 side = 0; side < 2; ++side) {
        const i32 character_id = customiser->character_ids[side];
        if (character_id < 0 || character_id >= CHARCOUNT)
            continue;
        CHARACTERMODEL_s *model = APICharacterLoaded(character_id);
        if (model == NULL || customiser->animation_state[side] != 0)
            continue;
        if (GCDataList == NULL)
            continue;
        GAMECHARACTERDATA *runtime = &GCDataList[character_id];
        i32 helmet = runtime->helmet_locator;
        if (helmet < 0 || helmet >= 16 || model->points_of_interest[helmet] == NULL)
            helmet = -1;
        const i32 selected = CustomiseMenuY[side];
        rtldata_s light_data;
        rtlResetEx(&light_data, 1);
        rtlApplySetScale(world->rtl_set, &light_data, &CustomisePos[side], NULL, -1, 1.0f);
        SetLights_RTLDATA(&light_data, 1.0f);
        const i16 *selection = Customiser_GetSelection(side);
        u32 layer_mask = Customiser_GetLayerMask(customiser, runtime, selection);
        if (runtime->cape_layer != -1 && selected == 5 && GetMenuID() == 12 && customiser_quit == 0)
            layer_mask |= 1u << (static_cast<u8>(runtime->cape_layer) & 31);
        // The reference's preview matrix lives in a 16-byte-aligned frame.
        NUMTX_ALIGNED16 matrix;
        NuMtxSetRotationY(&matrix, CustomiseYRot[side] + 0x8000);
        NuMtxTranslate(&matrix, &CustomisePos[side]);
        if (GameDrawCharacterModel(model, &customiser->animation_packets[side], &matrix, NULL, NULL,
                                   customiser->joint_matrices[side], NULL, layer_mask) == 0)
            continue;
        if (helmet != -1) {
            for (i32 category = 0; category < 9; ++category) {
                if (customiser->categories[category] == NULL || customiser->categories[category]->uses_special == 0 ||
                    world->customiser_resources[category] == NULL || customiser->piece_counts[category] <= 0)
                    continue;
                const i32 piece_index = selection[category];
                if (piece_index < 0 || piece_index >= customiser->piece_counts[category])
                    continue;
                CUSTOMPIECERESOURCE *resource = &world->customiser_resources[category][piece_index];
                if (NuSpecialExistsFn(&resource->special) == 0)
                    continue;
                const bool preview = GetMenuID() == 12 && customiser_quit == 0 && category == selected &&
                                     (active_count == 2 || side == preview_side);
                if (preview)
                    NuFmod(CustomiseMenuTime[side], 0.3f);
                if (category == 0 && !preview) {
                    CUSTOMPIECE *piece = Customiser_GetSelectedPiece(customiser, selection, category);
                    if (piece != NULL && (piece->layer_flags & 0x20) != 0)
                        continue;
                }
                matrix = customiser->joint_matrices[side][helmet];
                if (category == 0)
                    NuMtxPreTranslate(&matrix, &NoHatPos[side]);
                NuSpecialDrawAt(&resource->special, &matrix);
            }
        }
        const i32 hand = runtime->weapon_joints[0];
        CUSTOMPIECE *weapon = Customiser_GetSelectedPiece(customiser, selection, 2);
        if (weapon != NULL && hand >= 0 && hand < 16 && model->points_of_interest[hand] != NULL &&
            weapon->weapon_model >= 0 && world->lev_objs != NULL) {
            LEVEL_OBJECT_RUNTIME_s *object = &world->lev_objs[weapon->weapon_model];
            if (object->active != 0) {
                matrix = customiser->joint_matrices[side][hand];
                NuSpecialDrawAt(&object->special, &matrix);
                i32 blade = -1;
                if (LightSabre_ColourFromObj(weapon->weapon_model, &blade) != -1) {
                    if (blade >= 0)
                        NuSpecialDrawAt(&world->lev_objs[blade].special, &matrix);
                    NuSpecialDrawAt(&world->lev_objs[17].special, &matrix);
                }
            }
        }
    }
    SetLevelLights(world->rtl_set, 1.0f);
}

void Customiser_Update(CUSTOMISER *customiser, WORLDINFO_s *world) {
    if (customiser == NULL)
        return;
    i32 active_side, active_count;
    Customiser_GetActiveWeirdoIndex(&active_side, &active_count);
    i32 preview_side = -1;
    if (active_count == 1 && active_side >= 0 && active_side < 2)
        preview_side = MenuPacket.customise_other_player[active_side] != 0 ? 1 - active_side : active_side;
    const u8 colour = menu_flash != 0 ? 255 : 191;
    for (i32 side = 0; side < 2; ++side) {
        const i32 control_side = active_count == 2                  ? side
                                 : MenuPacket.active_player[0] != 0 ? 0
                                 : MenuPacket.active_player[1] != 0 ? 1
                                                                    : -1;
        CustomiseRotY[side] += static_cast<u16>(static_cast<i32>(10922.0f * FRAMETIME));
        CustomiseTiltX[side] += static_cast<u16>(static_cast<i32>(12379.0f * FRAMETIME));
        CustomiseTiltZ[side] += static_cast<u16>(static_cast<i32>(10376.0f * FRAMETIME));
        CustomiseBob[side] += static_cast<u16>(static_cast<i32>(20206.0f * FRAMETIME));
        UpdateCustomPieceAnim(&HeadAnim[side], 0xd556, 0x2aaa);
        UpdateCustomPieceAnim(&ArmsAnim[side], 0x1555, 0x4000);
        UpdateCustomPieceAnim(&LegsAnim[side], 0xe000, 0x4000);
        const i16 *selection = Customiser_GetSelection(side);
        CUSTOMPIECE *head = Customiser_GetSelectedPiece(customiser, selection, 0);
        CUSTOMPIECE *torso = Customiser_GetSelectedPiece(customiser, selection, 1);
        f32 no_hat = 0.0f;
        if (torso != NULL && (torso->layer_flags & 1) != 0) {
            if ((torso->layer_flags & 4) != 0)
                no_hat = head != NULL && (head->layer_flags & 8) != 0 ? 0.21f : 0.168f;
            else
                no_hat = head != NULL && (head->layer_flags & 8) != 0 ? 0.168f : 0.126f;
        }
        NoHatPos[side].y = SeekValF(NoHatPos[side].y, no_hat, 10.0f);
        CustomiseMenuTime[side] += FRAMETIME;
        CHARACTERMODEL_s *model = APICharacterLoaded(customiser->character_ids[side]);
        if (model == NULL)
            continue;
        CUSTOMPIECE *weapon = Customiser_GetSelectedPiece(customiser, selection, 2);
        const i32 saber_colour = weapon == NULL ? -1 : LightSabre_ColourFromObj(weapon->weapon_model, NULL);
        if (customiser->animation_state[side] != 0)
            --customiser->animation_state[side];
        ANIMPACKET_s *packet = &customiser->animation_packets[side];
        const i16 intro = saber_colour == -1 ? 0x62 : 0x61;
        if (customiser->animation_active[side] == 0 && model->model_data_b != NULL &&
            model->model_data_b[intro] != NULL) {
            if (customiser->animation_values[side] == 0.0f) {
                customiser->animation_values[side] = AnimDuration(model->model_id, intro, 0.0f, 0.0f, 1);
            } else {
                customiser->animation_values[side] -= FRAMETIME;
                if (customiser->animation_values[side] <= 0.0f)
                    customiser->animation_active[side] = 1;
            }
        } else {
            customiser->animation_active[side] = 1;
        }
        packet->previous_animation = packet->animation_index;
        packet->requested_animation =
            customiser->animation_active[side] == 0 ? intro : static_cast<i16>(saber_colour == -1 ? 99 : 0xbe);
        UpdateAnimPacket(model, packet, FRAMETIME * 30.0f, 0.0f, FRAMETIME, 0.0f);
        const u16 angle = CustomiseYRot[side] + 0xc000;
        for (i32 category = 0; category < 9; ++category) {
            const i32 selected = CustomiseMenuY[side];
            const i32 locator = customiser->locator_indices[category];
            if (selected == category && GetMenuID() == 12 && customiser_quit == 0 &&
                (active_count == 2 || side == preview_side) && control_side != -1 && locator >= 0 && locator < 16 &&
                model->points_of_interest[locator] != NULL) {
                const NUMTX &joint = customiser->joint_matrices[side][locator];
                NUVEC position = {joint.m30, joint.m31, joint.m32};
                const f32 offset = customiser->locator_x_offsets[category];
                if (offset != 0.0f) {
                    position.x += offset * NU_SIN_LUT(angle);
                    position.z += offset * NU_COS_LUT(angle);
                }
                position.y += customiser->locator_y_offsets[category];
                f32 scale = 1.0f;
                if (TestForController() == 0) {
                    const f32 idle = GlobalTimer.time_elapsed - (LastTouchTime + 1.0f);
                    if (idle > 4.0f) {
                        const i32 phase = static_cast<i32>(NuFmod(idle, 4.0f) * 0.25f * 65536.0f);
                        const f32 pulse = NU_SIN_LUT(phase) - 0.8f;
                        if (pulse >= 0.0f)
                            scale += pulse;
                    }
                }
                scale *= 1.5f;
                const f32 width = 0.125f * scale;
                const f32 height = width / GetAspectRatio();
                NUVEC screen;
                NuCameraTransformScreenClip(&screen, &position, 1, NULL);
                Text3D("<", screen.x - width, screen.y, 1.0f, scale, scale, scale, 0, colour, colour, colour);
                customiser->touch_widths[0] = width;
                customiser->touch_heights[0] = height;
                customiser->touch_positions[0].x = screen.x - width;
                customiser->touch_positions[0].y = screen.y;
                Text3D(">", screen.x + width, screen.y, 1.0f, scale, scale, scale, 0, colour, colour, colour);
                customiser->touch_widths[1] = width;
                customiser->touch_heights[1] = height;
                customiser->touch_positions[1].x = screen.x + width;
                customiser->touch_positions[1].y = screen.y;
                if (category > 0) {
                    Text3D(ASCII_UP, screen.x, screen.y + height, 1.0f, scale, scale, scale, 0, colour, colour, colour);
                    customiser->touch_positions[2].x = screen.x;
                    customiser->touch_positions[2].y = screen.y + height;
                    customiser->touch_widths[2] = width;
                    customiser->touch_heights[2] = height;
                } else {
                    customiser->touch_widths[2] = 0.0f;
                }
                if (category < 8) {
                    Text3D(ASCII_DOWN, screen.x, screen.y - height, 1.0f, scale, scale, scale, 0, colour, colour,
                           colour);
                    customiser->touch_positions[3].x = screen.x;
                    customiser->touch_positions[3].y = screen.y - height;
                    customiser->touch_widths[3] = width;
                    customiser->touch_heights[3] = height;
                } else {
                    customiser->touch_widths[3] = 0.0f;
                }
            }
            if (world == NULL || model->hierarchy == NULL)
                continue;
            CUSTOMPIECECATEGORY *category_data = customiser->categories[category];
            CUSTOMPIECERESOURCE *resources = world->customiser_resources[category];
            if (category_data == NULL || category_data->uses_special != 0 || category_data->material_tag == -1 ||
                resources == NULL || customiser->piece_counts[category] <= 0)
                continue;
            const i32 piece_index = selection[category];
            if (piece_index < 0 || piece_index >= customiser->piece_counts[category] ||
                resources[piece_index].texture_id == 0)
                continue;
            for (i32 material = 0; material < model->hierarchy->material_count; ++material) {
                NUMTL *entry = model->hierarchy->materials[material];
                if (entry != NULL && entry->unknown_9a[0] == static_cast<u8>(category_data->material_tag)) {
                    entry->tex_id = resources[piece_index].texture_id;
                    NuMtlUpdate(entry);
                }
            }
        }
    }
    const f32 target = GetMenuID() == 12 && customiser_quit == 0 ? 1.0f : 0.0f;
    Customise_NameAlpha = SeekLinearF(Customise_NameAlpha, target, 1.5f * FRAMETIME);
}

void CustomiserMenu_End() {
    LevLock[1] = 1;
    LevTime[1] = 2.0f;
    MenuRememberCursor(&GameMenu[GameMenuLevel]);
    MenuReset();
    ResetTimer(&JoinInTimer, 0.0f);
    CustomiseMode[0] = 2;
    CustomiseMode[1] = 2;
    if (GAMEDEMO != 0) {
        GAMEDEMO = 2;
        NewLData = HUB_LDATA;
        NextArea_FreePlay = 1;
        FreePlay = 1;
        makeplayerlist_freeplay = 1;
        makefreeplaymodellist = 1;
    }
}

void CustomiserMenu_Draw(MENU_s *) {
    if (MenuStopDraw != 0 || customiser_quit != 0 || CharacterCustomiser == NULL)
        return;
    CUSTOMISER *customiser = CharacterCustomiser;
    i32 side, active_count;
    if (Customise_NameAlpha > 0.0f) {
        Customiser_GetActiveWeirdoIndex(&side, &active_count);
        if (active_count == 1 && MenuPacket.customise_other_player[side] != 0)
            side = side == 0;
        for (i32 index = 0; index < active_count; ++index, ++side) {
            const u8 blue = side == 0 ? 255 : 0;
            const u8 green = side == 0 ? 191 : 255;
            const u8 red = side == 0 ? 0 : 127;
            if (CustomiseMode[side] == 1) {
                const char *name = side == 0 ? Game.customizer.primary_name : Game.customizer.secondary_name;
                char letter[2];
                letter[1] = 0;
                const f32 gap = Game.options_save.widescreen == 0 ? 0.05249999836087227f : 0.041999999433755875f;
                const f32 half_width = Game.options_save.widescreen == 0 ? 0.367499977350235f : 0.2939999997615814f;
                i32 length = 15;
                while (length > 0 && name[length - 1] == ' ')
                    --length;
                f32 x = CustomiseScreenPos[side].x - half_width;
                if (active_count == 2)
                    x += side == 0 ? -0.06f : 0.06f;
                for (i32 column = 0; column != 15; ++column) {
                    f32 scale;
                    u8 letter_red, letter_green, letter_blue;
                    if (CustomiseNameX[side] == column) {
                        letter[0] = CustomiseLetter[CustomiseNameY[side]];
                        const u8 arrow_colour = menu_flash == 0 ? 255 : 191;
                        Text3D(ASCII_UP, x, -0.75f, 1.0f, 0.6f, 0.6f, 0.6f, 0, arrow_colour, arrow_colour,
                               arrow_colour);
                        Text3D(ASCII_DOWN, x, -0.9500000476837158f, 1.0f, 0.6f, 0.6f, 0.6f, 0, arrow_colour,
                               arrow_colour, arrow_colour);
                        letter_red = 255;
                        letter_green = menu_flash == 0 ? 255 : 0;
                        letter_blue = 255;
                        scale = CustomiseNameLetterBlipScale[side];
                    } else {
                        letter[0] = name[column];
                        letter_red = red;
                        letter_green = green;
                        letter_blue = blue;
                        scale = 1.0f;
                        if (CustomiseNameX[side] < column && length <= column && letter[0] == ' ')
                            letter[0] = '.';
                    }
                    Text3D(letter, x, -0.85f, 1.0f, scale * 0.5f, scale * 0.5f, 0.5f, 0, letter_red, letter_green,
                           letter_blue);
                    if (column == 0)
                        Text3D("[", (x - gap) + 0.01f, -0.8425000309944153f, 1.0f, 0.5f, 0.6f, 0.5f, 0, red, green,
                               blue);
                    else if (column == 14)
                        Text3D("]", (x + gap) - 0.01f, -0.8425000309944153f, 1.0f, 0.5f, 0.6f, 0.5f, 0, red, green,
                               blue);
                    x += gap;
                }
            } else {
                char name[64];
                GameObj_GetName(customiser->character_ids[side], NULL, name);
                SmartTextEx(name, CustomiseScreenPos[side].x, -0.85f, 1.0f, 0.5f, 0.5f, 0.5f, 0, red, green, blue,
                            0.75f, 2, NULL, 0, static_cast<i32>(128.0f * Customise_NameAlpha));
            }
            const f32 icon_size = ICONSIZE;
            f32 pulse = 1.0f;
            if (!TestForController()) {
                const f32 idle = GlobalTimer.time_elapsed - (2.0f + LastTouchTime);
                if (idle > 4.0f) {
                    const f32 time = NuFmod(idle, 4.0f);
                    const f32 excess = NuTrigTable[(static_cast<i32>(time * 0.25f * 65536.0f) >> 1) & 0x7fff] - 0.8f;
                    if (excess >= 0.0f)
                        pulse = 1.0f + excess;
                }
            }
            const f32 x = CustomiseScreenPos[side].x;
            const f32 size = pulse * 0.1f * icon_size;
            const f32 alpha = (1.0f - CustomiseNameBoardMul[side]) * Customise_NameAlpha;
            DrawPanel3DObject(x, -0.6f, 1.0f, size, size, size, 0, 0, 0, &WORLD->lev_objs[167].special, 0, alpha);
            CUSTOMISESAVE_s *save =
                side == 0 ? &Game.customizer : reinterpret_cast<CUSTOMISESAVE_s *>(Game.customizer.secondary_pieces);
            const i32 icon = Customiser_GetIcon(customiser, save, index);
            if (icon != -1)
                DrawPanel3DObject(x, -0.6f, 1.0f, size, size, size, 0, 0, 0,
                                  &WORLD->lev_objs[icon + (icon != LEGOOBJ_ICON_WEIRDO)].special, 0, alpha);
            customiser->touch_positions[4].x = x;
            customiser->touch_positions[4].y = -0.6f;
            customiser->touch_widths[4] = size;
            customiser->touch_heights[4] = size / GetAspectRatio();
            pulse = 1.0f;
            if (!TestForController()) {
                const f32 idle = GlobalTimer.time_elapsed - (3.0f + LastTouchTime);
                if (idle > 4.0f) {
                    const f32 time = NuFmod(idle, 4.0f);
                    const f32 excess = NuTrigTable[(static_cast<i32>(time * 0.25f * 65536.0f) >> 1) & 0x7fff] - 0.8f;
                    if (excess >= 0.0f)
                        pulse = 1.0f + excess;
                }
            }
            const f32 width = 0.125f * pulse;
            const f32 height = width / GetAspectRatio();
            const u8 colour = menu_flash == 0 ? 255 : 191;
            const f32 toggle_x = side == 0 ? x + 0.2f : x - 0.2f;
            Text3D(const_cast<char *>(side == 0 ? ">" : "<"), toggle_x, -0.6f, 1.0f, pulse, pulse, pulse, 0, colour,
                   colour, colour);
            customiser->touch_positions[5].x = toggle_x;
            customiser->touch_positions[5].y = -0.6f;
            customiser->touch_widths[5] = width;
            customiser->touch_heights[5] = height;
        }
    }
    Customiser_GetActiveWeirdoIndex(&side, &active_count);
    const f32 alpha = MenuPacket.active_player[0] == 0 ? DROPINALPHA : 1.0f;
    DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 0xa6, alpha, alpha, 1, NULL);
    if (GAMEDEMO == 0)
        Customise_GetToggleString(active_count == 1 && MenuPacket.customise_other_player[side] != 0);
}

void Customiser_InitNames(CUSTOMISER *customiser) {
    if (customiser == NULL)
        return;
    if (customiser->character_ids[0] != -1) {
        const i32 text = CDataList[customiser->character_ids[0]].name_id;
        if (text != -1) {
            NuStrCpy(customiser->display_names[0], TTab[text]);
            TTab[text] = customiser->display_names[0];
        }
    }
    if (customiser->character_ids[1] != -1) {
        const i32 text = CDataList[customiser->character_ids[1]].name_id;
        if (text != -1) {
            NuStrCpy(customiser->display_names[1], TTab[text]);
            TTab[text] = customiser->display_names[1];
        }
    }
}

void CustomiserMenu_Update(MENU_s *) {
    if (customiser_save_done != 0) {
        CustomiserMenu_End();
        customiser_save_done = 0;
        return;
    }
    CUSTOMISER *customiser = CharacterCustomiser;
    bool quit = customiser == NULL;
    i32 active_index, active_count;
    Customiser_GetActiveWeirdoIndex(&active_index, &active_count);
    BlipL[0] = BlipL[1] = 0.0f;
    BlipR[0] = BlipR[1] = 0.0f;
    BlipU[0] = BlipU[1] = 0.0f;
    BlipD[0] = BlipD[1] = 0.0f;
    i32 select[2];
    bool cancel = false;
    select[0] = 0;
    CustomiseNameBoardTMul[0] = CustomiseMode[0] == 1 ? 1.0f : 0.0f;
    CustomiseNameBoardMul[0] = SeekLinearF(CustomiseNameBoardMul[0], CustomiseNameBoardTMul[0], 4.0f * FRAMETIME);
    CustomiseNameLetterBlipScale[0] = SeekLinearF(CustomiseNameLetterBlipScale[0], 1.0f, 5.0f * FRAMETIME);
    if (MenuPacket.active_player[0] != 0) {
        if ((GamePad[0].buttons_pressed & GAMEPAD_MENUSELECT) != 0)
            select[0] = 1;
        else if ((GamePad[0].buttons_pressed & GAMEPAD_MENUCANCEL) != 0)
            cancel = true;
    }

    select[1] = 0;
    CustomiseNameBoardTMul[1] = CustomiseMode[1] == 1 ? 1.0f : 0.0f;
    CustomiseNameBoardMul[1] = SeekLinearF(CustomiseNameBoardMul[1], CustomiseNameBoardTMul[1], 4.0f * FRAMETIME);
    CustomiseNameLetterBlipScale[1] = SeekLinearF(CustomiseNameLetterBlipScale[1], 1.0f, 5.0f * FRAMETIME);
    if (MenuPacket.active_player[1] != 0) {
        if ((GamePad[1].buttons_pressed & GAMEPAD_MENUSELECT) != 0)
            select[1] = 1;
        else if ((GamePad[1].buttons_pressed & GAMEPAD_MENUCANCEL) != 0)
            cancel = true;
    }
    if (cancel && CustomiseMode[0] != 1 && CustomiseMode[1] != 1) {
        quit = true;
        if (GAMEDEMO != 0 && MenuPacket.active_player[0] != 0 && MenuPacket.active_player[1] != 0) {
            if ((GamePad[0].buttons_pressed & GAMEPAD_MENUCANCEL) != 0)
                MenuPacket.customise_demo_exit[0] = MenuPacket.customise_demo_exit[0] == 0;
            if ((GamePad[1].buttons_pressed & GAMEPAD_MENUCANCEL) != 0)
                MenuPacket.customise_demo_exit[1] = MenuPacket.customise_demo_exit[1] == 0;
            quit = MenuPacket.customise_demo_exit[0] != 0 && MenuPacket.customise_demo_exit[1] != 0;
        }
    } else if ((select[0] != 0 || select[1] != 0) && active_count == 1 && CustomiseMode[0] != 1 &&
               CustomiseMode[1] != 1) {
        if (!quit) {
            MenuPacket.customise_other_player[active_index] = MenuPacket.customise_other_player[active_index] == 0;
            GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            NewBuzzFrames(Player[active_index]->pad_gamepad->pad, 1, 0);
        }
    } else {
        bool toggle = false;
        bool cancel_name = false;
        quit = false;
        for (u32 player = 0; player != 2; ++player) {
            if (MenuPacket.active_player[player] == 0)
                continue;
            GAMEPAD_s *pad = &GamePad[player];
            const u32 pressed = pad->buttons_pressed | pad->left_directions;
            u32 side = player;
            if (active_count == 1 && MenuPacket.customise_other_player[player] != 0)
                side = player ^ 1;
            const i32 old_mode = CustomiseMode[side];
            i32 new_mode = -1;
            bool randomise = false, touch_exit = false;
            if ((pressed & GAMEPAD_ACTION) != 0 && old_mode != 1) {
                randomise = true;
            } else if ((pressed & GAMEPAD_SPECIAL) != 0 ||
                       (select[player] != 0 && (CustomiseMode[0] == 1 || CustomiseMode[1] == 1))) {
                if ((pressed & GAMEPAD_SPECIAL) != 0)
                    cancel_name = true;
                new_mode = old_mode;
                do {
                    new_mode = (new_mode + 1) % 3;
                } while (new_mode == 0);
                GameAudio_PlaySfx(0x30, NULL, 0, 0);
            } else {
                const f32 repeat_delay = old_mode == 1 ? 0.1f : 0.25f;
                const u32 held = pad->buttons_held | pad->buttons_released;
                const i32 category = CustomiseMenuY[side];
                const i32 piece_count = customiser->piece_counts[category];
                i16 *pieces = side == 0 ? Game.customizer.pieces : Game.customizer.secondary_pieces;
                const u16 old_piece = static_cast<u16>(pieces[category]);
                i32 up = pressed & GAMEPAD_DUP, down = pressed & GAMEPAD_DDOWN;
                i32 left = pressed & GAMEPAD_DLEFT, right = pressed & GAMEPAD_DRIGHT;
                i32 up_held = held & GAMEPAD_DUP, down_held = held & GAMEPAD_DDOWN;
                i32 left_held = held & GAMEPAD_DLEFT, right_held = held & GAMEPAD_DRIGHT;
                if (up != 0 && down != 0)
                    up = down = 0;
                if (left != 0 && right != 0)
                    left = right = 0;
                if (up_held != 0 && down_held != 0)
                    up_held = down_held = 0;
                if (left_held != 0 && right_held != 0)
                    left_held = right_held = 0;
                MenuRepeat(&up_held, &up, &uprepeattime[player], &uprepeatcount[player], 0.1f, FRAMETIME);
                MenuRepeat(&down_held, &down, &downrepeattime[player], &downrepeatcount[player], 0.1f, FRAMETIME);
                MenuRepeat(&left_held, &left, &leftrepeattime[player], &leftrepeatcount[player], repeat_delay,
                           FRAMETIME);
                MenuRepeat(&right_held, &right, &rightrepeattime[player], &rightrepeatcount[player], repeat_delay,
                           FRAMETIME);
                if (customiser->touch_requests[0] != 0) {
                    left = 1;
                    customiser->touch_requests[0] = 0;
                }
                if (customiser->touch_requests[1] != 0) {
                    right = 1;
                    customiser->touch_requests[1] = 0;
                }
                if (customiser->touch_requests[2] != 0) {
                    up = 1;
                    customiser->touch_requests[2] = 0;
                }
                if (customiser->touch_requests[3] != 0) {
                    down = 1;
                    customiser->touch_requests[3] = 0;
                }
                randomise = customiser->touch_requests[4] != 0;
                if (randomise)
                    customiser->touch_requests[4] = 0;
                if (customiser->touch_requests[5] != 0) {
                    toggle = true;
                    customiser->touch_requests[5] = 0;
                }
                if (customiser->field_0xd16 != 0) {
                    toggle = true;
                    customiser->field_0xd16 = 0;
                }
                touch_exit = customiser->field_0xd17 != 0;
                if (touch_exit)
                    customiser->field_0xd17 = 0;
                if (CustomiseMode[side] == 1) {
                    char *name = side == 0 ? Game.customizer.primary_name : Game.customizer.secondary_name;
                    const i32 old_x = CustomiseNameX[side], old_y = CustomiseNameY[side];
                    if (left != 0) {
                        if (old_x > 0) {
                            name[old_x] = CustomiseLetter[old_y];
                            CustomiseNameX[side] = old_x - 1;
                        }
                    } else if (right != 0) {
                        NuStrLen(name);
                        if (CustomiseNameX[side] < 14) {
                            name[CustomiseNameX[side]] = CustomiseLetter[CustomiseNameY[side]];
                            ++CustomiseNameX[side];
                        }
                    } else if (up != 0) {
                        CustomiseNameY[side] = old_y + 1 == 38 ? 0 : old_y + 1;
                    } else if (down != 0) {
                        CustomiseNameY[side] = old_y - 1 == -1 ? 37 : old_y - 1;
                    }
                    if (old_x != CustomiseNameX[side]) {
                        i32 letter = 0;
                        for (; letter != 38; ++letter)
                            if (name[CustomiseNameX[side]] == CustomiseLetter[letter])
                                break;
                        CustomiseNameY[side] = letter == 38 ? 0 : letter;
                    }
                    if (old_x != CustomiseNameX[side] || old_y != CustomiseNameY[side]) {
                        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                        CustomiseNameLetterBlipScale[side] = 1.5f;
                    }
                } else {
                    if (up_held != 0 && category > 0)
                        BlipU[player] = 1.0f;
                    else if (down_held != 0 && category < 8)
                        BlipD[player] = 1.0f;
                    if (left_held != 0)
                        BlipL[player] = 1.0f;
                    else if (right_held != 0)
                        BlipR[player] = 1.0f;
                    i32 next_category = category;
                    u32 next_piece = old_piece;
                    if (up != 0) {
                        if (category > 0)
                            --next_category;
                    } else if (down != 0) {
                        if (category < 8)
                            ++next_category;
                    } else if (left != 0)
                        next_piece = Customiser_NextPieceLeft(customiser, old_piece, piece_count, side, category);
                    else if (right != 0)
                        next_piece = Customiser_NextPieceRight(customiser, old_piece, piece_count, side, category);
                    pieces[category] = static_cast<i16>(next_piece);
                    CustomiseMenuY[side] = next_category;
                    if (next_category != category || next_piece != old_piece) {
                        GameAudio_PlaySfx(0x2f, NULL, 0, 0);
                        CustomiseMenuTime[side] = 0.0f;
                        if (next_piece != old_piece)
                            Customiser_SetNameAndIcon(customiser, side);
                    }
                }
            }
            if (new_mode != -1) {
                CustomiseMode[side] = new_mode;
                char *name = side == 0 ? Game.customizer.primary_name : Game.customizer.secondary_name;
                // The upper half of each 32-byte name field holds the edit backup.
                char *backup = name + 16;
                if (new_mode == 1) {
                    NuStrCpy(backup, name);
                    i32 length = NuStrLen(name);
                    if (length < 15)
                        name[length] = ' ';
                    i32 letter = 0;
                    for (; letter != 38; ++letter)
                        if (name[CustomiseNameX[side]] == CustomiseLetter[letter])
                            break;
                    CustomiseNameY[side] = letter == 38 ? 0 : letter;
                    CustomiseNameLetterBlipScale[side] = 1.0f;
                } else if (old_mode == 1) {
                    if (cancel_name)
                        NuStrCpy(name, backup);
                    else {
                        name[CustomiseNameX[side]] = CustomiseLetter[CustomiseNameY[side]];
                        i32 length = 15;
                        if (name[14] == ' ') {
                            i32 last = 13;
                            while (last >= 0 && name[last] == ' ')
                                --last;
                            if (last < 0) {
                                NuStrCpy(name, TTab[side == 0 ? tPLAYER1 : tPLAYER2]);
                                FinishWeirdoNames(side);
                                length = -1;
                            } else
                                length = last + 1;
                        }
                        if (length >= 0)
                            name[length] = 0;
                        if (NuStringFilterBadWords(reinterpret_cast<NUWCHAR8 *>(name),
                                                   reinterpret_cast<NUWCHAR8 *>(name),
                                                   reinterpret_cast<NUWCHAR8 *>(const_cast<char *>("Stranger"))) != 0)
                            NuStrCpy(name, backup);
                    }
                }
                if (side == 0)
                    Game.customizer.primary_name_unlocked = 1;
                else
                    Game.customizer.secondary_name_unlocked = 1;
            }
            if (randomise) {
                NUVEC *position = &CustomisePos[side];
                PlaySfx("ToggleChar", position);
                NewBuzzFrames(pad->pad, 1, 0);
                i16 *pieces = side == 0 ? Game.customizer.pieces : Game.customizer.secondary_pieces;
                for (i32 category = 0; category != 9; ++category) {
                    if (customiser->piece_counts[category] > 0) {
                        i32 piece;
                        do {
                            piece = qrand() / (0xffff / customiser->piece_counts[category] + 1);
                        } while (customiser->piece_available(&customiser->piece_sets[category][piece]) == 0);
                        pieces[category] = static_cast<i16>(piece);
                    }
                }
                Customiser_SetNameAndIcon(customiser, side);
                NUVEC debris_position = {0.0f, 0.21f, 0.0f};
                NUMTX_ALIGNED16 matrix;
                const i32 z = static_cast<i32>(NuTrigTable[CustomiseTiltZ[side] >> 1] * 910.0f);
                const f32 sz = NuTrigTable[(z >> 1) & 0x7fff], cz = NuTrigTable[((z + 0x4000) >> 1) & 0x7fff];
                matrix.m00 = cz;
                matrix.m01 = sz;
                matrix.m02 = matrix.m03 = 0.0f;
                matrix.m10 = -sz;
                matrix.m11 = cz;
                matrix.m12 = matrix.m13 = 0.0f;
                matrix.m20 = matrix.m21 = 0.0f;
                matrix.m22 = 1.0f;
                matrix.m23 = 0.0f;
                matrix.m30 = matrix.m31 = matrix.m32 = 0.0f;
                matrix.m33 = 1.0f;
                const i32 x = static_cast<i32>(910.0f * NuTrigTable[CustomiseTiltX[side] >> 1]);
                const f32 sx = NuTrigTable[(x >> 1) & 0x7fff], cx = NuTrigTable[((x + 0x4000) >> 1) & 0x7fff];
#define CUSTOMISE_ROTATE_X(row)                                                                                        \
    {                                                                                                                  \
        const f32 first = matrix.m##row##1, second = matrix.m##row##2;                                                 \
        matrix.m##row##1 = first * cx - second * sx;                                                                   \
        matrix.m##row##2 = first * sx + second * cx;                                                                   \
    }
                CUSTOMISE_ROTATE_X(0);
                CUSTOMISE_ROTATE_X(1);
                CUSTOMISE_ROTATE_X(2);
                CUSTOMISE_ROTATE_X(3);
#undef CUSTOMISE_ROTATE_X
                const i32 y = static_cast<i32>(10922.0f * NuTrigTable[CustomiseRotY[side] >> 1]);
                const f32 sy = NuTrigTable[(y >> 1) & 0x7fff], cy = NuTrigTable[((y + 0x4000) >> 1) & 0x7fff];
#define CUSTOMISE_ROTATE_Y(row)                                                                                        \
    {                                                                                                                  \
        const f32 first = matrix.m##row##0, second = matrix.m##row##2;                                                 \
        matrix.m##row##0 = first * cy + second * sy;                                                                   \
        matrix.m##row##2 = second * cy - first * sy;                                                                   \
    }
                CUSTOMISE_ROTATE_Y(0);
                CUSTOMISE_ROTATE_Y(1);
                CUSTOMISE_ROTATE_Y(2);
                CUSTOMISE_ROTATE_Y(3);
#undef CUSTOMISE_ROTATE_Y
                NuVecMtxRotate(&debris_position, &debris_position, &matrix);
                NuVecAdd(&debris_position, &debris_position, position);
                AddGameDebris(WORLD->debris_sys, 0x7b, &debris_position);
                AddGameDebris(WORLD->debris_sys, 0x5c, &debris_position);
            }
            if (touch_exit) {
                quit = true;
                break;
            }
        }
        if (!quit && toggle) {
            MenuPacket.customise_other_player[active_index] = MenuPacket.customise_other_player[active_index] == 0;
            GameCam_Blend(GameCam, 1.0f, 0.0f, 1);
            NewBuzzFrames(Player[active_index]->pad_gamepad->pad, 1, 0);
        }
    }
    // The reference compares both complete 0x38-byte save records, including
    // the final byte currently represented by GAMESAVE_s::field_0x7c9f.
    const u8 *current_save = reinterpret_cast<const u8 *>(&Game) + offsetof(GAMESAVE_s, customizer);
    const u8 *old_save = reinterpret_cast<const u8 *>(&OldCustomiseGame) + offsetof(GAMESAVE_s, customizer);
    customiser_changed = memcmp(current_save, old_save, 0x70) != 0 ||
                         memcmp(Game.customizer.primary_name, OldCustomiseGame.customizer.primary_name, 0x20) != 0 ||
                         Game.customizer.primary_name_unlocked != OldCustomiseGame.customizer.primary_name_unlocked ||
                         Game.customizer.secondary_name_unlocked != OldCustomiseGame.customizer.secondary_name_unlocked;
    if (!quit)
        return;
    PlaySfx("menuBack", NULL);
    customiser_quit = 1;
    if (GAMEDEMO == 0 && customiser_changed != 0) {
        Hub_ClearStats();
        Text_FillInExtendedSaveInfo();
        if (TriggerAutoSave() == 0) {
            customiser_save_done = 1;
            NewMenu(1000, -1, -1);
            return;
        }
    }
    CustomiserMenu_End();
}

void Customiser_PieceConfig(CUSTOMPIECE *piece, nufpar_s *parser) {
    for (CUSTOMISER_GAMESETTING *setting = Customiser_GameSetting; setting->name != NULL; ++setting) {
        if (NuStrICmp(parser->word_buf, setting->name) == 0) {
            piece->model_flags |= setting->model_flags;
            piece->gameplay_flags |= setting->gameplay_flags;
            break;
        }
    }
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

void Customiser_SetNameAndIcon(CUSTOMISER *customiser, i32 index) {
    if (customiser == NULL)
        return;
    bool single = index != -1;
    if (index == 0 || !single) {
        customiser->display_names[0][0] = 0;
        sprintf(customiser->display_names[0], "%s %i", TTab[tSTRANGER], 1);
        CDataList[customiser->character_ids[0]].field20_0x42 = Customiser_GetIcon(customiser, &Game.customizer, 0);
    }
    if (index == 1 || !single) {
        customiser->display_names[1][0] = 0;
        sprintf(customiser->display_names[1], "%s %i", TTab[tSTRANGER], 2);
        CDataList[customiser->character_ids[1]].field20_0x42 =
            Customiser_GetIcon(customiser, reinterpret_cast<CUSTOMISESAVE_s *>(Game.customizer.secondary_pieces), 1);
    }
}

void Customiser_GetActiveWeirdoIndex(i32 *index, i32 *count) {
    if (MenuPacket.active_player[0] != 0) {
        if (MenuPacket.active_player[1] == 0) {
            *index = 0;
            *count = 1;
            return;
        }
    } else if (MenuPacket.active_player[1] != 0) {
        *index = 1;
        *count = 1;
        return;
    }
    *index = 0;
    *count = 2;
}

i32 Customise_GetToggleString(i32 index) {
    i32 mode = CustomiseMode[index];
    if (mode == 1) {
        return tCANCEL;
    }
    if (mode != 0) {
        do {
            mode = (i32)((u32)mode + 1u) % 3;
        } while (mode == 0);
    }
    return tEDITNAME;
}
