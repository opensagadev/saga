#include "legoapi/characters/core/customiser.h"

#include "legoapi/legoapi_types.h"
#include "globals.h"
#include "batman.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/menus/screens/store.h"
#include "legoapi/menus/screens/gamestructure.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nucore/nustring.h"
#include <string.h>

struct CUSTOMPIECERESOURCE {
    NUGSCN *scene;
    nuhspecial_s special;
    i32 original_texture_id;
    union {
        i32 texture_id;
        void *model;
    };
    i32 material_index;
    CHARACTERMODEL_s *character_model;
};
DECOMP_ASSERT(sizeof(CUSTOMPIECERESOURCE) == 0x20, "CUSTOMPIECERESOURCE size");
DECOMP_ASSERT(offsetof(CUSTOMPIECERESOURCE, texture_id) == 0x14, "Customiser texture offset");
DECOMP_ASSERT(offsetof(CUSTOMPIECERESOURCE, character_model) == 0x1c, "Customiser model offset");
static CUSTOMPIECERESOURCE Accessory[2][9];

static __used__ bool Customiser_PieceAvailable_Default(CUSTOMPIECE *) {
    STUBBED();
    return {};
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
                resource->texture_id =
                    NuTexRead(path, &characterbuffer_ptr, reinterpret_cast<VARIPTR *>(characterbuffer_end.addr));
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

void Customiser_AddPartAccessories(CUSTOMISER *, GameObject_s *, i32, i32, float) {
    STUBBED();
}

void Customiser_DumpAccessories(CUSTOMISER *) {
    STUBBED();
}

void Customiser_LoadAll(CUSTOMISER *, WORLDINFO_s *) {
    STUBBED();
}

void Customiser_DumpAll(CUSTOMISER *, WORLDINFO_s *) {
    STUBBED();
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

void Customiser_SaveModelTextureIDs(CUSTOMISER *, CHARACTERMODEL_s *) {
    STUBBED();
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

void Customiser_FindPieceByName(CUSTOMISER *, char *, i32 *, i32 *) {
    STUBBED();
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
