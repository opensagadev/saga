// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "decomp.h"
#include "globals.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/gizmo/base/gizmo.h"
#include "legoapi/gizmos/object/newblowup.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nucore/nustring.h"

#include <string.h>
#include "legoapi/gizmo/object/gizmoblowups.h"

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

void ResetRadios() {
    memset(radios, 0, sizeof(radios));
    radios_playing = 0;
}

void UpdateRadios() {
    if (radios_playing == 0) {
        return;
    }

    radios_playing = 0;
    for (RadioEntry *radio = radios; radio != &radios[8]; radio++) {
        if (!(radio->time > 0.0f)) {
            continue;
        }

        radio->time -= FRAMETIME;
        if (radio->time < 0.0f) {
            radio->time = 0.0f;
        } else {
            radios_playing = 1;
        }

        f32 phase = NuFmod(radio->time, 0.5f);
        i32 angle = static_cast<i32>(phase * 65536.0f);
        if (radio->blowup != NULL) {
            GizmoBlowupUpdateMatrix(radio->blowup);
            f32 scale_value = NuTrigTable[(angle >> 1) & 0x7fff] * 0.1f + 1.0f;
            NUVEC scale = {scale_value, scale_value, scale_value};
            NuMtxPreScale(&radio->blowup->transform, &scale);
            radio->blowup->state_flags |= 1;
            PlaySfx("swdisco", reinterpret_cast<NUVEC *>(&radio->blowup->transform.m30));
        } else {
            if (NuSpecialExistsFn(&radio->special) == 0) {
                continue;
            }
            NUMTX *source = NuSpecialGetMtx(&radio->special);
            NUMTX *matrix = NuSpecialGetDrawMtx(&radio->special);
            *matrix = *source;
            if (angle != 0) {
                f32 scale_value = NuTrigTable[(angle >> 1) & 0x7fff] * 0.1f + 1.0f;
                NUVEC scale = {scale_value, scale_value, scale_value};
                NuMtxPreScale(matrix, &scale);
                NuSpecialUpdate(&radio->special);
            }
            PlaySfx("swdisco", reinterpret_cast<NUVEC *>(&matrix->m30));
        }
    }
}

void PlayRadio(char *special_name, char *blowup_name, i32 play) {
    if (!(special_name == NULL && blowup_name == NULL)) {
        if (play != 0) {
            for (i32 i = 0; i < 8; i++) {
                RadioEntry *radio = &radios[i];
                if (radio->time <= 0.0f) {
                    if (blowup_name != NULL) {
                        GIZMO *gizmo = GizmoFindByName(WORLD->gizmo_sys, blowup_gizmotype_id, blowup_name);
                        if (gizmo == NULL) {
                            return;
                        }
                        radio->blowup = static_cast<GIZMOBLOWUP_s *>(gizmo->object);
                    } else if (special_name == NULL ||
                               NuSpecialFind(WORLD->current_gscn, &radio->special, special_name, 1) == 0) {
                        return;
                    }
                    radio->time = 20.0f;
                    radios_playing = 1;
                    return;
                }
            }
            return;
        }

        if (blowup_name == NULL) {
            return;
        }
        for (i32 i = 0; i < 8; i++) {
            RadioEntry *radio = &radios[i];
            if (radio->blowup != NULL && NuStrICmp(radio->blowup->name, blowup_name) == 0) {
                GizmoBlowupUpdateMatrix(radio->blowup);
                radio->blowup->state_flags |= 1;
                memset(radio, 0, sizeof(*radio));
                return;
            }
            if (special_name != NULL) {
                char *name = NuSpecialGetName(&radio->special);
                if (name != NULL && NuStrICmp(name, special_name) == 0) {
                    NUMTX *source = NuSpecialGetMtx(&radio->special);
                    NUMTX *instance = NuSpecialGetInstanceMtx(&radio->special);
                    *instance = *source;
                    NuSpecialUpdate(&radio->special);
                    memset(radio, 0, sizeof(*radio));
                    return;
                }
            }
        }
    }
}

