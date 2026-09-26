#include "legoapi/characters/core/customiser.h"

#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "batman.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/render/fx/parts.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nuplatform/nuplatform.h"
#include <string.h>

DECOMP_ASSERT(sizeof(CUSTOMPIECERESOURCE) == 0x20, "CUSTOMPIECERESOURCE size");
DECOMP_ASSERT(offsetof(CUSTOMPIECERESOURCE, texture_id) == 0x14, "Customiser texture offset");
DECOMP_ASSERT(offsetof(CUSTOMPIECERESOURCE, character_model) == 0x1c, "Customiser model offset");
static CUSTOMPIECERESOURCE Accessory[2][9];

static __used__ bool Customiser_PieceAvailable_Default(CUSTOMPIECE *piece) {
    if ((piece->availability_flags & 0x180) != 0 && Game_100PercentComplete() == 0) {
        return false;
    }
    return true;
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
            const u16 piece_index = side == 0 ? static_cast<u16>(customiser->save->pieces[category_index])
                                              : static_cast<u16>(customiser->save->secondary_pieces[category_index]);
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

void Customiser_DrawAccessories(CUSTOMISER *, GameObject_s *, numtx_s *) {
    STUBBED();
}

void Customiser_AddPartAccessories(CUSTOMISER *customiser, GameObject_s *object, i32 animation, i32 mode, float scale) {
    if (customiser == NULL || object == NULL || object->apiobj.character_data == NULL ||
        object->apiobj.character_data->player_config == NULL || object->apiobj.character_model == NULL)
        return;
    const i32 joint = object->apiobj.character_data->player_config->helmet_locator;
    if (joint == -1 || object->apiobj.character_model->points_of_interest[joint] == NULL)
        return;

    const i32 side = object->id != customiser->character_ids[0];
    const i16 *pieces = side == 0 ? customiser->save->pieces : customiser->save->secondary_pieces;
    for (i32 category = 0; category != 9; ++category) {
        if (category == 2)
            continue;
        if (category == 0 &&
            ((customiser->piece_sets[1][static_cast<u16>(pieces[1])].availability_flags & 1) != 0 ||
             (customiser->piece_sets[0][static_cast<u16>(pieces[0])].availability_flags & 0x20) != 0)) {
            continue;
        }

        nuhspecial_s *special = &Accessory[side][category].special;
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
        params.flags = mode < 1 ? 0x480 : 0x90;
        params.field_3c = PartImpact_Brick;
        params.stop_fn = PartStop_Flickerer;
        params.draw_fn = PartDraw_Flickerer;
        params.time_step = FRAMETIME;
        params.lighting = Cheats_CheckFlags(1) ? reinterpret_cast<PARTLIGHTSOURCE_s *>(ZeroRTL)
                                               : reinterpret_cast<PARTLIGHTSOURCE_s *>(&object->light_data);
        AddPart(&params);
    }
}

void Customiser_DumpAccessories(CUSTOMISER *) {
    STUBBED();
}

void Customiser_LoadAll(CUSTOMISER *customiser, WORLDINFO_s *world) {
    if (customiser == NULL) {
        return;
    }

    Customiser_AccessoriesLoaded = 2;
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    for (i32 category_index = 0; category_index < 9; ++category_index) {
        CUSTOMPIECECATEGORY *category = customiser->categories[category_index];
        const i32 piece_count = customiser->piece_counts[category_index];
        if (piece_count < 1 || category == NULL || category->name == NULL || category->name[0] == '\0') {
            world->customiser_resources[category_index] = NULL;
            continue;
        }

        const usize bytes = piece_count * sizeof(CUSTOMPIECERESOURCE);
        CUSTOMPIECERESOURCE *resources = reinterpret_cast<CUSTOMPIECERESOURCE *>(world->giz_buffer.void_ptr);
        world->customiser_resources[category_index] = resources;
        memset(resources, 0, bytes);
        world->giz_buffer.addr += bytes;
    }

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
            NuStrCat(path, category->name);
            NuStrCat(path, "\\");
            NuStrCat(path, piece_name);

            if (category->uses_special != 0) {
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

            if (category->material_tag == -1) {
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
                if (entry->unknown_9a[0] == static_cast<u8>(customiser->categories[category]->material_tag)) {
                    customiser->model_texture_ids[character * 9 + category] = entry->tex_id;
                }
            }
        }
    }
}

void Customiser_RestoreModelTextureIDs(CUSTOMISER *) {
    STUBBED();
}

void Customiser_Set100PercentPieces(CUSTOMISER *) {
    STUBBED();
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
    save->pieces[0] = customiser->default_pieces[0][0];
    save->pieces[1] = customiser->default_pieces[0][1];
    save->pieces[2] = customiser->default_pieces[0][2];
    save->pieces[3] = customiser->default_pieces[0][3];
    save->pieces[4] = customiser->default_pieces[0][4];
    save->pieces[5] = customiser->default_pieces[0][5];
    save->pieces[6] = customiser->default_pieces[0][6];
    save->pieces[7] = customiser->default_pieces[0][7];
    save->pieces[8] = customiser->default_pieces[0][8];
    save->secondary_pieces[0] = customiser->default_pieces[1][0];
    save->secondary_pieces[1] = customiser->default_pieces[1][1];
    save->secondary_pieces[2] = customiser->default_pieces[1][2];
    save->secondary_pieces[3] = customiser->default_pieces[1][3];
    save->secondary_pieces[4] = customiser->default_pieces[1][4];
    save->secondary_pieces[5] = customiser->default_pieces[1][5];
    save->secondary_pieces[6] = customiser->default_pieces[1][6];
    save->secondary_pieces[7] = customiser->default_pieces[1][7];
    save->secondary_pieces[8] = customiser->default_pieces[1][8];
}

void Customiser_Configure(char *, variptr_u *, variptr_u *, i32, i32, i32 (*)(CUSTOMPIECE *),
                          void (*)(CUSTOMPIECE *, nufpar_s *), i32 (*)(char *), CUSTOMISESAVE_s *, i16 *) {
    STUBBED();
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
    CUSTOMPIECE *first = &customiser->piece_sets[0][static_cast<u16>(save->pieces[0])];
    CUSTOMPIECE *second = &customiser->piece_sets[1][static_cast<u16>(save->pieces[1])];
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
