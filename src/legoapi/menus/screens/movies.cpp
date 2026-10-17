#include "legoapi/menus/screens/movies.h"
#include "globals.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nu3d/android/nufmv_android.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nusound/nusound.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

static i32 movie_skipped;
static f32 MoviePlayTime;
static f32 MovieFrameTime;
static i32 (*MovieInputFn)();

static i32 Movie_CallBack() {
    i32 skipped = MovieInputFn != NULL ? MovieInputFn() : 0;
    if (skipped != 0) {
        skipped = MoviePlayTime >= 0.2f;
        if (skipped != 0) {
            movie_skipped = 1;
        }
    }
    MoviePlayTime += MovieFrameTime;
    return skipped;
}

static char (*MovieList)[32];
static i32 MovieCount;

void Movies_ConfigureList(char *path, variptr_u *buf, variptr_u *buf_end) {
    (void)buf_end;
    NUFPAR *parser = NuFParCreate(path);
    if (parser == NULL) {
        return;
    }

    MovieCount = 0;
    buf->addr = ALIGN(buf->addr, 4);
    MovieList = reinterpret_cast<char (*)[32]>(buf->addr);
    char *destination = reinterpret_cast<char *>(buf->addr);
    while (NuFParGetLine(parser) != 0) {
        NuFParGetWord(parser);
        while (NuStrICmp(parser->word_buf, const_cast<char *>("movie")) == 0) {
            if (NuFParGetWord(parser) == 0 || NuStrLen(parser->word_buf) > 31) {
                goto next_line;
            }
            NuStrCpy(destination, parser->word_buf);
            destination += 32;
            ++MovieCount;
            if (NuFParGetLine(parser) == 0) {
                goto done;
            }
            NuFParGetWord(parser);
        }
    next_line:;
    }
done:
    NuFParDestroy(parser);
    if (MovieCount == 0) {
        MovieList = NULL;
    } else {
        buf->void_ptr = destination;
    }
}

i32 Movie_Play(char *name, VARIPTR *buffer, VARIPTR *buffer_end, f32 frame_time, i32 (*input_fn)(), f32 volume) {
    char path[256];
    char subtitles[256];
    movie_skipped = 0;
    NuStrCpy(path, "movies\\");
    if (PAL != 0) {
        NuStrCat(path, "pal\\");
    } else {
        NuStrCat(path, "ntsc\\");
    }
    NuStrCat(path, name);
    NuStrCpy(subtitles, path);
    NuStrCat(subtitles, ".sub");
    NuStrCat(path, ".pss");
    if (NOSOUND == 0) {
        NuSound3KillAllAudio();
    }
    f32 movie_volume = volume;
    MovieInputFn = input_fn;
    MoviePlayTime = 0.0f;
    MovieFrameTime = frame_time;
    if (MovieInputFn == NULL) {
        MovieInputFn = GamePads_SkipMovie;
    }
    if (buffer_end == NULL || buffer == NULL) {
        return 0;
    }
    if (NuFmvPlayV(2, path, 3, 4, 2, 7, Movie_CallBack, 10, 0, 14, &movie_volume, 8, buffer, buffer_end->void_ptr, 5,
                   subtitles, 1) == 0) {
        return 0;
    }
    return movie_skipped != 0 ? 1 : 2;
}
