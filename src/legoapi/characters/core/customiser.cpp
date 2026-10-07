#include "legoapi/characters/core/customiser.h"

#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "batman.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/misc/utilities.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nuplatform/nuplatform.h"
#include <string.h>

DECOMP_ASSERT(sizeof(CUSTOMPIECERESOURCE) == 0x20, "CUSTOMPIECERESOURCE size");
DECOMP_ASSERT(offsetof(CUSTOMPIECERESOURCE, texture_id) == 0x14, "Customiser texture offset");
DECOMP_ASSERT(offsetof(CUSTOMPIECERESOURCE, character_model) == 0x1c, "Customiser model offset");
static CUSTOMPIECERESOURCE Accessory[2][9];

static CUSTOMPIECECATEGORY CustomSetData[9] = {
    {"hat_hair", 1, -1, {1, 0}, "chars\\weirdo\\all\\hat_hair_all.gsc"},
    {"head", 1, -1, {0, 0}, "chars\\weirdo\\all\\head_all.gsc"},
    {NULL, 0, -1, {0, 0}, NULL},
    {"arm", 0, 2, {1, 0}, NULL},
    {"hand", 0, 4, {0, 0}, NULL},
    {"cape", 0, 3, {2, 0}, NULL},
    {"body", 0, 1, {3, 0}, NULL},
    {"under_pants", 0, 6, {1, 0}, NULL},
    {"leg", 0, 5, {2, 0}, NULL},
};

static inline i32 Customiser_FindSetFromName(char *name) {
    if (NuStrICmp(name, "hathair") == 0)
        return 0;
    if (NuStrICmp(name, "head") == 0)
        return 1;
    if (NuStrICmp(name, "cape") == 0)
        return 5;
    if (NuStrICmp(name, "body") == 0)
        return 6;
    if (NuStrICmp(name, "arms") == 0)
        return 3;
    if (NuStrICmp(name, "hands") == 0)
        return 4;
    if (NuStrICmp(name, "weapon") == 0)
        return 2;
    if (NuStrICmp(name, "underpants") == 0)
        return 7;
    if (NuStrICmp(name, "legs") == 0)
        return 8;
    return -1;
}

static i32 Customiser_PieceAvailable_Default(CUSTOMPIECE *piece) {
    if ((piece->availability_flags & 0x180) != 0 && Game_100PercentComplete() == 0) {
        return 0;
    }
    return 1;
}

void Customiser_SetAnimsToLoad(CUSTOMISER *customiser, i32 enabled) {
    if (customiser == NULL) {
        return;
    }

    for (i32 character = 0; character < 2; ++character) {
        CHARACTERANIM_s *animation = CDataList[customiser->character_ids[character]].animations;
        if (animation == NULL) {
            continue;
        }
        for (; animation->name != NULL; ++animation) {
            if (enabled == 0) {
                animation->flags &= ~0x8000u;
                continue;
            }

            animation->flags |= 0x8000u;
            const u16 *animation_ids = customiser->animation_ids_to_load;
            if (animation_ids == NULL) {
                continue;
            }
            for (; *animation_ids != 0xffff; ++animation_ids) {
                if (*animation_ids == static_cast<u16>(animation->animation_id)) {
                    animation->flags &= ~0x8000u;
                    break;
                }
            }
        }
    }
}

i32 Customiser_NextPieceLeft(CUSTOMISER *customiser, i32 index, i32 count, i32 unused, i32 category) {
    i32 found = 0;
    i32 attempts = 0;
    if (customiser) {
        while (attempts < count && !found) {
            --index;
            if (index < 0)
                index += count;
            i32 available = 1;
            if (category != 2) {
                WORLDINFO_s *world = WorldInfo_CurrentlyActive();
                if (world && HUB_ADATA && world->area == HUB_ADATA) {
                    CUSTOMPIECERESOURCE *resources = world->customiser_resources[category];
                    if (resources) {
                        CUSTOMPIECERESOURCE *resource = &resources[index];
                        if (customiser->categories[category]->uses_special)
                            available = NuSpecialExistsFn(&resource->special) != 0;
                        else
                            available = resource->model != NULL;
                    } else
                        available = 0;
                }
            }
            if (available) {
                if (!(customiser->piece_sets[category][index].availability_flags & 0x180) ||
                    Game_100PercentComplete()) {
                    if (customiser->piece_available(&customiser->piece_sets[category][index]))
                        found = 1;
                    else
                        ++attempts;
                } else
                    ++attempts;
            } else
                ++attempts;
        }
    }
    return index;
}

