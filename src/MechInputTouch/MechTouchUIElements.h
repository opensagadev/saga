#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct MechTouchUIElement;
struct TouchHolder;
struct VuMtx;
struct numtl_s;
void MechTouchUIPartySelector_OnRelease_Callback(MechTouchUIElement &, TouchHolder &);
void MechTouchUIPauseButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);
void MechTouchUITagButton_OnClick_Callback(MechTouchUIElement &, TouchHolder &);
void PerformPauseButtonStuff();
void PlayerButton_OnClick_Callback_NextButton(MechTouchUIElement &, TouchHolder &);
void PlayerButton_OnHold_Callback(MechTouchUIElement &, TouchHolder &);
void PlayerButton_OnLeave_Callback(MechTouchUIElement &, TouchHolder &);
void RndrTexQuad(f32, f32, f32, f32, i32, numtl_s *, i32);
void RndrTexQuad3D(VuMtx const &matrix, i32 colour, numtl_s *material);
