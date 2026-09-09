#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/gizmos/object/gizpanel.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/world/world_shared.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numusic/sfx.h"

extern NUVEC nusound_special_positions[5];
extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);

extern "C" f32 GIZPANEL_PLAYERPOSLIFT;
extern "C" {
    i32 DeletePlatinst(i32 platform_id);
    i16 NewPlatPickupInst(void *object, i32 object_type);
    void PlatInstRotate(i32 platform_id, i32 enabled);
}

f32 GameShadow(GameObject_s *object, NUVEC *position, f32 probe_height, i32 terrain_mask);
void FindAnglesZX(NUVEC *normal, u16 *x_rotation, u16 *z_rotation);
i32 SuperWeirdo(GameObject_s *object);
extern i16 id_DARTHVADER, id_GRANDMOFFTARKIN, id_IMPERIALOFFICER, id_IMPERIALSHUTTLEPILOT;
void GizPanel_GetAbsTargetPos(GIZPANEL_s *panel, nuvec_s *target_position, i32 player_position);

static __used__ void GizPanel_CreateTerrain(GIZPANEL_s *panel) {
    if (panel == NULL) {
        return;
    }
    if (panel->platform_id != -1) {
        DeletePlatinst(panel->platform_id);
    }

    switch (panel->model_variant) {
        case 0:
            panel->platform_id = NewPlatPickupInst(panel, 4);
            break;
        case 1:
            panel->platform_id = NewPlatPickupInst(panel, 7);
            break;
        case 2:
            panel->platform_id = NewPlatPickupInst(panel, 5);
            break;
        case 3:
            panel->platform_id = NewPlatPickupInst(panel, 6);
            break;
        default:
            break;
    }
    PlatInstRotate(panel->platform_id, 1);
}

void GizPanel_Use(GameObject_s &, GIZPANEL_s &) {
}

void GizPanel_Reset(GIZPANEL_s *panel) {
    NUVEC *floor_position = &panel->floor_position;
    floor_position->y = 0.0f;
    floor_position->x = 0.0f;
    floor_position->z = 0.4f;
    NuVecRotateY(floor_position, floor_position, static_cast<u16>(panel->y_rotation + 0x8000));
    NuVecAdd(floor_position, floor_position, &panel->position);

    NUVEC target_position;
    GizPanel_GetAbsTargetPos(panel, &target_position, 0);
    target_position.y = panel->position.y;
    floor_position->y = GameShadow(NULL, floor_position, 0.2f, -1);
    const f32 target_floor = GameShadow(NULL, &target_position, 0.2f, -1);
    panel->target_offset.y = target_floor;
    if (target_floor != -1.0f) {
        panel->target_offset.y = target_floor + GIZPANEL_PLAYERPOSLIFT;
        FindAnglesZX(&ShadNorm, &panel->target_pitch, &panel->target_roll);
    }

    panel->activation_time = 0.0f;
    panel->arm_x_rotation = 0;
    panel->flags =
        static_cast<GIZPANEL_FLAGS>((panel->flags & 0xfc) | GIZPANEL_FLAG_VISIBLE | GIZPANEL_FLAG_TRACK_PLAYER);
    NuMtxSetRotationY(&panel->matrix, panel->y_rotation);
    NuMtxTranslate(&panel->matrix, &panel->position);
}

void GizPanel_PlaySfx(char *name, nuvec_s *position, i32 player_bits) {
    if (position == NULL || name == NULL)
        return;
    const i16 sfx_id = static_cast<i16>(GetSfxId(name));
    if (sfx_id == -1)
        return;
    if (player_bits == 0) {
        PlaySfxById(sfx_id, position);
    } else {
        if ((player_bits & 1) != 0) {
            nusound_special_positions[1] = *position;
            PlaySfxById(sfx_id, &nusound_special_positions[1]);
            nusound_special_positions[1] = nusound_special_positions[0];
        }
        if ((player_bits & 2) != 0) {
            nusound_special_positions[2] = *position;
            PlaySfxById(sfx_id, &nusound_special_positions[2]);
            nusound_special_positions[2] = nusound_special_positions[0];
        }
    }
}

void GizPanel_MoveCode(WORLDINFO_s *, GameObject_s *, i32) {
}

i32 GizPanel_BeingUsed(GIZPANEL_s *panel) {
    return panel->flags & 1;
}

GIZPANEL_s *GizPanel_FindByName(WORLDINFO_s *world, char *name) {
    if (world != NULL && world->giz_panel_sys != NULL) {
        GIZPANEL_s *panel = world->giz_panel_sys->panels;
        for (i32 index = 0; index < world->giz_panel_sys->count; ++index, ++panel) {
            if (NuStrICmp(panel->name, name) == 0)
                return panel;
        }
    }
    return NULL;
}

