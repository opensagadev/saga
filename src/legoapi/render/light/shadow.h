#pragma once

#include "nu2api/nucore/fixed_width.h"

// Shadow light system (module legoapi/render/light, shadow.cpp).

void InitShadowLights();
float BlobShadowFade(struct nuvec_s *position, float fade_start, float fade_end, float alpha);
void EnableShadowMapRendering(i32 enable);
void ResetShadowMapRendering();
