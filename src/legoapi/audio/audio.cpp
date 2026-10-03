#include "decomp.h"
#include "globals.h"
#include "legoapi/audio/audio.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/items/base/apiobject.h"
#include "legoapi/items/collect/spacelevel.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/level.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

extern i32 DoubleScore;
extern i32 Paused;
i32 (*GameAudio_ActionMusicFn)(void) = NULL;
static f32 sticky_attack_timeout[2] = {1.0f, 6.0f};
static i32 CurrentMusicPair_Quiet = -1;
static i32 last_currentmusicpair_quiet = -1;
static i32 sticky_attack;
static f32 sticky_attack_time;
static i32 MusicPlrsUnderAttack;
static i32 MusicActive = -1;
static i32 MusicActiveReset;
static f32 MusicSwapDelay;
static i32 MusicSeekIndex;
static i32 MusicInside;
static i32 MusicFindCut;
static i32 MusicRestoreTrack;
static i32 MusicOnFlag = 1;
static i32 MusicPlrsHoldAttack;

extern "C" void PlaySfxByIdAndSetVolume(i32 sfx_id, nuvec_s *position, f32 volume);
extern "C" void SetPreSeekStartPoint(f32 start_point);

void PlayAMusic(i32 a, i32 b, i32 c, i32 d) {
    if (NOSOUND != 0 || NOMUSIC != 0) {
        return;
    }

    NuSound3StopStereoStream(a);
    NuSound3PlayStereoV(NUSOUNDPLAYTOK_STEREOSTREAM, a, NUSOUNDPLAYTOK_SAMPLE, b, NUSOUNDPLAYTOK_VOL, c,
                        d != 0 ? NUSOUNDPLAYTOK_ONESHOT : static_cast<NUSOUNDPLAYTOK>(0), NUSOUNDPLAYTOK_END);
    Music.secondary_stream = static_cast<i16>(a);
    Music.transition_frames = 0;
}
i16 GetMusicIndex(char *name, nusound_filename_info_s *table, i32 def) {
    (void)name;
    (void)table;
    (void)def;
    return -1;
}
void MusicClearAll() {
    MusicPlrsUnderAttack = 0;
    MusicActive = -1;
    sticky_attack_time = 0.0f;
    MusicActiveReset = 0;
    MusicSwapDelay = 0.0f;
    MusicSeekIndex = 0;
    PlayersUnderAttack = 0;
    MusicInside = 0;
    MusicFindCut = 0;
    MusicRestoreTrack = 0;
    MusicOnFlag = 1;
    MusicPlrsHoldAttack = 0;
    CurrentMusicPair_Quiet = -1;
}

