#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nutex.h"

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

GIZACTIONDEFN_s *gizactiondefs;

void RegisterGizActions(GIZACTIONDEFN_s *definitions) {
    gizactiondefs = definitions;
}
