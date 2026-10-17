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
struct nuqthdr_s;
struct nunativegscene_s;
struct SHOPINPUT;
struct EDCREATURE_s;
i32 creatureEditor_CalculatePos(EDCREATURE_s *, i32, nuvec_s *, i32);
i32 creatureEditor_IsSelectable(EDCREATURE_s *);
i32 CanWearHatsInFreePlay(i32);

extern "C" {
    extern void *ed_fnt;
    extern i32 AIEDITOR_ANTINODES;
    i32 aisys_maxnumcreaturesets = 32;
    void aieditor_cbCancelMainMenu(eduimenu_s *, eduimenu_s *);
    void aieditor_cvSelectEditorMode(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSave(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbGoToPlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbMovePlayer(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbStopPlatformsToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSnapHeightToggle(eduimenu_s *, eduiitem_s *, u32);
    void aieditor_cbSolidAntinodeDisplayToggle(eduimenu_s *, eduiitem_s *, u32);
    void pathEditorDrawPaths();
    void creatureEditor_RenderAllCreatures();
    void areaEditorDrawAreas();
    void locatorEditorDrawLocators();
    void antinodeEditorDrawAntinodes();
    void AiRndrLine3d(NURND_VERTEX3D *, numtl_s *, NUMTX *);
}

void routeEditor_Render(i32 x, i32 y, float xscale, float yscale);

template <bool RequireHats, bool RejectFlag40, bool RejectFlag80>
static __attribute__((always_inline)) inline i32 FindModelListDataFlags(APICHARACTERMODELLIST_s *models,
                                                                        u32 model_flags, u32 game_flags, i16 first_id) {
    for (i16 id = first_id; id != -1; ++models, id = models->model_id) {
        if (Collection_Got(id) == 0)
            continue;
        const i32 current_id = models->model_id;
        const GAMECHARACTERDATA &game_data = GCDataList[current_id];
        if ((game_data.flags_090 & game_flags) != game_flags ||
            (CDataList[current_id].model_flags & model_flags) != model_flags)
            continue;
        if (RejectFlag40 && (game_data.flags_094[1] & 0x40) != 0)
            continue;
        if (RejectFlag80 && static_cast<i8>(game_data.flags_094[1]) < 0)
            continue;
        if (RequireHats && CanWearHatsInFreePlay(current_id) == 0)
            continue;
        return 1;
    }
    return 0;
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

void antinodeEditor_Render(i32 x, i32 y, float xscale, float yscale);

void creatureEditor_Render(i32 x, i32 y, float xscale, float yscale);

#include "editor/edtoolsall_types.h"

__attribute__((force_align_arg_pointer)) i32 creatureEditor_CalculatePos(EDCREATURE_s *creature, i32 index,
                                                                         nuvec_s *position, i32 check_path);

i32 creatureEditor_IsSelectable(EDCREATURE_s *creature);