i32 Customiser_NextPieceRight(CUSTOMISER *customiser, i32 index, i32 count, i32 unused, i32 category) {
    i32 found = 0;
    i32 attempts = 0;
    if (customiser) {
        while (attempts < count && !found) {
            ++index;
            if (index >= count)
                index -= count;
            i32 available = 1;
            if (category != 2) {
                WORLDINFO_s *world = WorldInfo_CurrentlyActive();
                if (world && HUB_ADATA && world->area == HUB_ADATA) {
                    CUSTOMPIECERESOURCE *resources = world->customiser_resources[category];
                    if (resources) {
                        CUSTOMPIECERESOURCE *resource = &resources[index];
                        if (customiser->categories[category]->uses_special)
                            available = NuSpecialExistsFn(&resource->special) != 0;
                        else
                            available = resource->model != NULL;
                    } else
                        available = 0;
                }
            }
            if (available) {
                if (!(customiser->piece_sets[category][index].availability_flags & 0x180) ||
                    Game_100PercentComplete()) {
                    if (customiser->piece_available(&customiser->piece_sets[category][index]))
                        found = 1;
                    else
                        ++attempts;
                } else
                    ++attempts;
            } else
                ++attempts;
        }
    }
    return index;
}

void Customiser_LoadAccessories(CUSTOMISER *customiser, APICHARACTERMODELLIST_s *models) {
    if (customiser == NULL) {
        return;
    }

    Customiser_AccessoriesLoaded = 1;
    for (; models->model_id != -1; ++models) {
        i32 side = 0;
        if (models->model_id != customiser->character_ids[0]) {
            if (models->model_id != customiser->character_ids[1]) {
                continue;
            }
            side = 1;
        }

        const i16 slot = apicharsys->playermodelids[models->model_id];
        CHARACTERMODEL_s *character_model = &apicharsys->models[slot];
        for (i32 category_index = 0; category_index < 9; ++category_index) {
            CUSTOMPIECERESOURCE *resource = &Accessory[side][category_index];
            memset(resource, 0, sizeof(*resource));

            CUSTOMPIECECATEGORY *category = customiser->categories[category_index];
            if (category->name == NULL || customiser->piece_counts[category_index] <= 0) {
                continue;
            }

            resource->character_model = character_model;
            const u16 piece_index = customiser->save[side].pieces[category_index];
            CUSTOMPIECE *piece = &customiser->piece_sets[category_index][piece_index];
            char piece_name[0x80];
            char path[0x80];
            NuStrCpy(piece_name, piece->name);
            NuStrCpy(path, "chars\\weirdo\\");
            NuStrCat(path, category->name);
            NuStrCat(path, "\\");
            NuStrCat(path, piece_name);

            if (category->uses_special == 0) {
                if (category->material_tag == -1) {
                    continue;
                }
                NuStrCat(path, ".pnt");
                resource->texture_id = NuTexRead(path, &characterbuffer_ptr, characterbuffer_end);
                if (resource->texture_id == 0) {
                    continue;
                }

                nuhgobj_s *hierarchy = character_model->hierarchy;
                for (i32 material_index = 0; material_index < hierarchy->material_count; ++material_index) {
                    NUMTL *material = hierarchy->materials[material_index];
                    if (material->unknown_9a[0] != category->material_tag) {
                        continue;
                    }
                    resource->material_index = material_index;
                    resource->original_texture_id = material->tex_id;
                    material->tex_id = static_cast<i16>(resource->texture_id);
                    NuMtlUpdate(material);
                }
            } else {
                NuStrCat(path, ".gsc");
                resource->scene = NuGScnRead(&characterbuffer_ptr, characterbuffer_end, path);
                if (resource->scene != NULL) {
                    NuSpecialFind(resource->scene, &resource->special, piece_name, 1);
                }
            }
        }
    }
}

