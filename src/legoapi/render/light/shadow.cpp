// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "legoapi/render/light/shadow.h"
#include "decomp.h"
#include "globals.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/items/base/apiobject.h"
#include "nu2api/nu3d/nucamera.h"

void Shadow_SetMode() {
    ShadowMode = 0;
    for (i32 i = 0; i < 8; ++i) {
        GameObject_s *player = Player[i];
        if (player == NULL || player->suit == NULL) {
            continue;
        }

        const SUIT_s *suit = static_cast<const SUIT_s *>(player->suit);
        if ((suit->store_flag & SUIT_STORE_FLAG_SHADOW_MODE) != 0) {
            ShadowMode = 1;
            return;
        }
    }
}

