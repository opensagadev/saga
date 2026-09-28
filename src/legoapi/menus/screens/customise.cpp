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
#include "legoapi/menus/core/panel.h"
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
#include "legoapi/render/core/render.h"
#include "legoapi/render/fx.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/render/light/lighting.h"
#include "legoapi/world/area.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/nufile/nufpar.h"

f32 CustomiseMenuTime[2];
GAMESAVE_s OldCustomiseGame = {};
i32 customiser_save_done = 0;
i32 customiser_quit = 0;
i32 customiser_changed = 0;

struct CUSTOMISEMOTION_s {
    f32 field_00, field_04, delay;
    i16 angles[3];
    u8 reserved_12[2];
};
DECOMP_ASSERT(sizeof(CUSTOMISEMOTION_s) == 0x14, "Customise motion size");
static i32 CustomiseMode[2];
static u16 CustomiseYRot[2];
static CUSTOMISEMOTION_s HeadAnim[2], ArmsAnim[2], LegsAnim[2];
static u16 CustomiseBob[2];
static f32 Customise_NameAlpha;
static f32 CustomiseNameBoardTMul[2], CustomiseNameBoardMul[2], CustomiseNameLetterBlipScale[2];
static NUVEC CustomiseScreenPos[2];
u16 CustomiseRotY[2], CustomiseTiltX[2], CustomiseTiltZ[2];
void Customiser_SetNameAndIcon(CUSTOMISER *, i32);

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

void Customiser_SetUpCharacterData(CUSTOMISER *customiser) {
    if (customiser == NULL) {
        return;
    }

    for (i32 side = 0; side < 2; ++side) {
        const i32 character_id = customiser->character_ids[side];
        if (character_id < 0 || character_id >= CHARCOUNT) {
            continue;
        }
        CHARACTERDATA *character = &CDataList[character_id];
        GAMECHARACTERDATA *runtime = character->game_character;
        if (runtime == NULL) {
            continue;
        }

        const i16 *selection = Customiser_GetSelection(side);
        const u32 layer_mask = Customiser_GetLayerMask(customiser, runtime, selection);
        runtime->layer_mask_special = layer_mask;
        runtime->layer_mask = layer_mask;
        runtime->layer_mask_medium = layer_mask;
        runtime->layer_mask_low = layer_mask;
        runtime->layer_mask_dead = layer_mask;

        character->model_flags &= 3;
        runtime->flags_090 = 0;
        CUSTOMPIECE *torso = Customiser_GetSelectedPiece(customiser, selection, 1);
        const bool torso_replaces_base = torso != NULL && (torso->layer_flags & 1) != 0;
        for (i32 category = 0; category < 9; ++category) {
            CUSTOMPIECE *piece = Customiser_GetSelectedPiece(customiser, selection, category);
            if (piece == NULL || (category == 0 && torso_replaces_base)) {
                continue;
            }
            character->model_flags |= piece->model_flags;
            if ((character->model_flags & CHARACTER_MODEL_FLAG_ALTERNATE_WEAPON) != 0) {
                character->model_flags |= 0x10000000;
            }
            runtime->flags_090 |= piece->gameplay_flags;
        }
        if (torso_replaces_base) {
            runtime->flags_090 |= 0x10;
        }

        CUSTOMPIECE *weapon = Customiser_GetSelectedPiece(customiser, selection, 2);
        if (weapon != NULL) {
            runtime->weapon_model = weapon->weapon_model;
            runtime->field_0x117 = static_cast<u8>(LightSabre_ColourFromObj(runtime->weapon_model, NULL));
        }
    }
    Customiser_SetNameAndIcon(customiser, -1);
}