void Customiser_DrawAccessories(CUSTOMISER *customiser, GameObject_s *object, numtx_s *matrices) {
    const i32 joint = object->apiobj.character_data->player_config->helmet_locator;
    if (customiser == NULL || joint == -1 || object->apiobj.character_model->points_of_interest[joint] == NULL)
        return;

    const i32 side = object->id != customiser->character_ids[0];
    i32 next_category = 0;
    CUSTOMPIECERESOURCE *next_resource = Accessory[side];
    do {
        CUSTOMPIECERESOURCE *resource = next_resource++;
        const i32 category = next_category++;
        if (customiser->piece_counts[category] <= 0 || category == 2)
            continue;
        if (category == 0) {
            const u16 *pieces = customiser->save[side].pieces;
            if ((customiser->piece_sets[0][static_cast<u16>(pieces[0])].layer_flags & 0x20) != 0 ||
                (customiser->piece_sets[1][static_cast<u16>(pieces[1])].layer_flags & 1) != 0 ||
                object->field_0x108e != 0)
                continue;
        }

        nuhspecial_s *special = &resource->special;
        if (!NuSpecialExistsFn(special))
            continue;

        // Retail keeps both draw matrices on a 16-byte-aligned stack.
        NUMTX_ALIGNED16 matrix;
        NUMTX_ALIGNED16 reflection;
        if (matrices != NULL)
            matrix = matrices[joint];
        else
            matrix = object->joint_matrices[joint];
        NuSpecialDrawAt(special, &matrix);
        if (object->field_0x1088 != 0 && MatrixReflection(&matrix, object->field_0x1087, object->field_0x1020,
                                                          WORLD->current_level->unknown_0cc, &reflection)) {
            NuRndrStartReflectionRender(0);
            NuSpecialDrawAt(special, &reflection);
            NuRndrEndReflectionRender();
        }
    } while (next_category != 9);
}

void Customiser_AddPartAccessories(CUSTOMISER *customiser, GameObject_s *object, i32 animation, i32 mode, float scale) {
    const i32 joint = object->apiobj.character_data->player_config->helmet_locator;
    if (customiser == NULL || joint == -1 || object->apiobj.character_model->points_of_interest[joint] == NULL)
        return;

    const i32 side = object->id != customiser->character_ids[0];
    i32 next_category = 0;
    CUSTOMPIECERESOURCE *next_resource = Accessory[side];
    do {
        CUSTOMPIECERESOURCE *resource = next_resource++;
        const i32 category = next_category++;
        if (category == 2)
            continue;
        if (category == 0) {
            const u16 *pieces = customiser->save[side].pieces;
            if ((customiser->piece_sets[1][pieces[1]].layer_flags & 1) != 0 ||
                (customiser->piece_sets[0][pieces[0]].layer_flags & 0x20) != 0)
                continue;
        }

        nuhspecial_s *special = &resource->special;
        if (!NuSpecialExistsFn(special))
            continue;

        NUVEC momentum;
        SetKillPartMom(&momentum);
        momentum.y += scale;
        if (animation != -1)
            momentum.y += 1.0f;

        ADDPART_s params = Default_ADDPART;
        params.matrix = &object->joint_matrices[joint];
        params.velocity = &momentum;
        params.field_14 = 0.1f;
        params.field_18 = 0.1f;
        params.gravity = -5.0f;
        params.special = special;
        params.lighting = Cheats_CheckFlags(1) ? reinterpret_cast<PARTLIGHTSOURCE_s *>(ZeroRTL)
                                               : reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);
        params.flags = mode == 0 ? 0x480 : 0x90;
        params.time_step = FRAMETIME;
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_Flickerer;
        params.field_3c = PartImpact_Brick;
        AddPart(&params);
    } while (next_category != 9);
}

static inline void Customiser_DumpAccessory(CUSTOMISER *customiser, i32 side, i32 category) {
    if (customiser->categories[category]->name == NULL)
        return;

    CUSTOMPIECERESOURCE *resource = &Accessory[side][category];
    if (resource->scene != NULL) {
        NuGScnRemove(resource->scene);
        resource->scene = NULL;
    } else if (resource->texture_id != 0) {
        NUMTL *material = resource->character_model->hierarchy->materials[resource->material_index];
        material->tex_id = static_cast<i16>(resource->original_texture_id);
        NuMtlUpdate(material);
        NuTexDestroy(resource->texture_id);
    }
}