void GameAudio_SetActionMusicTimes(f32 initial_delay, f32 hold_time) {
    sticky_attack_timeout[0] = initial_delay;
    sticky_attack_timeout[1] = hold_time;
}
void SpaceAudioPoint() {
    music_man.SetTrackEntryTimeByClass(TRACK_CLASS_ACTION, WORLD->space_level->door_time);
    music_man.SetTrackEntryTimeByClass(TRACK_CLASS_NOMUSIC, WORLD->space_level->door_time);
}
void legoSetCutVolume(float v) {
    music_man.SetClassVolume(TRACK_CLASS_CUTSCENE, v);
}
f32 GetAudioFadeLevel() {
    return AUDIOFADELEVEL;
}
void SetBackgroundMusic(i32 track) {
    // libTTapp.so 0x4df8b0: platform stub (eight NOPs and ret).
    // Title music is selected and started later by GamePlayMusic.
    (void)track;
}
void legoSetMusicVolume(float v) {
    // Original: NuMusic::SetClassVolume(0x23, v) — the mask covers QUIET (1),
    // ACTION (2) and the 0x20 theme class.
    music_man.SetClassVolume(0x23, v);
}
void ProcessMusicChanges(LEVELDATA_s *level, OPTIONSSAVE_s *opts) {
    (void)opts;

    if (GameAudio_ActionMusicFn != NULL) {
        PlayersUnderAttack = GameAudio_ActionMusicFn();
    } else {
        const auto player_under_attack = [](GameObject_s *player) {
            if (player == NULL)
                return false;
            if (player->ai.opponent != NULL)
                return true;
            GameObject_s *opponent = static_cast<GameObject_s *>(player->ai.nearest_opponent);
            return opponent != NULL && opponent->apiobj.field_0x287 == 0 && player->ai.nearest_opponent_metric < 3.0f;
        };
        PlayersUnderAttack = DoubleScore != 0 || Cheat_PowerUpActive(-1) != 0 || player_under_attack(Player[0]) ||
                             player_under_attack(Player[1]);
    }

    MusicOther = CheckMusicOtherFn != NULL ? CheckMusicOtherFn() : 0;

    if (sticky_attack != PlayersUnderAttack) {
        if (Paused == 0) {
            sticky_attack_time -= FRAMETIME;
        }
        if (sticky_attack_time <= 0.0f) {
            sticky_attack = PlayersUnderAttack;
            sticky_attack_time = sticky_attack_timeout[sticky_attack];
        }
    }

    LEVELDATA_s *music_level = level;
    CUTINFO *cut_stop = static_cast<CUTINFO *>(CutStopInfo);
    if (cut_stop != NULL && cut_stop->skip_level != -1) {
        LEVELDATA_s *cut_level = &LDataList[cut_stop->skip_level];
        if ((cut_level->flags & LEVEL_CONFIG_LOADED) != 0) {
            music_level = cut_level;
        }
    }

    music_man.SelectTrackByHandle(TRACK_CLASS_QUIET, music_level->music_tracks[0][MusicOther]);
    music_man.SelectTrackByHandle(TRACK_CLASS_ACTION, music_level->music_tracks[1][MusicOther]);
    music_man.SelectTrackByHandle(TRACK_CLASS_NOMUSIC, music_level->music_tracks[2][MusicOther]);

    if (SuperOptions.music_enabled == 0)
        goto play_nomusic;
    if (sticky_attack != 0) {
        if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1)
            goto play_action;
        if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) == -1)
            goto play_nomusic;
    } else {
        if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) == -1) {
            if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1)
                goto play_action;
            goto play_nomusic;
        }
    }
    music_man.PlayTrack(TRACK_CLASS_QUIET);
    goto process_music;
play_action:
    music_man.PlayTrack(TRACK_CLASS_ACTION);
    goto process_music;
play_nomusic:
    music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