void Customiser_Draw3D(CUSTOMISER *customiser) {
    WORLDINFO_s *world = WorldInfo_CurrentlyActive();
    if (customiser == NULL || world == NULL || world->camera_splines == NULL || world->camera_splines[18] == NULL) {
        return;
    }

    for (i32 side = 0; side < 2; ++side) {
        CHARACTERMODEL_s *model = APICharacterLoaded(customiser->character_ids[side]);
        if (model == NULL || customiser->animation_state[side] != 0) {
            continue;
        }
        GAMECHARACTERDATA *runtime = CDataList[customiser->character_ids[side]].game_character;
        if (runtime == NULL) {
            continue;
        }

        rtldata_s light_data;
        rtlResetEx(&light_data, 1);
        rtlApplySetScale(world->rtl_set, &light_data, &CustomisePos[side], NULL, -1, 1.0f);
        SetLights_RTLDATA(&light_data, 1.0f);

        NUMTX matrix;
        NuMtxSetRotationY(&matrix, CustomiseYRot[side] + 0x8000);
        NuMtxTranslate(&matrix, &CustomisePos[side]);
        const i16 *selection = Customiser_GetSelection(side);
        const u32 layer_mask = Customiser_GetLayerMask(customiser, runtime, selection);
        if (GameDrawCharacterModel(model, &customiser->animation_packets[side], &matrix, NULL, NULL,
                                   customiser->joint_matrices[side], NULL, layer_mask) == 0) {
            continue;
        }

        const i32 locator = runtime->helmet_locator;
        if (locator < 0 || locator >= 16 || model->points_of_interest[locator] == NULL) {
            continue;
        }
        for (i32 category = 0; category < 9; ++category) {
            if (customiser->categories[category] == NULL || customiser->categories[category]->uses_special == 0 ||
                world->customiser_resources[category] == NULL) {
                continue;
            }
            i32 piece_index = selection[category];
            if (piece_index < 0 || piece_index >= customiser->piece_counts[category]) {
                continue;
            }
            CUSTOMPIECERESOURCE *resource = &world->customiser_resources[category][piece_index];
            if (NuSpecialExistsFn(&resource->special) != 0) {
                NuSpecialDrawAt(&resource->special, &customiser->joint_matrices[side][locator]);
            }
        }
    }
    SetLevelLights(world->rtl_set, 1.0f);
}

