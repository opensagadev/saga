#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct DEFLATECONTEXT;
i32 DecodeDeflatedBlock(DEFLATECONTEXT *ctx);
i32 DecodeUncompressedBlock(DEFLATECONTEXT *ctx);
