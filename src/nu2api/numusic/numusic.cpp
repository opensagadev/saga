// NuMusic - the music player (nu2api.2013/numusic/numusic.cpp).
//
// Decompressed from the original libTTapp.so. The player owns a parsed
// music.cfg "album" table (albums -> tracks), a shared entry-time pool and two
// stereo-stream voices. The boot path for the title music is:
//
//   LoadPermData -> ConfigureMusic -> NuMusic::Initialise("audio\\music.cfg")
//   NuMain -> GamePlayMusic(titles level) -> SelectTrackByHandle x3
//           -> PlayTrack(0x20) -> PlayTrackI -> Voice::Load -> Voice::Play
//           -> NuSound3PlayStereoV (host nusound streamer)
//   per-frame: NuMusic::Process (fades, ducking, volume mixing)
//
// Track handles pack (albumIndex << 11) | trackIndexInAlbum. The original
// constructs/validates them through reciprocal multiplies because
// sizeof(Album) == 36 == 4 * 9 and sizeof(Track) == 60 == 4 * 15; those
// literal forms are kept so the arithmetic matches the original binary.

#include <stdlib.h>
#include <string.h>

#include "decomp.h"

#include "nu2api/nufile/nufpar.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"

extern NUFPCOMJMPCTX top_jmp_tab[11];
extern NUFPCOMJMPCTX track_jmp_tab[15];

// Globals driving GamePlayMusic. The original keeps these in the batman TU.
i32 MusicOther = 0;
i32 PlayersUnderAttack = 0;
i32 (*CheckMusicOtherFn)(void) = NULL;

// i32 GamePlayMusic(LEVELDATA_s *level, i32 check, OPTIONSSAVE_s *options)
//
// Picks the level's music tracks and starts playback. `check` is 0 on the
// boot/init call and 1 when re-invoked from the main loop (where it bails
// out early if the "other" pair hasn't changed). Classes: 1 = quiet,
// 2 = action, 0x20 = plain theme (the title music lives here).

void NuMusic::GlobalParseErrorFn(nufpar_s *param_1) {
    STUBBED();
}

void NuMusic::TrackParseErrorFn(nufpar_s *param_1) {
    STUBBED();
}

