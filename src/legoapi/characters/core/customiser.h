#pragma once

#include "decomp.h"

struct CUSTOMISER;
struct CUSTOMISESAVE_s;
struct APICHARACTERMODELLIST_s;

i32 Customiser_NextPieceLeft(CUSTOMISER *customiser, i32 index, i32 count, i32 unused, i32 category);
i32 Customiser_GetIcon(CUSTOMISER *customiser, CUSTOMISESAVE_s *save, i32 side);
i32 Customiser_NextPieceRight(CUSTOMISER *customiser, i32 index, i32 count, i32 unused, i32 category);
void Customiser_ResetModelTextureIDs(CUSTOMISER *customiser);
void Customiser_CopyDefaultPiecesToSave(CUSTOMISER *customiser, CUSTOMISESAVE_s *save);
void Customiser_LoadAccessories(CUSTOMISER *customiser, APICHARACTERMODELLIST_s *models);
void Customiser_DumpAccessories(CUSTOMISER *customiser);
void Customiser_RestoreModelTextureIDs(CUSTOMISER *customiser);
void Customiser_TransformToPanel(CUSTOMISER *customiser);
extern i32 customiser_quit;
