#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
abi_ulong GetMatchLength(unsigned char *first, unsigned char *second, abi_ulong maximum);
extern i32 refpack(unsigned char *, abi_long, unsigned char *);
u32 HashString(unsigned char *string);
void refpack_init();
