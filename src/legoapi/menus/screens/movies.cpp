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

char (*MovieList)[32];
i32 MovieCount;

void Movies_ConfigureList(char *path, variptr_u *buf, variptr_u *buf_end) {
    (void)buf_end;
    NUFPAR *parser = NuFParCreate(path);
    if (parser == NULL)
        return;

    MovieCount = 0;
    buf->addr = ALIGN(buf->addr, 4);
    MovieList = reinterpret_cast<char (*)[32]>(buf->addr);
    bool done = false;
    while (!done && NuFParGetLine(parser) != 0) {
        while (NuFParGetWord(parser) != 0) {
            if (NuStrICmp(parser->word_buf, const_cast<char *>("movie")) != 0 || NuFParGetWord(parser) == 0 ||
                NuStrLen(parser->word_buf) > 31) {
                break;
            }
            NuStrCpy(MovieList[MovieCount], parser->word_buf);
            ++MovieCount;
            buf->addr += 32;
            if (NuFParGetLine(parser) == 0) {
                done = true;
                break;
            }
        }
    }
    NuFParDestroy(parser);
    if (MovieCount == 0)
        MovieList = NULL;
}

static __used__ void Movie_CallBack() {
    STUBBED();
}
