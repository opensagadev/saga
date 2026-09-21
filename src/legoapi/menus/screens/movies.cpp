#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufpar.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

void Movie_Play(char *, variptr_u *, variptr_u *, float, i32 (*)(), float) {
    STUBBED();
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

static __used__ void Movie_CallBack() {
    STUBBED();
}