void Customiser_Update(CUSTOMISER *customiser, WORLDINFO_s *world) {
    if (customiser == NULL) {
        return;
    }

    for (i32 side = 0; side < 2; ++side) {
        CustomiseRotY[side] += static_cast<u16>(4096.0f * FRAMETIME);
        CustomiseTiltX[side] += static_cast<u16>(2048.0f * FRAMETIME);
        CustomiseTiltZ[side] += static_cast<u16>(3072.0f * FRAMETIME);
        CustomiseBob[side] += static_cast<u16>(4096.0f * FRAMETIME);
        CustomiseMenuTime[side] += FRAMETIME;

        CHARACTERMODEL_s *model = APICharacterLoaded(customiser->character_ids[side]);
        if (model == NULL) {
            continue;
        }
        const i16 *selection = Customiser_GetSelection(side);
        CUSTOMPIECE *weapon = Customiser_GetSelectedPiece(customiser, selection, 2);
        const i32 saber_colour =
            weapon == NULL ? -1 : LightSabre_ColourFromObj(static_cast<i32>(weapon->weapon_model), NULL);

        if (customiser->animation_active[side] != 0) {
            --customiser->animation_active[side];
        }
        ANIMPACKET_s *packet = &customiser->animation_packets[side];
        if (customiser->animation_state[side] == 0) {
            const i16 intro_animation = saber_colour == -1 ? 0x62 : 0x61;
            if (customiser->animation_values[side] == 0.0f) {
                customiser->animation_values[side] =
                    AnimDuration(customiser->character_ids[side], intro_animation, 0.0f, 0.0f, 1);
            } else {
                customiser->animation_values[side] -= FRAMETIME;
                if (customiser->animation_values[side] <= 0.0f) {
                    customiser->animation_state[side] = 1;
                }
            }
            packet->previous_animation = packet->animation_index;
            packet->requested_animation = customiser->animation_state[side] == 0
                                              ? intro_animation
                                              : static_cast<i16>(saber_colour == -1 ? 99 : 0xbe);
        } else {
            customiser->animation_state[side] = 1;
            packet->previous_animation = packet->animation_index;
            packet->requested_animation = saber_colour == -1 ? 99 : 0xbe;
        }
        UpdateAnimPacket(model, packet, FRAMETIME, 0.0f, FRAMETIME, 0.0f);

        if (world == NULL || model->hierarchy == NULL) {
            continue;
        }
        for (i32 category = 0; category < 9; ++category) {
            CUSTOMPIECECATEGORY *category_data = customiser->categories[category];
            CUSTOMPIECERESOURCE *resources = world->customiser_resources[category];
            if (category_data == NULL || category_data->uses_special != 0 || resources == NULL) {
                continue;
            }
            i32 piece_index = selection[category];
            if (piece_index < 0 || piece_index >= customiser->piece_counts[category]) {
                continue;
            }
            const i32 texture_id = resources[piece_index].texture_id;
            for (i32 material = 0; material < model->hierarchy->material_count; ++material) {
                NUMTL *entry = model->hierarchy->materials[material];
                if (entry != NULL && entry->unknown_9a[0] == static_cast<u8>(category_data->material_tag)) {
                    entry->tex_id = texture_id;
                    NuMtlUpdate(entry);
                }
            }
        }
    }
    Customise_NameAlpha = SeekLinearF(Customise_NameAlpha, customiser_quit == 0 ? 1.0f : 0.0f, 2.0f * FRAMETIME);
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

extern char *ASCII_UP, *ASCII_DOWN;
extern f32 ICONX, ICONSIZE, DROPINALPHA;
extern i32 menu_flash;
static i32 CustomiseNameX[2], CustomiseNameY[2];
static i32 CustomiseMenuY[2];
void Customiser_GetActiveWeirdoIndex(i32 *, i32 *);
i32 Customise_GetToggleString(i32);
extern f32 BlipL[2], BlipR[2], BlipU[2], BlipD[2];
extern "C" i32 TriggerAutoSave(void);
void Hub_ClearStats();
void Text_FillInExtendedSaveInfo();
void FinishWeirdoNames(i32);
void GameAudio_PlaySfxById(i32, NUVEC *, i32, i32);

void CustomiserMenu_Draw(MENU_s *) {
    CUSTOMISER *customiser = CharacterCustomiser;
    if (MenuStopDraw != 0 || customiser_quit != 0 || customiser == NULL)
        return;

    i32 side = 0;
    i32 count = 0;
    Customiser_GetActiveWeirdoIndex(&side, &count);
    if (count == 1 && MenuPacket.active_player[side] != 0)
        side = side == 0 ? 1 : 0;

    if (Customise_NameAlpha > 0.0f && count > 0) {
        for (i32 item = 0; item < count; ++item, ++side) {
            if (side >= 2)
                break;
            u8 dim = side == 0 ? 0xff : 0x7f;
            f32 x = CustomiseScreenPos[side].x;
            if (CustomiseMode[side] == 1) {
                char name[2] = {0, 0};
                const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ- 0123456789";
                char *saved_name = reinterpret_cast<char *>(&Game) + 0x7c44 + side * 0x38;
                i32 end = 14;
                while (end > 0 && saved_name[end] == ' ')
                    --end;
                f32 step = 0.08f;
                f32 letter_x = x - 0.6f;
                for (i32 letter = 0; letter < 15; ++letter) {
                    if (letter == CustomiseNameX[side]) {
                        name[0] = alphabet[CustomiseNameY[side] % 38];
                        u8 arrow = menu_flash == 0 ? 0xff : 0xbf;
                        Text3D(ASCII_UP, letter_x, -0.75f, 1.0f, 0.6f, 0.6f, 0.6f, 0, arrow, arrow, arrow);
                        Text3D(ASCII_DOWN, letter_x, -0.95f, 1.0f, 0.6f, 0.6f, 0.6f, 0, arrow, arrow, arrow);
                    } else {
                        name[0] = saved_name[letter];
                        if (letter > CustomiseNameX[side] && letter >= end && name[0] == ' ')
                            name[0] = '.';
                    }
                    Text3D(name, letter_x, -0.85f, 1.0f, 0.6f, 0.6f, 0.5f, 0, dim,
                           letter == CustomiseNameX[side] && menu_flash == 0 ? 0xff : dim, dim);
                    letter_x += step;
                }
            } else {
                char name[64];
                GameObj_GetName(customiser->character_ids[side], NULL, name);
                SmartTextEx(name, x, -0.85f, 1.0f, 0.5f, 0.5f, 0.5f, 0, dim, 0xff, side == 0 ? 0xff : 0, 0.75f, 2, NULL,
                            0, static_cast<i32>(255.0f * Customise_NameAlpha));
            }

            f32 scale = ICONSIZE * 0.75f;
            f32 alpha = (1.0f - CustomiseNameBoardMul[side]) * Customise_NameAlpha;
            DrawPanel3DObject(x, -0.6f, 1.0f, scale, scale, scale, 0, 0, 0, &WORLD->lev_objs[0xa7].special, 0, alpha);
            CUSTOMISESAVE_s *save =
                side == 0 ? &Game.customizer : reinterpret_cast<CUSTOMISESAVE_s *>(Game.customizer.secondary_pieces);
            i32 icon = Customiser_GetIcon(customiser, save, item);
            if (icon != -1)
                DrawPanel3DObject(x, -0.6f, 1.0f, scale, scale, scale, 0, 0, 0,
                                  &WORLD->lev_objs[icon + (icon != LEGOOBJ_ICON_WEIRDO)].special, 0, alpha);
            f32 *screen = reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(customiser) + 0xcc8 + item * 4);
            *screen = x;
            reinterpret_cast<f32 *>(reinterpret_cast<u8 *>(customiser) + 0xccc)[item * 3] = -0.6f;
            Text3D(const_cast<char *>(side == 0 ? ">" : "<"), side == 0 ? x + 0.5f : x - 0.5f, -0.6f, 1.0f, 1.0f, 1.0f,
                   1.0f, 0, dim, dim, dim);
        }
    }
    f32 icon_alpha = MenuPacket.active_player[0] != 0 ? 1.0f : DROPINALPHA;
    DrawCharIcon(MenuPacket.player_model[0], -ICONX, STATSPOSY, 0.0f, ICONSIZE, 0xa6, icon_alpha, icon_alpha, 1, NULL);
    if (GAMEDEMO == 0)
        Customise_GetToggleString(count == 1 && MenuPacket.active_player[1] != 0);
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
    if (customiser == NULL)
        return;
    i32 active_side = 0;
    i32 count = 0;
    Customiser_GetActiveWeirdoIndex(&active_side, &count);
    for (i32 side = 0; side < 2; ++side) {
        BlipL[side] = BlipR[side] = BlipU[side] = BlipD[side] = 0.0f;
        CustomiseNameBoardTMul[side] = CustomiseMode[side] == 1 ? 1.0f : 0.0f;
        CustomiseNameBoardMul[side] =
            SeekLinearF(CustomiseNameBoardMul[side], CustomiseNameBoardTMul[side], 2.0f * FRAMETIME);
        CustomiseNameLetterBlipScale[side] = SeekLinearF(CustomiseNameLetterBlipScale[side], 1.0f, 4.0f * FRAMETIME);
    }

    bool quit = false;
    bool toggle_active = false;
    const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ- 0123456789";
    for (i32 input = 0; input < 2; ++input) {
        if (MenuPacket.active_player[input] == 0)
            continue;
        GAMEPAD_s *pad = &GamePad[input];
        u32 pressed = pad->buttons_pressed;
        i32 side = count == 1 && MenuPacket.reserved_0[input] != 0 ? input ^ 1 : input;
        if (side < 0 || side > 1)
            continue;
        i32 mode = CustomiseMode[side];
        bool select = (pressed & (GAMEPAD_ACTION | GAMEPAD_MENUSELECT)) != 0;
        bool cancel = (pressed & GAMEPAD_MENUCANCEL) != 0;
        if (cancel && mode != 1) {
            quit = true;
            continue;
        }
        if (select || (pressed & GAMEPAD_SPECIAL) != 0) {
            if ((pressed & GAMEPAD_SPECIAL) != 0) {
                u16 *pieces =
                    reinterpret_cast<u16 *>(side == 0 ? Game.customizer.pieces : Game.customizer.secondary_pieces);
                for (i32 category = 0; category < 9; ++category) {
                    if (customiser->piece_counts[category] <= 0)
                        continue;
                    i32 piece;
                    do {
                        piece = qrand() % customiser->piece_counts[category];
                    } while (customiser->piece_available != NULL &&
                             customiser->piece_available(&customiser->piece_sets[category][piece]) == 0);
                    pieces[category] = piece;
                }
                Customiser_SetNameAndIcon(customiser, side);
                PlaySfx("ToggleChar", &CustomisePos[side]);
                AddGameDebris(WORLD->debris_sys, 0x7b, &CustomisePos[side]);
                AddGameDebris(WORLD->debris_sys, 0x5c, &CustomisePos[side]);
                continue;
            }
            i32 next_mode = mode == 1 ? 2 : 1;
            CustomiseMode[side] = next_mode;
            char *name = reinterpret_cast<char *>(&Game) + 0x7c44 + side * 0x38;
            char *backup = name + 0x10;
            if (next_mode == 1) {
                NuStrCpy(backup, name);
                i32 length = NuStrLen(name);
                if (length < 15)
                    name[length] = ' ';
                CustomiseNameX[side] = 0;
                CustomiseNameY[side] = 0;
            } else if (cancel) {
                NuStrCpy(name, backup);
            } else {
                name[CustomiseNameX[side]] = alphabet[CustomiseNameY[side]];
                i32 length = 15;
                while (length > 0 && name[length - 1] == ' ')
                    --length;
                name[length] = 0;
                if (length == 0)
                    FinishWeirdoNames(side);
            }
            GameAudio_PlaySfxById(0x30, NULL, 0, 0);
            continue;
        }
        i32 up = (pressed & GAMEPAD_DUP) != 0;
        i32 down = (pressed & GAMEPAD_DDOWN) != 0;
        i32 left = (pressed & GAMEPAD_DLEFT) != 0;
        i32 right = (pressed & GAMEPAD_DRIGHT) != 0;
        if (left && right)
            left = right = 0;
        if (up && down)
            up = down = 0;
        if (mode == 1) {
            char *name = reinterpret_cast<char *>(&Game) + 0x7c44 + side * 0x38;
            if (left && CustomiseNameX[side] > 0) {
                name[CustomiseNameX[side]] = alphabet[CustomiseNameY[side]];
                --CustomiseNameX[side];
            } else if (right && CustomiseNameX[side] < 14) {
                name[CustomiseNameX[side]] = alphabet[CustomiseNameY[side]];
                ++CustomiseNameX[side];
            }
            if (up)
                CustomiseNameY[side] = (CustomiseNameY[side] + 1) % 38;
            else if (down)
                CustomiseNameY[side] = (CustomiseNameY[side] + 37) % 38;
            if (left || right || up || down) {
                i32 letter = 0;
                while (letter < 38 && alphabet[letter] != name[CustomiseNameX[side]])
                    ++letter;
                if (left || right)
                    CustomiseNameY[side] = letter < 38 ? letter : 0;
                GameAudio_PlaySfxById(0x2f, NULL, 0, 0);
                CustomiseNameLetterBlipScale[side] = 1.5f;
            }
            continue;
        }
        i32 &category = CustomiseMenuY[side];
        if (up && category > 0) {
            --category;
            BlipU[input] = 1.0f;
        } else if (down && category < 8) {
            ++category;
            BlipD[input] = 1.0f;
        }
        i16 *pieces = side == 0 ? Game.customizer.pieces : Game.customizer.secondary_pieces;
        if (left && customiser->piece_counts[category] > 0) {
            pieces[category] = Customiser_NextPieceLeft(customiser, pieces[category],
                                                        customiser->piece_counts[category], side, category);
            BlipL[input] = 1.0f;
        } else if (right && customiser->piece_counts[category] > 0) {
            pieces[category] = Customiser_NextPieceRight(customiser, pieces[category],
                                                         customiser->piece_counts[category], side, category);
            BlipR[input] = 1.0f;
        }
        if (left || right) {
            CustomiseMenuTime[side] = 0.0f;
            Customiser_SetNameAndIcon(customiser, side);
        }
    }

    if (count == 1 && (GamePad[active_side].buttons_pressed & GAMEPAD_MENUSELECT) != 0)
        toggle_active = true;
    if (toggle_active)
        MenuPacket.reserved_0[active_side] ^= 1;
    customiser_changed = memcmp(&Game.customizer, &OldCustomiseGame.customizer, sizeof(Game.customizer)) != 0;
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
