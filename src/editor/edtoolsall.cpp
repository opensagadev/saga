#include "decomp.h"
#include "editor/edpath.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/items/base/collection.h"
#include "nu2api/nucore/nulist.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/nutex.h"
#include <stdio.h>
#include <string.h>

struct AIROW_s;
struct nunativegscene_s;
struct EDCREATURE_s;
i32 CanWearHatsInFreePlay(i32);

extern "C" {
    i32 aisys_maxnumcreaturesets = 32;
}

i32 InModelListDataFlags(APICHARACTERMODELLIST_s *models, u32 model_flags, u32 game_flags, i32 require_hats,
                         i32 reject_flag_40) {
    if (models == nullptr)
        return 0;
    const i16 first_id = models->model_id;
    if (first_id == -1)
        return 0;
    const u32 reject_flag_80 = model_flags & 8;
    if (__builtin_expect(require_hats == 0, 0)) {
        if (reject_flag_40 != 0) {
            if (reject_flag_80 != 0) {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (Collection_Got(id) == 0)
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    if ((game_data.flags_094[1] & 0x40) != 0)
                        continue;
                    if (static_cast<i8>(game_data.flags_094[1]) < 0)
                        continue;
                    return 1;
                }
                return 0;
            } else {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (Collection_Got(id) == 0)
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    if ((game_data.flags_094[1] & 0x40) != 0)
                        continue;
                    return 1;
                }
                return 0;
            }
        } else {
            if (reject_flag_80 != 0) {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (Collection_Got(id) == 0)
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    if (__builtin_expect(!(static_cast<i8>(game_data.flags_094[1]) < 0), 1)) {
                        return 1;
                    }
                }
                return 0;
            } else {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (Collection_Got(id) == 0)
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    return 1;
                }
                return 0;
            }
        }
    } else {
        if (reject_flag_80 == 0) {
            if (__builtin_expect(reject_flag_40 != 0, 0)) {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (Collection_Got(id) == 0)
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    if ((game_data.flags_094[1] & 0x40) != 0)
                        continue;
                    if (CanWearHatsInFreePlay(current_id) != 0) {
                        return 1;
                    }
                }
                return 0;
            } else {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (__builtin_expect(Collection_Got(id) == 0, 0))
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    if (CanWearHatsInFreePlay(current_id) == 0)
                        continue;
                    return 1;
                }
                return 0;
            }
        } else {
            if (reject_flag_40 != 0) {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (__builtin_expect(Collection_Got(id) == 0, 0))
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    if ((game_data.flags_094[1] & 0x40) != 0)
                        continue;
                    if (static_cast<i8>(game_data.flags_094[1]) < 0)
                        continue;
                    if (CanWearHatsInFreePlay(current_id) == 0)
                        continue;
                    return 1;
                }
                return 0;
            } else {
                for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
                    if (__builtin_expect(Collection_Got(id) == 0, 0))
                        continue;
                    const i32 current_id = models->model_id;
                    const GAMECHARACTERDATA &game_data = GCDataList[current_id];
                    if ((game_data.flags_090 & game_flags) != game_flags ||
                        (CDataList[current_id].model_flags & model_flags) != model_flags)
                        continue;
                    if (static_cast<i8>(game_data.flags_094[1]) < 0)
                        continue;
                    if (CanWearHatsInFreePlay(current_id) == 0)
                        continue;
                    return 1;
                }
                return 0;
            }
        }
    }
}

#include "editor/edtoolsall_types.h"

