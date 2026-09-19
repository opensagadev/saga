#ifndef LEGOAPI_ITEMS_OBJECTS_GRABBER_H
#define LEGOAPI_ITEMS_OBJECTS_GRABBER_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

extern GRABBER_s *Grab_grabber;

void Grabber_Configure(WORLDINFO_s *world, char *config);
void Grabber_StoreProgress(WORLDINFO_s *world, LEVEL_PROGRESS_s *progress);

#endif
