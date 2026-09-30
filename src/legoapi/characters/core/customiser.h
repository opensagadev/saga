#pragma once

#include "decomp.h"

struct CUSTOMISER;
struct CUSTOMPIECE;
struct CUSTOMISESAVE_s;
struct APICHARACTERMODELLIST_s;
struct GameObject_s;
struct nufpar_s;

i32 Customiser_NextPieceLeft(CUSTOMISER *customiser, i32 index, i32 count, i32 unused, i32 category);
i32 Customiser_GetIcon(CUSTOMISER *customiser, CUSTOMISESAVE_s *save, i32 side);
i32 Customiser_NextPieceRight(CUSTOMISER *customiser, i32 index, i32 count, i32 unused, i32 category);
void Customiser_ResetModelTextureIDs(CUSTOMISER *customiser);
void Customiser_CopyDefaultPiecesToSave(CUSTOMISER *customiser, CUSTOMISESAVE_s *save);
void Customiser_LoadAccessories(CUSTOMISER *customiser, APICHARACTERMODELLIST_s *models);
void Customiser_DumpAccessories(CUSTOMISER *customiser);
void Customiser_RestoreModelTextureIDs(CUSTOMISER *customiser);
void Customiser_TransformToPanel(CUSTOMISER *customiser);
void Customiser_InitNames(CUSTOMISER *customiser);
void CustomiserMenu_End();
void Customiser_GetActiveWeirdoIndex(i32 *index, i32 *count);
i32 Customise_GetToggleString(i32 index);
void Customiser_PieceConfig(CUSTOMPIECE *piece, nufpar_s *parser);
i32 Customiser_PieceAvailable(CUSTOMPIECE *piece);
CUSTOMISER *Customiser_Configure(char *filename, VARIPTR *buffer, VARIPTR *end, i32 first_character,
                                 i32 second_character, i32 (*available)(CUSTOMPIECE *),
                                 void (*configure_piece)(CUSTOMPIECE *, nufpar_s *), i32 (*weapon_from_name)(char *),
                                 CUSTOMISESAVE_s *save, i16 *animations);
CUSTOMPIECE *Customiser_FindPieceByName(CUSTOMISER *customiser, char *name, i32 *category, i32 *index);
void Customiser_AddPartAccessories(CUSTOMISER *customiser, GameObject_s *object, i32 animation, i32 mode, float scale);
extern i32 customiser_quit;