void GizPanel_UpdateHint(HINT_s *) {
}

i32 GizPanel_CanUsePanel(GameObject_s *object, GIZPANEL_s *panel) {
    if (panel == NULL || object == NULL)
        return 0;
    if (SuperWeirdo(object) != 0)
        return 1;
    switch (panel->model_variant) {
        case 0:
            return (object->apiobj.character_data->model_flags & 0x40) != 0 ||
                   (object->apiobj.character_data->model_flags & 0x1000010) == 0x1000010;
        case 1:
            return (object->apiobj.character_data->model_flags & 0x20) != 0 ||
                   (object->apiobj.character_data->model_flags & 0x1000010) == 0x1000010;
        case 2:
            return (object->apiobj.character_data->model_flags & 0x1000000) != 0 || object->field_0x108e == 6;
        case 3:
            if (object->field_0x108e == 5 ||
                (static_cast<GAMECHARACTERDATA *>(object->apiobj.character_data->field11_0x24)->flags_090 & 0x40000) !=
                    0)
                return 1;
            if (GCDataList[object->id].uses_weapon_action == 8 || GCDataList[object->id].uses_weapon_action == 1)
                return 1;
            return object->id == id_DARTHVADER || object->id == id_THEEMPEROR || object->id == id_GRANDMOFFTARKIN ||
                   object->id == id_IMPERIALOFFICER || object->id == id_IMPERIALSHUTTLEPILOT;
        default:
            return 0;
    }
}

GIZPANEL_s *GizPanel_FindNearest(WORLDINFO_s *world, nuvec_s *position, GameObject_s *object, float *distance_squared,
                                 i32 check_eligibility) {
    if (world == NULL || world->giz_panel_sys == NULL)
        return NULL;
    GIZPANEL_s *nearest = NULL;
    f32 nearest_distance = 1.0e9f;
    for (i32 index = 0; index < world->giz_panel_sys->count; ++index) {
        GIZPANEL_s *panel = &world->giz_panel_sys->panels[index];
        f32 distance;
        if (object != NULL) {
            if ((panel->flags & 0x0f) != 0x0c || panel->floor_position.y == -1.0f)
                continue;
            if (check_eligibility != 0 && GizPanel_CanUsePanel(object, panel) == 0)
                continue;
            NUVEC target;
            GizPanel_GetAbsTargetPos(panel, &target, 0);
            distance = NuVecDistSqr(position, &target, NULL);
        } else {
            distance = NuVecDistSqr(position, &panel->position, NULL);
        }
        if (distance < nearest_distance) {
            nearest_distance = distance;
            nearest = panel;
        }
    }
    if (distance_squared != NULL)
        *distance_squared = nearest_distance;
    return nearest;
}

void GizPanel_InitTerrain(WORLDINFO_s *world) {
    GIZPANELSYS_s *panel_sys = world->giz_panel_sys;
    if (panel_sys == NULL || panel_sys->count <= 0) {
        return;
    }

    for (i32 index = 0; index < panel_sys->count; ++index) {
        GIZPANEL_s *panel = &panel_sys->panels[index];
        panel->platform_id = -1;
        GizPanel_CreateTerrain(panel);
        panel_sys = world->giz_panel_sys;
    }
}

void GizPanel_GetAbsPlayerPos(GIZPANEL_s *panel, nuvec_s *position) {
    if (position != NULL) {
        if (panel != NULL && static_cast<u8>(panel->model_variant - 2) <= 1)
            GizPanel_GetAbsTargetPos(panel, position, 0);
        else
            *position = panel->floor_position;
    }
}

void GizPanel_GetAbsTargetPos(GIZPANEL_s *panel, nuvec_s *target_position, i32 player_position) {
    if (target_position == NULL || panel == NULL) {
        return;
    }

    NUVEC offset;
    if (player_position != 0) {
        if (panel->model_variant == 0) {
            offset = {-0.04f, panel->target_offset.y, -0.3f};
        } else if (panel->model_variant == 1) {
            offset = {0.035f, panel->target_offset.y, -0.25f};
        } else {
            offset = panel->target_offset;
        }
    } else {
        offset = panel->target_offset;
    }

    NuVecRotateY(&offset, &offset, panel->y_rotation);
    offset.x += panel->position.x;
    offset.z += panel->position.z;
    *target_position = offset;
}

void GIZPANEL_s::ClearMechObjectInterface() {
}

void GIZPANEL_s::GetMechObjectInterface() {
}
