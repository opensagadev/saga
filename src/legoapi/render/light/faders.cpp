#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nuvec.h"
#include "globals.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

// Retail realigns this stack frame before copying the matrix local.
void __attribute__((force_align_arg_pointer)) Faders_Draw(WORLDINFO_s *world) {
    for (i32 i = 0; world->faders != NULL && i < world->fader_count; ++i) {
        FADER_s &fader = world->faders[i];
        NUMTX matrix = *NuSpecialGetDrawMtx(&fader.special);
        f32 alpha = 0.0f;
        if (NuSpecialExistsFn(&fader.while_animating) == 0) {
            f32 distance =
                NuVecDist(reinterpret_cast<NUVEC *>(&matrix.m30), reinterpret_cast<NUVEC *>(&pNuCam->mtx.m30), NULL);
            if (distance >= 2.0f) {
                alpha = 1.0f;
            } else if (distance >= 0.5f) {
                alpha = (distance - 0.5f) / 1.5f;
            }
        } else {
            alpha = 1.0f - NuSpecialGetAnimPos(&fader.while_animating);
        }
        if (alpha > 0.0f) {
            NuSpecialDrawAtAlpha(&fader.special, &matrix, alpha);
        }
    }
}

void Faders_Reset(WORLDINFO_s *world) {
    if (world->faders != NULL && world->fader_count > 0) {
        for (i32 i = 0; i < world->fader_count; ++i) {
            NuSpecialSetVisibility(&world->faders[i].special, 0);
        }
    }
}

void Faders_Configure(WORLDINFO_s *world, char *config) {
    world->faders = NULL;
    world->fader_count = 0;
    if (world->current_gscn == NULL) {
        return;
    }
    NUFPAR *parser = NuFParCreateMem(const_cast<char *>("faders"), config, 0xffff);
    if (parser == NULL) {
        return;
    }
    world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
    world->faders = reinterpret_cast<FADER_s *>(world->giz_buffer.addr);
    FADER_s *fader = world->faders;
    while (NuFParGetLine(parser) != 0) {
        NuFParGetWord(parser);
        if (NuStrICmp(parser->word_buf, const_cast<char *>("fader")) != 0) {
            continue;
        }
        fader->special.scene = NULL;
        fader->special.special = NULL;
        fader->special.display_special = NULL;
        fader->while_animating.scene = NULL;
        fader->while_animating.special = NULL;
        fader->while_animating.display_special = NULL;
        if (NuFParGetWord(parser) == 0 ||
            NuSpecialFind(world->current_gscn, &fader->special, parser->word_buf, 1) == 0) {
            continue;
        }
        while (NuFParGetWord(parser) != 0) {
            if (NuStrICmp(parser->word_buf, const_cast<char *>("while_animating")) == 0 && NuFParGetWord(parser) != 0) {
                NuSpecialFind(world->current_gscn, &fader->while_animating, parser->word_buf, 1);
            }
        }
        ++world->fader_count;
        ++fader;
    }
    NuFParDestroy(parser);
    if (world->fader_count == 0) {
        world->faders = NULL;
    } else {
        world->giz_buffer.addr = reinterpret_cast<usize>(fader);
    }
}
