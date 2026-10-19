#pragma once

#include "decomp.h"

struct GAMEPAD_s;
struct GameObject_s;
struct nupad_s;

// Gamepad system (module legoapi/core/input, gamepads.cpp).

extern "C" GAMEPAD_s GamePad[64];

// Directional/button mask constants (original .data @0x667bf0-0x667c40).
extern u32 GAMEPAD_LIFT;
extern u32 GAMEPAD_DRIGHT;
extern u32 GAMEPAD_DLEFT;
extern u32 GAMEPAD_DDOWN;
extern u32 GAMEPAD_DUP;
extern u32 GAMEPAD_TOGGLERIGHT;
extern u32 GAMEPAD_TOGGLELEFT;
extern u32 GAMEPAD_LIFT;
extern u32 GAMEPAD_TAG;
extern u32 GAMEPAD_R3;
extern u32 GAMEPAD_SPECIAL;
extern u32 GAMEPAD_ACTION;
extern u32 GAMEPAD_JUMP;
extern u32 GAMEPAD_START;
extern u32 GAMEPAD_SELECT;
extern u32 GAMEPAD_MENUSELECT;
extern u32 GAMEPAD_MENUCANCEL;

void GamePads_Init();
void GamePad_InitButtons();
GAMEPAD_s *GamePad_Allocate();
u16 GamePad_InputAngle(GameObject_s *object, GAMEPAD_s *pad);
f32 GamePad_Rotate(GameObject_s *object);
i32 GamePad_Waggle(GAMEPAD_s *pad);
i32 ObjLookingWithLeftStick(GameObject_s *object);
i32 ReadPad(i32 port);
void ReadPads();
i32 GamePads_SkipMovie();
i32 NoPad(i32 port, i32 require_game_input);
void PadOutPause(i32 port, struct WORLDINFO_s *world);
void DoInput(struct WORLDINFO_s *world);
void NewRumble(nupad_s *pad, f32 strength, i32 mode);
void NewBuzz(nupad_s *pad, f32 duration, i32 mode);
void NewBuzzFrames(nupad_s *pad, i32 frames, i32 mode);
void NewRumbleAllPlayers(f32 strength, f32 duration, i32 frames, i32 player_index);
void DieRumble(GameObject_s *object);
void SpaceRumbleProcess();

extern "C" i32 Controller_IsConnected();
extern "C" bool TestForController();

extern f32 PadOldSpeed2[2];
extern f32 PadOldSpeed[2];
extern u16 PadOldAngle2[2];
extern u16 PadOldAngle[2];

// Declared here for the other units that use them.
struct GameObject_s;
struct RUMBLEPACKET;
extern i32 readpads_always;
void ConstantRumble(GameObject_s *object, f32 strength, f32 duration);
void HitRumble(GameObject_s *);
void KillRumble(GameObject_s *);
void ResetRumble(RUMBLEPACKET *packet);
void UpdateRumble(RUMBLEPACKET *packet);
void TakeHitRumble(GameObject_s *object, float strength);
