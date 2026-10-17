#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct RGBA;
struct WORLDINFO_s;
struct numtl_s;
struct numtx_s;
struct nuvec_s;
struct ripple_node_s;
struct ripple_set_s;
i32 LookupRippleEffectIndex(char *name);
void AddRipple(ripple_set_s *, numtx_s *, float, float, float, float, RGBA, RGBA, i32, numtl_s *, nuvec_s *);
void DestroyRippleMtls(WORLDINFO_s *);
void DrawRipple(ripple_node_s *node);
void InitRipples(ripple_set_s **, VARIPTR *, VARIPTR *, i32);
void ResetRippleSet(ripple_set_s *);
