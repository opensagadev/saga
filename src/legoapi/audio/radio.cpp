#include "decomp.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>

struct AIROW_s;
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;

struct RadioEntry {
    union {
        GIZMOBLOWUP_s *blowup;
        struct {
            void *empty;
            nuhspecial_s special;
        };
    };
    f32 time;
};
DECOMP_ASSERT(sizeof(RadioEntry) == 0x14, "RadioEntry size");

static RadioEntry radios[8];

void GizmoBlowupUpdateMatrix(GIZMOBLOWUP_s *blowup);

void ResetRadios() {
    memset(radios, 0, sizeof(radios));
    radios_playing = 0;
}

void UpdateRadios() {
    if (radios_playing == 0) {
        return;
    }

    radios_playing = 0;
    for (i32 i = 0; i < 8; i++) {
        RadioEntry *radio = &radios[i];
        if (radio->time <= 0.0f) {
            continue;
        }

        radio->time -= FRAMETIME;
        if (radio->time >= 0.0f) {
            radios_playing = 1;
        } else {
            radio->time = 0.0f;
        }

        f32 phase = NuFmod(radio->time, 0.5f);
        i32 angle = static_cast<i32>(phase * 65536.0f) >> 1;
        NUMTX *matrix;
        if (radio->blowup == NULL) {
            if (NuSpecialExistsFn(&radio->special) == 0) {
                continue;
            }
            *NuSpecialGetDrawMtx(&radio->special) = *NuSpecialGetMtx(&radio->special);
            matrix = NuSpecialGetDrawMtx(&radio->special);
            if (static_cast<i32>(phase * 65536.0f) != 0) {
                f32 scale_value = NuTrigTable[angle & 0x7fff] * 0.1f + 1.0f;
                NUVEC scale = {scale_value, scale_value, scale_value};
                NuMtxPreScale(matrix, &scale);
                NuSpecialUpdate(&radio->special);
            }
        } else {
            GizmoBlowupUpdateMatrix(radio->blowup);
            f32 scale_value = NuTrigTable[angle & 0x7fff] * 0.1f + 1.0f;
            NUVEC scale = {scale_value, scale_value, scale_value};
            matrix = &radio->blowup->transform;
            NuMtxPreScale(matrix, &scale);
            radio->blowup->state_flags |= 1;
        }
        PlaySfx("swdisco", reinterpret_cast<NUVEC *>(&matrix->m30));
    }
}

void PlayRadio(char *special_name, char *blowup_name, i32 play) {
    if (special_name == NULL && blowup_name == NULL) {
        return;
    }

    if (play != 0) {
        RadioEntry *radio = NULL;
        for (i32 i = 0; i < 8; i++) {
            if (radios[i].time <= 0.0f) {
                radio = &radios[i];
                break;
            }
        }
        if (radio == NULL) {
            return;
        }

        if (blowup_name != NULL) {
            GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, blowup_name);
            if (gizmo == NULL) {
                return;
            }
            radio->blowup = static_cast<GIZMOBLOWUP_s *>(gizmo->object);
        } else {
            if (special_name == NULL || NuSpecialFind(WORLD->current_gscn, &radio->special, special_name, 1) == 0) {
                return;
            }
        }
        radio->time = 20.0f;
        radios_playing = 1;
        return;
    }

    if (blowup_name == NULL) {
        return;
    }
    for (i32 i = 0; i < 8; i++) {
        RadioEntry *radio = &radios[i];
        bool match = radio->blowup != NULL && NuStrICmp(radio->blowup->name, blowup_name) == 0;
        if (!match && special_name != NULL) {
            char *name = NuSpecialGetName(&radio->special);
            match = name != NULL && NuStrICmp(name, special_name) == 0;
        }
        if (!match) {
            continue;
        }

        if (radio->blowup != NULL) {
            GizmoBlowupUpdateMatrix(radio->blowup);
            radio->blowup->state_flags |= 1;
        } else {
            *NuSpecialGetInstanceMtx(&radio->special) = *NuSpecialGetMtx(&radio->special);
            NuSpecialUpdate(&radio->special);
        }
        memset(radio, 0, sizeof(*radio));
        return;
    }
}
