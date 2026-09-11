#include "decomp.h"
#include "legoapi/legoapi_types.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"

extern "C" {
    extern i16 id_GRABMACHINE;
    extern i16 id_GRABMAGNET;
    extern i16 id_ROBOTBASE;
}

GRABBER_s *Grab_grabber;

static __used__ void Grab_invert_x(nufpar_s *) {
    Grab_grabber->invert_x = 1;
}
static __used__ void Grab_move_xy(nufpar_s *parser) {
    if (NuFParGetInt(parser) != 0) {
        Grab_grabber->move_xy = 1;
    }
}
static __used__ void Grab_radius(nufpar_s *parser) {
    Grab_grabber->radius = NuFParGetFloat(parser);
}
static __used__ void Grab_rotate(nufpar_s *parser) {
    GRABBER_s *grabber = Grab_grabber;
    i32 rotate = NuFParGetInt(parser) & 1;
    grabber->flags_559 = (grabber->flags_559 & ~0x08) | (rotate << 3);
}
static __used__ void Grab_scale(nufpar_s *parser) {
    Grab_grabber->scale = NuFParGetFloat(parser);
}
static __used__ void Grab_shadow(nufpar_s *parser) {
    Grab_grabber->flags_559 |= 0x10;
    if (NuFParGetWord(parser) != 0 && NuStrICmp(parser->word_buf, const_cast<char *>("off")) == 0) {
        Grab_grabber->flags_559 &= ~0x10;
    }
}
static __used__ void Grab_speed(nufpar_s *parser) {
    Grab_grabber->speed = NuFParGetFloat(parser);
}
static __used__ void Grab_type(nufpar_s *parser) {
    if (NuFParGetWord(parser) == 0) {
        return;
    }
    if (NuStrICmp(parser->word_buf, const_cast<char *>("GRABMAGNET")) == 0) {
        Grab_grabber->character_id = id_GRABMAGNET;
    } else if (NuStrICmp(parser->word_buf, const_cast<char *>("ROBOTBASE")) == 0) {
        Grab_grabber->character_id = id_ROBOTBASE;
    } else {
        Grab_grabber->character_id = id_GRABMACHINE;
    }
}