void Customiser_DumpAccessories(CUSTOMISER *customiser) {
    if (customiser == NULL || Customiser_AccessoriesLoaded != 1)
        return;

    if (apicharsys->playermodelids[customiser->character_ids[0]] != -1) {
        Customiser_DumpAccessory(customiser, 0, 0);
        Customiser_DumpAccessory(customiser, 0, 1);
        Customiser_DumpAccessory(customiser, 0, 2);
        Customiser_DumpAccessory(customiser, 0, 3);
        Customiser_DumpAccessory(customiser, 0, 4);
        Customiser_DumpAccessory(customiser, 0, 5);
        Customiser_DumpAccessory(customiser, 0, 6);
        Customiser_DumpAccessory(customiser, 0, 7);
        Customiser_DumpAccessory(customiser, 0, 8);
    }
    if (apicharsys->playermodelids[customiser->character_ids[1]] != -1) {
        Customiser_DumpAccessory(customiser, 1, 0);
        Customiser_DumpAccessory(customiser, 1, 1);
        Customiser_DumpAccessory(customiser, 1, 2);
        Customiser_DumpAccessory(customiser, 1, 3);
        Customiser_DumpAccessory(customiser, 1, 4);
        Customiser_DumpAccessory(customiser, 1, 5);
        Customiser_DumpAccessory(customiser, 1, 6);
        Customiser_DumpAccessory(customiser, 1, 7);
        Customiser_DumpAccessory(customiser, 1, 8);
    }
}

void Customiser_LoadAll(CUSTOMISER *customiser, WORLDINFO_s *world) {
    if (customiser == NULL) {
        return;
    }

    Customiser_AccessoriesLoaded = 2;
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    const auto allocate_category = [customiser, world](i32 category_index) {
        const i32 piece_count = customiser->piece_counts[category_index];
        if (piece_count < 1) {
            world->customiser_resources[category_index] = NULL;
        } else {
            CUSTOMPIECECATEGORY *category = customiser->categories[category_index];
            if (category == NULL || category->name == NULL) {
                world->customiser_resources[category_index] = NULL;
            } else {
                const usize bytes = piece_count * sizeof(CUSTOMPIECERESOURCE);
                CUSTOMPIECERESOURCE *resources = reinterpret_cast<CUSTOMPIECERESOURCE *>(world->giz_buffer.void_ptr);
                world->customiser_resources[category_index] = resources;
                memset(resources, 0, bytes);
                world->giz_buffer.addr += bytes;
            }
        }
    };
    allocate_category(0);
    allocate_category(1);
    allocate_category(2);
    allocate_category(3);
    allocate_category(4);
    allocate_category(5);
    allocate_category(6);
    allocate_category(7);
    allocate_category(8);

    void *texture_pack = NULL;
    extern i32 CHARPAK;
    if (CHARPAK != 0) {
        texture_pack = NuFilePakLoad("chars\\weirdo\\all_textures.fpk", &world->giz_buffer, world->unknown_0108, 0x20);
    }

    for (i32 category_index = 0; category_index < 9; ++category_index) {
        CUSTOMPIECERESOURCE *resources = world->customiser_resources[category_index];
        world->customiser_shared_scenes[category_index] = NULL;
        if (resources == NULL) {
            continue;
        }

        CUSTOMPIECECATEGORY *category = customiser->categories[category_index];
        if (CUSTOMISER_USEBIGSCENES != 0 && category->uses_special != 0 && category->shared_scene != NULL) {
            world->customiser_shared_scenes[category_index] =
                NuGScnRead(&world->giz_buffer, world->unknown_0108, category->shared_scene);
        }

        for (i32 piece_index = 0; piece_index < customiser->piece_counts[category_index]; ++piece_index) {
            CUSTOMPIECERESOURCE *resource = &resources[piece_index];
            CUSTOMPIECE *piece = &customiser->piece_sets[category_index][piece_index];
            char piece_name[0x80];
            char path[0x80];
            NuStrCpy(piece_name, piece->name);
            NuStrCpy(path, "chars\\weirdo\\");
            NuStrCat(path, customiser->categories[category_index]->name);
            NuStrCat(path, "\\");
            NuStrCat(path, piece_name);

            if (customiser->categories[category_index]->uses_special != 0) {
                resource->scene = world->customiser_shared_scenes[category_index];
                if (resource->scene == NULL) {
                    NuStrCat(path, ".gsc");
                    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 0x40);
                    resource->scene = NuGScnRead(&world->giz_buffer, world->unknown_0108, path);
                }
                if (resource->scene != NULL) {
                    NuSpecialFind(resource->scene, &resource->special, piece_name, 1);
                }
                continue;
            }

            if (customiser->categories[category_index]->material_tag == -1) {
                continue;
            }
            PLATFORMS_SUPPORTED platform = NuPlatform::Get()->GetCurrentPlatform();
            if (platform != IOS_PLATFORM && platform != ANDROID_ATITC_PLATFORM) {
                NuStrCat(path, ".pnt");
            }

            if (texture_pack != NULL) {
                char converted_path[0x88];
                NuFileExtConvert(converted_path, path);
                i32 item = NuFilePakGetItem(texture_pack, converted_path);
                void *texture_data;
                i32 texture_size;
                if (item != -1 && NuFilePakGetItemInfo(texture_pack, item, &texture_data, &texture_size) != 0) {
                    resource->texture_id =
                        NuTexCreateNative(static_cast<NUNATIVETEX *>(NuPtrBlockFix(texture_data)), true);
                }
            }
            if (resource->texture_id == 0) {
                resource->texture_id = NuTexRead(path, &world->giz_buffer, world->unknown_0108);
            }
        }
    }
}

