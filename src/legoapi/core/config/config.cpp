#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "legoapi/characters/core/charconfig.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

// Game-side character config keywords (retail config.cpp).

static void CC_variant(NUFPAR *parser) {
    if (NuFParGetWord(parser) == 0)
        return;
    if (NuStrICmp(parser->word_buf, "stormtrooper") == 0)
        charconfig.runtime->field275_0x116 = 1;
    else if (NuStrICmp(parser->word_buf, "fett") == 0)
        charconfig.runtime->field275_0x116 = 2;
    else if (NuStrICmp(parser->word_buf, "macewindu") == 0)
        charconfig.runtime->field275_0x116 = 3;
    else if (NuStrICmp(parser->word_buf, "battledroid") == 0)
        charconfig.runtime->field275_0x116 = 4;
    else if (NuStrICmp(parser->word_buf, "hoverdroid") == 0)
        charconfig.runtime->field275_0x116 = 17;
    else if (NuStrICmp(parser->word_buf, "walker2legs") == 0)
        charconfig.runtime->field275_0x116 = 15;
    else if (NuStrICmp(parser->word_buf, "walker4legs") == 0)
        charconfig.runtime->field275_0x116 = 16;
    else if (NuStrICmp(parser->word_buf, "wookiee") == 0)
        charconfig.runtime->field275_0x116 = 5;
    else if (NuStrICmp(parser->word_buf, "obiwankenobi") == 0)
        charconfig.runtime->field275_0x116 = 6;
    else if (NuStrICmp(parser->word_buf, "leia") == 0)
        charconfig.runtime->field275_0x116 = 7;
    else if (NuStrICmp(parser->word_buf, "clone") == 0)
        charconfig.runtime->field275_0x116 = 8;
    else if (NuStrICmp(parser->word_buf, "rebel") == 0)
        charconfig.runtime->field275_0x116 = 9;
    else if (NuStrICmp(parser->word_buf, "tie") == 0)
        charconfig.runtime->field275_0x116 = 10;
    else if (NuStrICmp(parser->word_buf, "lando") == 0)
        charconfig.runtime->field275_0x116 = 11;
    else if (NuStrICmp(parser->word_buf, "weirdo") == 0)
        charconfig.runtime->field275_0x116 = 0;
    else if (NuStrICmp(parser->word_buf, "luke") == 0)
        charconfig.runtime->field275_0x116 = 12;
    else if (NuStrICmp(parser->word_buf, "hansolo") == 0)
        charconfig.runtime->field275_0x116 = 13;
    else if (NuStrICmp(parser->word_buf, "padme") == 0)
        charconfig.runtime->field275_0x116 = 14;
    else if (NuStrICmp(parser->word_buf, "critter") == 0)
        charconfig.runtime->field275_0x116 = 18;
    else if (NuStrICmp(parser->word_buf, "naboostarfighter") == 0)
        charconfig.runtime->field275_0x116 = 19;
    else if (NuStrICmp(parser->word_buf, "pod") == 0)
        charconfig.runtime->field275_0x116 = 20;
    else if (NuStrICmp(parser->word_buf, "republicgunship") == 0)
        charconfig.runtime->field275_0x116 = 21;
    else if (NuStrICmp(parser->word_buf, "henchman") == 0)
        charconfig.runtime->field275_0x116 = 22;
}

NUFPCOMJMP ConfigChar_GameKeywords[] = {
    {"variant", CC_variant},
    {NULL, NULL},
};