process_music:

    music_man.Process(FRAMETIME);
}
void SpaceResetAudioPoint() {
    for (i32 door = 6; door >= 0; --door) {
        const f32 distance = DogFightDoors.doors[door].distance;
        if ((Player[0] != NULL && Player[0]->sock_position.distance > distance) ||
            (Player[1] != NULL && Player[1]->sock_position.distance > distance)) {
            music_man.SetTrackEntryTimeByClass(TRACK_CLASS_ACTION, DogFightDoors.doors[door].timer);
            music_man.SetTrackEntryTimeByClass(TRACK_CLASS_NOMUSIC, DogFightDoors.doors[door].timer);
            return;
        }
    }
    SetPreSeekStartPoint(0.0f);
}
i32 CheckMusicSwapInstant() {
    // Original Android build is a platform placeholder returning false.
    return 0;
}
void UpdateBackgroundMusic() {
}
extern "C" {
    i32 fake_seeking;

    i32 GetCurPreSeek(void) {
        return Music.requested_track;
    }
    i32 GetCurrentMusicId(void) {
        return Music.current_track;
    }
    i32 GetOppMusicId(void) {
        return Music.queued_track;
    }

    void MusicPreSeek(i32 track) {
        if (NOSOUND != 0 || NOMUSIC != 0 || track < 0 || track >= SFX_MUSIC_COUNT) {
            return;
        }

        const auto play_stream = [track](i32 stream, f32 seek_offset) {
            NuSound3StopStereoStream(stream);
            NuSound3PlayStereoV(NUSOUNDPLAYTOK_STEREOSTREAM, stream, NUSOUNDPLAYTOK_SAMPLE, track, NUSOUNDPLAYTOK_VOL,
                                0, NUSOUNDPLAYTOK_STARTOFFSET, static_cast<f64>(seek_offset), NUSOUNDPLAYTOK_ONESHOT,
                                NUSOUNDPLAYTOK_END);
        };
        const MusicPlaybackState state = Music.state;
        if ((state & ~MUSIC_PLAYBACK_ACTIVE) == MUSIC_PLAYBACK_STOPPED &&
            (track != Music.requested_track || Music.pause_requested)) {
            const i16 transition_frames = Music.transition_frames;
            const i32 primary_stream = Music.primary_stream;
            Music.resume_track = static_cast<i16>(track);
            Music.queued_track = static_cast<i16>(track);
            Music.requested_track = static_cast<i16>(track);
            reinterpret_cast<u8 *>(&Music)[primary_stream + 0x12] = 0;
            if (transition_frames < 64) {
                if (state == MUSIC_PLAYBACK_STOPPED)
                    goto first_play;
                Music.pause_requested = true;
            } else {
            first_play:
                Music.pause_requested = false;
                const f32 seek_offset = Music.seek_offset;
                const i32 stream = 1 - primary_stream;
                play_stream(stream, seek_offset);
                Music.secondary_stream = static_cast<i16>(stream);
                Music.transition_frames = 0;
                Music.seek_offset = 0.0f;
                if (track >= SFX_MUSIC_COUNT) {
                    return;
                }
            }
        }

        if (static_cast<u16>(Music.state - MUSIC_PLAYBACK_DUAL_STREAM) < 3 &&
            (track != Music.current_track || Music.pause_requested)) {
            const i32 primary_stream = Music.primary_stream;
            const i16 transition_frames = Music.transition_frames;
            Music.current_track = static_cast<i16>(track);
            Music.queued_track = static_cast<i16>(track);
            Music.requested_track = static_cast<i16>(track);
            Music.state = MUSIC_PLAYBACK_DUAL_STREAM;
            reinterpret_cast<u8 *>(&Music)[primary_stream + 0x12] = 0;
            if (transition_frames < 64) {
                Music.pause_requested = true;
            } else {
                Music.pause_requested = false;
                const f32 seek_offset = Music.seek_offset;
                if (NOSOUND == 0 && NOMUSIC == 0) {
                    play_stream(primary_stream, seek_offset);
                    Music.secondary_stream = static_cast<i16>(primary_stream);
                }
                Music.transition_frames = 0;
                Music.seek_offset = 0.0f;
            }
        }
    }

    void MusicPreSeekNow(i32 track) {
        if (track < 0 || track >= SFX_MUSIC_COUNT) {
            return;
        }

        const f32 seek_offset = Music.seek_offset;
        const i32 primary_stream = Music.primary_stream;
        Music.queued_track = static_cast<i16>(track);
        Music.requested_track = static_cast<i16>(track);
        Music.pause_requested = false;
        Music.stream_status_delay[primary_stream] = 0;
        if (NOSOUND == 0 && NOMUSIC == 0) {
            const i32 stream = 1 - primary_stream;
            NuSound3StopStereoStream(stream);
            NuSound3PlayStereoV(NUSOUNDPLAYTOK_STEREOSTREAM, stream, NUSOUNDPLAYTOK_SAMPLE, track, NUSOUNDPLAYTOK_VOL,
                                0, NUSOUNDPLAYTOK_STARTOFFSET, static_cast<f64>(seek_offset), NUSOUNDPLAYTOK_ONESHOT,
                                NUSOUNDPLAYTOK_END);
            Music.secondary_stream = static_cast<i16>(stream);
            Music.transition_frames = 0;
        }
        Music.seek_offset = 0.0f;
    }

    i32 MusicSeeking(void) {
        if (fake_seeking != 0 || Music.pause_requested) {
            return 1;
        }
        if (NuSound3GetStereoStreamStatus(0) == NUSOUND_STEREO_STREAM_FINISHED) {
            return 1;
        }
        return NuSound3GetStereoStreamStatus(1) == NUSOUND_STEREO_STREAM_FINISHED;
    }

    i32 MusicState(void) {
        return Music.state;
    }
}

