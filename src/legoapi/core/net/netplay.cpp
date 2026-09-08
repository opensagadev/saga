#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/core/input/qrand.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/world.h"
#include "legoapi/world/area.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "nu2api/nusound/nusound.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void GetClientMineInfo(nuvec_s **, u64 **, u64 **) {
}

extern void Player_ResetContexts(PLAYERPACKET_s *packet);
extern void CharShadows_Reset(PLAYERPACKET_s *packet);
extern void ResetForceGlow(PLAYERPACKET_s *packet);

void ResetPlayerPacket(PLAYERPACKET_s *packet, CHARACTERDATA_s *) {
    packet->field_0x664 = 0;
    packet->delayed_turn_timer = 0.0f;
    packet->context_state_flags &= static_cast<u8>(~0x01);
    packet->movement_lean_angle = 0;
    packet->secondary_lean_angle = 0;
    packet->tertiary_lean_angle = 0;

    Player_ResetContexts(packet);

    GAMEPAD_s *gamepad = packet->gamepad;
    gamepad->rumble_packet.field_0x00 = 0;
    gamepad->allocated_5a &= static_cast<u8>(~GAMEPAD_RUNTIME_SUPPRESS_MOVEMENT);
    gamepad->buttons_held = 0;
    gamepad->buttons_pressed = 0;
    gamepad->buttons_released = 0;
    gamepad->left_directions = 0;
    gamepad->previous_left_directions = 0;
    gamepad->right_directions = 0;
    gamepad->previous_right_directions = 0;
    gamepad->unknown_20 = 0;
    gamepad->rumble_packet.rumble_amount = 0.0f;
    gamepad->rumble_packet.field_0x08 = 0;
    gamepad->rumble_packet.rumble_time = 0.0f;
    gamepad->allocated_5a &= static_cast<u8>(~(0x04 | 0x10));
    packet->secondary_flags &= static_cast<u8>(~0x04);
    packet->input_state = 0;
    gamepad->operator_data = NULL;

    CharShadows_Reset(packet);
    packet->field_0x6c8 = 0;
    packet->field_0x6e8 = 0;
    packet->field_0x614 = 0;
    packet->field_0x618 = 0;
    packet->field_0x61c = 0;
    packet->field_0x620 = 0;
    packet->field_0x624 = 0;
    packet->field_0x628 = 0;
    packet->field_0x62c = 0;
    packet->field_0x630 = 0;

    ResetForceGlow(packet);
    packet->ground_height = 2000000.0f;
    packet->field_0x73c = 0;
    packet->field_0x6fc = 0;
    packet->movement_flags &= static_cast<u8>(~0x01);
    packet->force_glow_index = -1;
    packet->movement_angle_0 = -1;
    packet->field_0x718 = 0;
    packet->movement_angle_1 = qrand() > 0x7fff ? 0x4f : 0x26;
    packet->random_state[0] = static_cast<u8>(qrand() >> 8);
    packet->random_state[1] = static_cast<u8>(qrand() >> 8);
    packet->random_state[2] = static_cast<u8>(qrand() >> 8);
    packet->random_state[3] = static_cast<u8>(qrand() >> 8);
    packet->render_flags &= static_cast<u8>(~0x01);
    packet->field_0x71c = 0;
    packet->field_0x720 = 0;
    packet->field_0x728 = 0;
    packet->field_0x69c = 0;
    packet->field_0x690 = 0;
    packet->field_0x694 = 0;
    packet->field_0x794 = 0;
    packet->field_0x688 = 0;
    packet->field_0x771 &= static_cast<u8>(~0x0c);
    packet->reset_up_direction = v010;
    packet->field_0x604 = NULL;
    packet->surface_type = -1;
}

void FinishLoop_Network() {
}

extern STATUSPACKET_s StatusPacket;
extern FadeSystem FadeSys;
extern i32 reset_area;
extern i32 grab_screen_image;
extern i32 hub_from_mission;
void NeedScreenGrab(i32);
void GrabStillScreen();
void InitChallenge(i32);
void InitMission(MISSIONSYS_s *, i32);

void FinishStatusPacket(i32 choice) {
    RememberPlayerIDs(1, static_cast<i16>(StatusPacket.player0_model), static_cast<i16>(StatusPacket.player1_model));
    if (StatusPacket.finish_callback(WORLD, &StatusPacket, choice) != 0) {
        return;
    }
    NewLData = HUB_LDATA;
    if (choice == 0) {
        if ((StatusPacket.mode_flags & 1) != 0 || StatusPacket.challenge_state != 0 ||
            StatusPacket.mission_state != 0) {
            NewLData = Area_FindNextPlayLevel(ADataList[StatusPacket.area->index].levels[0]);
            FADETYPE fade = {FADE_TYPE_STILL_WIPE};
            ResetBits = 0xffffffbf;
            reset_area = 1;
            FadeSys.SetFade(fade, 0);
            NeedScreenGrab(1);
            GrabStillScreen();
            if (StatusPacket.challenge_state != 0) {
                InitChallenge(StatusPacket.area->index);
            }
            if (StatusPacket.mission_state != 0) {
                InitMission(MissionSys, static_cast<i8>(MissionSys->mission->count));
                NewLData = &LDataList[MissionSys->mission->level];
            }
            goto destination_selected;
        }
        if (StatusPacket.next_area != -1) {
            NewLData = &LDataList[ADataList[StatusPacket.next_area].levels[0]];
            if ((ADataList[StatusPacket.next_area].flags & 2) == 0) {
                loadareacharacters_no_backdrop_reset = 1;
                if ((StatusPacket.mode_flags & 4) != 0) {
                    finishloop_backdroponly = 1;
                }
            } else {
                grab_screen_image = 1;
            }
            goto destination_selected;
        }
    }
    if ((StatusPacket.mode_flags & 4) != 0 ||
        ((StatusPacket.area->flags & 4) != 0 && (StatusPacket.field_0xb0 & 0x40) == 0)) {
        NewLData = CREDITS_LDATA;
    }
destination_selected:
    if ((StatusPacket.mode_flags & 8) != 0) {
        NewLData = HUB_LDATA;
    }
    if (NewLData->area_index != -1 && (ADataList[NewLData->area_index].flags & 0x40) != 0 &&
        StatusPacket.mission_state != 0 && MissionSys->mission != NULL) {
        hub_from_mission = static_cast<i8>(MissionSys->mission->count);
    }
    OldBonusScore[0] = BonusScore[0];
    OldBonusScore[1] = BonusScore[1];
    if (NewLData == HUB_LDATA) {
        OldBonusScore[0] = 0;
        OldBonusScore[1] = 0;
    }
    NuSound3StopRumble();
}

i32 FinishStatusPacket_LSW(WORLDINFO_s *, STATUSPACKET_s *, i32) {
    return 0;
}

void setObjInNetWaitContext(GameObject_s *, i32) {
}
