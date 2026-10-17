#ifndef LEGOAPI_ITEMS_OBJECTS_GRABBER_H
#define LEGOAPI_ITEMS_OBJECTS_GRABBER_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

extern GRABBER_s *Grab_grabber;

void Grabber_Configure(WORLDINFO_s *world, char *config);
void Grabber_StoreProgress(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress);

#endif

// Declared here for the other units that use them.
struct PART_s;
struct WORLDINFO_s;
void Grabber_Reset(WORLDINFO_s *);
void PartKill_Grabber(PART_s *, i32);
