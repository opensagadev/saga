#include "decomp.h"
#include "batman.h"
#include "nu2api/nucore/nuvuvec.hpp"

extern "C" {
    extern char *apitxt_ENGLISH, *apitxt_FRENCH, *apitxt_GERMAN, *apitxt_ITALIAN, *apitxt_SPANISH, *apitxt_DANISH;
}

// Retail keeps the game's language list in its own text.cpp; the names are filled in by its static initializer.
LANGUAGEDATA Game_LanguageList[7] = {{1, apitxt_ENGLISH}, {2, apitxt_FRENCH}, {4, apitxt_GERMAN}, {5, apitxt_ITALIAN},
                                     {3, apitxt_SPANISH}, {8, apitxt_DANISH}, {-1, NULL}};