void Customiser_DumpAll(CUSTOMISER *customiser, WORLDINFO_s *world) {
    if (customiser == NULL) {
        return;
    }
    for (i32 category = 0; category < 9; ++category) {
        CUSTOMPIECERESOURCE *resources = world->customiser_resources[category];
        if (resources == NULL) {
            continue;
        }
        for (i32 piece = 0; piece < customiser->piece_counts[category]; ++piece) {
            CUSTOMPIECERESOURCE *resource = &resources[piece];
            if (resource->scene != NULL) {
                if (resource->scene != world->customiser_shared_scenes[category]) {
                    NuGScnRemove(resource->scene);
                }
                resource->scene = NULL;
            } else if (resource->texture_id != 0) {
                NuTexDestroy(resource->texture_id);
            }
        }
        if (world->customiser_shared_scenes[category] != NULL) {
            NuGScnRemove(world->customiser_shared_scenes[category]);
            world->customiser_shared_scenes[category] = NULL;
        }
    }
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

void Customiser_SaveModelTextureIDs(CUSTOMISER *customiser, CHARACTERMODEL_s *model) {
    if (model == NULL || customiser == NULL) {
        return;
    }
    for (i32 character = 0; character < 2; ++character) {
        if (customiser->character_ids[character] != model->model_id) {
            continue;
        }
        nuhgobj_s *hierarchy = model->hierarchy;
        for (i32 category = 0; category < 9; ++category) {
            for (i32 material = 0; material < hierarchy->material_count; ++material) {
                NUMTL *entry = hierarchy->materials[material];
                if (entry->unknown_9a[0] == customiser->categories[category]->material_tag) {
                    customiser->model_texture_ids[character * 9 + category] = entry->tex_id;
                }
            }
        }
    }
}

void Customiser_RestoreModelTextureIDs(CUSTOMISER *customiser) {
    if (customiser == NULL)
        return;
    for (i32 character = 0; character < 2; ++character) {
        CHARACTERMODEL_s *model = APICharacterLoaded(customiser->character_ids[character]);
        if (model == NULL)
            continue;
        for (i32 category = 0; category < 9; ++category) {
            if (customiser->model_texture_ids[character * 9 + category] == 0)
                continue;
            for (i32 material = 0; material < model->hierarchy->material_count; ++material) {
                NUMTL *entry = model->hierarchy->materials[material];
                if (entry->unknown_9a[0] == customiser->categories[category]->material_tag) {
                    entry->tex_id = customiser->model_texture_ids[character * 9 + category];
                    NuMtlUpdate(entry);
                }
            }
        }
    }
}

void Customiser_Set100PercentPieces(CUSTOMISER *customiser) {
    if (customiser == NULL || customiser->save == NULL)
        return;
    CUSTOMISESAVE_s *save = customiser->save;
    for (i32 category = 0; category < 9; ++category) {
        const i32 count = customiser->piece_counts[category];
        if (count <= 0)
            continue;
        CUSTOMPIECE *piece = customiser->piece_sets[category];
        for (i32 index = 0; index < count; ++index, ++piece) {
            const u16 flags = piece->availability_flags;
            if ((flags & 0x80) != 0)
                save[0].pieces[category] = index;
            if ((flags & 0x100) != 0)
                save[1].pieces[category] = index;
        }
    }
}

void Customiser_CopyDefaultPiecesToSave(CUSTOMISER *customiser, CUSTOMISESAVE_s *save) {
    if (customiser == NULL) {
        return;
    }
    if (save == NULL) {
        save = customiser->save;
        if (save == NULL) {
            return;
        }
    }
    save[0].pieces[0] = customiser->default_pieces[0][0];
    save[0].pieces[1] = customiser->default_pieces[0][1];
    save[0].pieces[2] = customiser->default_pieces[0][2];
    save[0].pieces[3] = customiser->default_pieces[0][3];
    save[0].pieces[4] = customiser->default_pieces[0][4];
    save[0].pieces[5] = customiser->default_pieces[0][5];
    save[0].pieces[6] = customiser->default_pieces[0][6];
    save[0].pieces[7] = customiser->default_pieces[0][7];
    save[0].pieces[8] = customiser->default_pieces[0][8];
    save[1].pieces[0] = customiser->default_pieces[1][0];
    save[1].pieces[1] = customiser->default_pieces[1][1];
    save[1].pieces[2] = customiser->default_pieces[1][2];
    save[1].pieces[3] = customiser->default_pieces[1][3];
    save[1].pieces[4] = customiser->default_pieces[1][4];
    save[1].pieces[5] = customiser->default_pieces[1][5];
    save[1].pieces[6] = customiser->default_pieces[1][6];
    save[1].pieces[7] = customiser->default_pieces[1][7];
    save[1].pieces[8] = customiser->default_pieces[1][8];
}

CUSTOMISER *Customiser_Configure(char *filename, VARIPTR *buffer, VARIPTR *, i32 first_character, i32 second_character,
                                 i32 (*available)(CUSTOMPIECE *), void (*configure_piece)(CUSTOMPIECE *, nufpar_s *),
                                 i32 (*weapon_from_name)(char *), CUSTOMISESAVE_s *save, i16 *animations) {
    if (first_character == -1 && second_character == -1)
        return NULL;
    if (Game_Customiser != NULL)
        return NULL;
    NUFPAR *parser = NuFParCreate(filename);
    if (parser == NULL)
        return NULL;

    i32 locators[9], layers[9];
    f32 x_offsets[9] = {}, y_offsets[9] = {};
    u8 categories[440];
    CUSTOMPIECE pieces[440];
    for (i32 category = 0; category < 9; ++category) {
        locators[category] = -1;
        layers[category] = -1;
    }
    memset(pieces, 0, sizeof(pieces));
    char *names = buffer->char_ptr;
    i32 count = 0;
    while (NuFParGetLine(parser) != 0) {
        if (NuFParGetWord(parser) == 0)
            break;
        i32 category = Customiser_FindSetFromName(parser->word_buf);
        if (category != -1 && NuFParGetWord(parser) != 0) {
            categories[count] = category;
            CUSTOMPIECE *piece = &pieces[count];
            piece->name = names;
            piece->field_0x10 = -1;
            piece->collection_type = -1;
            piece->availability_flags = 0;
            piece->model_flags = 0;
            piece->gameplay_flags = 0;
            piece->weapon_model = -1;
            piece->character_id = -1;
            piece->icon_character_id = -1;
            piece->unknown_indices_0a[0] = -1;
            piece->unknown_indices_0a[1] = -1;
            piece->unknown_indices_0a[2] = -1;
            NuStrCpy(names, parser->word_buf);
            names += NuStrLen(parser->word_buf) + 1;
            NuStrLen(parser->word_buf);
            if (CustomSetData[category].name == NULL && weapon_from_name != NULL)
                piece->weapon_model = weapon_from_name(piece->name);
            while (NuFParGetWord(parser) != 0) {
                if (NuStrICmp(parser->word_buf, "from") == 0) {
                    if (NuFParGetWord(parser) != 0)
                        piece->character_id = CharIDFromName(parser->word_buf);
                } else if (NuStrICmp(parser->word_buf, "icon_from") == 0) {
                    if (NuFParGetWord(parser) != 0)
                        piece->icon_character_id = CharIDFromName(parser->word_buf);
                } else if (NuStrICmp(parser->word_buf, "from_variant") == 0) {
                    if (NuFParGetWord(parser) != 0)
                        piece->collection_type = CharVariant_Find(parser->word_buf);
                } else if (NuStrICmp(parser->word_buf, "no_hat") == 0) {
                    piece->availability_flags |= 1;
                } else if (NuStrICmp(parser->word_buf, "hat_off") == 0) {
                    piece->availability_flags |= 0x20;
                } else if (NuStrICmp(parser->word_buf, "cape_off") == 0) {
                    piece->availability_flags |= 0x40;
                } else if (NuStrICmp(parser->word_buf, "helmet") == 0) {
                    piece->availability_flags |= 2;
                } else if (NuStrICmp(parser->word_buf, "big_head") == 0) {
                    piece->availability_flags |= 4;
                } else if (NuStrICmp(parser->word_buf, "big_hat") == 0) {
                    piece->availability_flags |= 8;
                } else if (NuStrICmp(parser->word_buf, "not_demo") == 0) {
                    piece->availability_flags |= 0x10;
                } else if (NuStrICmp(parser->word_buf, "100_percent") == 0) {
                    if (NuFParGetWord(parser) != 0) {
                        i32 side = NuAToI(parser->word_buf);
                        if (side == 1)
                            piece->availability_flags |= 0x80;
                        else if (side == 2)
                            piece->availability_flags |= 0x100;
                    }
                } else if (NuStrICmp(parser->word_buf, "default") == 0) {
                    if (NuFParGetWord(parser) != 0) {
                        i32 side = NuAToI(parser->word_buf);
                        if (side == 1)
                            piece->availability_flags |= 0x200;
                        else if (side == 2)
                            piece->availability_flags |= 0x400;
                    }
                } else if (configure_piece != NULL) {
                    configure_piece(piece, parser);
                }
            }
            ++count;
        } else if (NuStrICmp(parser->word_buf, "locator") == 0) {
            if (NuFParGetWord(parser) != 0) {
                category = Customiser_FindSetFromName(parser->word_buf);
                if (category != -1) {
                    i32 locator = NuFParGetInt(parser);
                    locators[category] = static_cast<u32>(locator) < 16 ? locator : -1;
                    if (static_cast<u32>(locator) < 16) {
                        while (NuFParGetWord(parser) != 0) {
                            if (NuStrICmp(parser->word_buf, "xoffset") == 0)
                                x_offsets[category] = NuFParGetFloat(parser);
                            else if (NuStrICmp(parser->word_buf, "yoffset") == 0)
                                y_offsets[category] = NuFParGetFloat(parser);
                        }
                    }
                }
            }
        } else if (NuStrICmp(parser->word_buf, "layer") == 0) {
            if (NuFParGetWord(parser) != 0) {
                category = Customiser_FindSetFromName(parser->word_buf);
                if (category != -1) {
                    i32 layer = NuFParGetInt(parser);
                    layers[category] = static_cast<u32>(layer) < 32 ? layer : -1;
                }
            }
        }
        if (count >= 440)
            break;
    }
    NuFParDestroy(parser);
    if (count == 0)
        return NULL;

    // Retail uses four-byte arena alignment. Pointer-bearing native records
    // need their real alignment and size, not the original i386 byte strides.
    buffer->addr = ALIGN(reinterpret_cast<usize>(names), alignof(CUSTOMISER));
    CUSTOMISER *customiser = static_cast<CUSTOMISER *>(buffer->void_ptr);
    memset(customiser, 0, sizeof(*customiser));
    buffer->addr = ALIGN(buffer->addr + sizeof(*customiser), alignof(CUSTOMPIECE));
    customiser->character_ids[0] = first_character;
    customiser->character_ids[1] = second_character;
    customiser->piece_available = available != NULL ? available : Customiser_PieceAvailable_Default;
    customiser->save = save;
    customiser->animation_ids_to_load = reinterpret_cast<u16 *>(animations);
    for (i32 category = 0; category < 9; ++category) {
        customiser->categories[category] = &CustomSetData[category];
        CUSTOMPIECE *output = static_cast<CUSTOMPIECE *>(buffer->void_ptr);
        customiser->piece_sets[category] = output;
        customiser->piece_counts[category] = 0;
        customiser->locator_indices[category] = locators[category];
        customiser->layer_indices[category] = layers[category];
        customiser->locator_x_offsets[category] = x_offsets[category];
        customiser->locator_y_offsets[category] = y_offsets[category];
        for (i32 index = 0; index < count; ++index) {
            if (categories[index] == category) {
                memmove(output, &pieces[index], sizeof(*output));
                ++customiser->piece_counts[category];
                ++output;
            }
        }
        if (customiser->piece_counts[category] > 0)
            buffer->addr = ALIGN(reinterpret_cast<usize>(output), alignof(CUSTOMPIECE));
    }
    if (customiser->save == NULL) {
        customiser->save = static_cast<CUSTOMISESAVE_s *>(buffer->void_ptr);
        // The serialized save occupies a four-byte-rounded arena allocation.
        buffer->addr = ALIGN(buffer->addr + sizeof(CUSTOMISESAVE_s) * 2, 4);
    }
    for (i32 category = 0; category < 9; ++category) {
        i32 first_found = 0, second_found = 0;
        const i32 piece_count = customiser->piece_counts[category];
        for (i32 index = 0; index < piece_count && !(first_found && second_found); ++index) {
            const u16 flags = customiser->piece_sets[category][index].availability_flags;
            if ((flags & 0x200) != 0) {
                customiser->default_pieces[0][category] = index;
                first_found = 1;
            }
            if ((flags & 0x400) != 0) {
                customiser->default_pieces[1][category] = index;
                second_found = 1;
            }
        }
    }
    Customiser_CopyDefaultPiecesToSave(customiser, NULL);
    customiser->field_0xd10 = 0;
    customiser->field_0xd11 = 0;
    customiser->field_0xd12 = 0;
    customiser->field_0xd13 = 0;
    customiser->field_0xd16 = 0;
    customiser->field_0xd17 = 0;
    Game_Customiser = customiser;
    return customiser;
}

CUSTOMPIECE *Customiser_FindPieceByName(CUSTOMISER *customiser, char *name, i32 *category, i32 *index) {
    if (customiser != NULL) {
        for (i32 category_index = 0; category_index < 9; ++category_index) {
            for (i32 piece_index = 0; piece_index < customiser->piece_counts[category_index]; ++piece_index) {
                if (NuStrICmp(name, customiser->piece_sets[category_index][piece_index].name) == 0) {
                    if (category != NULL)
                        *category = category_index;
                    if (index != NULL)
                        *index = piece_index;
                    return &customiser->piece_sets[category_index][piece_index];
                }
            }
        }
    }
    return NULL;
}

i32 Customiser_GetIcon(CUSTOMISER *customiser, CUSTOMISESAVE_s *save, i32) {
    CUSTOMPIECE *first = &customiser->piece_sets[0][static_cast<u16>(save[0].pieces[0])];
    CUSTOMPIECE *second = &customiser->piece_sets[1][static_cast<u16>(save[0].pieces[1])];
    i32 id;
    if (first->layer_flags & 0x20)
        goto second_piece;
    if (second->layer_flags & 1)
        goto second_piece;
    id = first->icon_character_id;
    if (id != -1)
        goto resolved_icon;
    id = first->character_id;
    if (id != -1)
        goto resolved_icon;
second_piece:
    id = second->icon_character_id;
    if (id == -1)
        id = second->character_id;
resolved_icon:
    if (id != -1)
        return CDataList[id].field20_0x42;
    return LEGOOBJ_ICON_WEIRDO;
}