extern "C" void SetSoundVolume(f32);
f32 GameSetMusicVolume(OPTIONSSAVE_s *);

f32 GameSetSoundVolume(OPTIONSSAVE_s *options) {
    f32 volume = (static_cast<f32>(static_cast<u32>(static_cast<u8>(options->field3_0x3))) / 10.0f) *
                 (static_cast<f32>(static_cast<u32>(static_cast<u8>(options->field5_0x5))) / 10.0f);
    SetSoundVolume(volume);
    legoSetCutVolume(0.85f * static_cast<f32>(static_cast<u32>(static_cast<u8>(options->field5_0x5))) / 10.0f);
    return volume;
}

void SetSoundFadeDist(WORLDINFO_s *world, OPTIONSSAVE_s *options) {
    options->field3_0x3 = 8;
    options->field4_0x4 = 6;
    if (SetSoundFadeDistCallBackFn != NULL && SetSoundFadeDistCallBackFn(world) != 0) {
        GameSetSoundVolume(options);
        GameSetMusicVolume(options);
        return;
    }
    if (VehicleArea == 0) {
        nusound_fade_start = 2.0f;
        nusound_fade_end = 15.0f;
    } else {
        nusound_fade_start = 10.0f;
        nusound_fade_end = 80.0f;
    }
    GameSetSoundVolume(options);
    GameSetMusicVolume(options);
}

i32 GamePlayMusic(LEVELDATA_s *level, i32 check, OPTIONSSAVE_s *options) {
    (void)options;
    i32 other = MusicOther;
    last_currentmusicpair_quiet = CurrentMusicPair_Quiet;
    MusicOther = 0;
    if (NOSOUND != 0) {
        last_currentmusicpair_quiet = CurrentMusicPair_Quiet;
        MusicOther = 0;
        return NOSOUND;
    }
    if (NOMUSIC != 0) {
        last_currentmusicpair_quiet = CurrentMusicPair_Quiet;
        MusicOther = 0;
        return NOMUSIC;
    }

    i32 music_other = 0;
    if (CheckMusicOtherFn != NULL) {
        music_other = CheckMusicOtherFn();
    }

    MusicOther = music_other;

    if (check != 0) {
        if (music_other == other)
            return music_other;
    } else {
        sticky_attack_time = 0;
        sticky_attack = PlayersUnderAttack;
    }

    // music_tracks is [class][pair]: the quiet slot of the selected pair,
    // plus the action/ambient slots of the MusicOther pair.
    music_man.SelectTrackByHandle(TRACK_CLASS_QUIET, level->music_tracks[0][music_other]);
    music_man.SelectTrackByHandle(TRACK_CLASS_ACTION, level->music_tracks[1][MusicOther]);
    music_man.SelectTrackByHandle(TRACK_CLASS_NOMUSIC, level->music_tracks[2][MusicOther]);

    if (SuperOptions.music_enabled == 0) {
        return music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    }

    if (sticky_attack != 0) {
        if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_ACTION);
        }
        if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_QUIET);
        }
        return music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    } else {
        if (music_man.GetTrackHandle(TRACK_CLASS_QUIET, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_QUIET);
        }
        if (music_man.GetTrackHandle(TRACK_CLASS_ACTION, NULL) != -1) {
            return music_man.PlayTrack(TRACK_CLASS_ACTION);
        }
        return music_man.PlayTrack(TRACK_CLASS_NOMUSIC);
    }
}
