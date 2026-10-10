#include "nu2api/nu3d/android/nuportal_android.h"
#include "nu2api/nu3d/nugscn.h"
#include "globals.h"

#include <string.h>

extern "C" void Initialise_PS(NUGSCN *scene) {
    scene->instance_visibility_flags = PortalVisiFlags;
}

extern "C" void FlagRoomInstancesAsVisible(NUROOM *room, NUGSCN *) {
    for (i32 i = 0; i < room->instance_count; ++i) {
        PortalVisiFlags[room->instance_indices[i] >> 3] =
            PortalVisiFlags[room->instance_indices[i] >> 3] | 1 << (room->instance_indices[i] & 7);
    }
}

extern "C" void SetAllInstancesVisible(NUGSCN *scene) {
    memset(PortalVisiFlags, 0xff, sizeof(PortalVisiFlags));
}

void SetAllInstancesHidden(NUGSCN *) {
    memset(PortalVisiFlags, 0, sizeof(PortalVisiFlags));
}

extern "C" void clipRoomAgainstFrustrum(NUGSCN *scene, NUROOM *room, NUFRUSTRUM *frustum) {
    u32 instance_index;
    i32 clip_result;
    u32 i;

    // Retain the null display-list guard: unseen sphere instances require
    // the display list's visibility and override tables below.
    if (scene->display_list != NULL) {
        if ((scene->display_list->render_buffer & NUDL_SCENE_RENDER_FLAG_CENTER_EXTENT_BOUNDS) != 0) {
            for (i = 0; i < static_cast<u32>(static_cast<i32>(room->instance_count)); ++i) {
                instance_index = room->instance_indices[i];
                u8 *instance_record = scene->instances + instance_index * 0x50;
                (void)instance_record;
                if ((((u8)PortalVisiFlags[instance_index >> 3] >> (instance_index & 7)) & 1) == 0) {
                    if ((((u8)scene->display_list->portal_visibility_overrides[instance_index >> 3] >>
                          (instance_index & 7)) &
                         1) == 0) {
                        if ((scene->display_list->visibility_flags[instance_index] & NUDL_INSTANCE_FLAG_VISIBLE) != 0) {
                            if ((scene->display_list->visibility_flags[instance_index] &
                                 NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) != 0) {
                                PortalVisiFlags[instance_index >> 3] |= 1 << (instance_index & 7);
                            } else {
                                clip_result = NuPortalClipTestBox(&scene->portal_boxes[instance_index].first,
                                                                  &scene->portal_boxes[instance_index].second, frustum);
                                if (clip_result != 0) {
                                    PortalVisiFlags[instance_index >> 3] |= 1 << (instance_index & 7);
                                }
                            }
                        }
                    } else {
                        PortalVisiFlags[instance_index >> 3] |= 1 << (instance_index & 7);
                    }
                }
            }
        } else {
            for (i = 0; i < static_cast<u32>(static_cast<i32>(room->instance_count)); ++i) {
                instance_index = room->instance_indices[i];
                u8 *instance_record = scene->instances + instance_index * 0x50;
                (void)instance_record;
                if ((((u8)PortalVisiFlags[instance_index >> 3] >> (instance_index & 7)) & 1) == 0) {
                    if ((((u8)scene->display_list->portal_visibility_overrides[instance_index >> 3] >>
                          (instance_index & 7)) &
                         1) == 0) {
                        if ((scene->display_list->visibility_flags[instance_index] &
                             NUDL_INSTANCE_FLAG_NO_VISIBILITY_TEST) != 0) {
                            PortalVisiFlags[instance_index >> 3] |= 1 << (instance_index & 7);
                        } else {
                            clip_result = clipTestSphere(&scene->portal_spheres[instance_index], frustum);
                            switch (clip_result) {
                                case 1:
                                    PortalVisiFlags[instance_index >> 3] |= 1 << (instance_index & 7);
                                    break;
                                case 2:
                                    clip_result = clipTestBox(&scene->portal_boxes[instance_index].first,
                                                              &scene->portal_boxes[instance_index].second,
                                                              frustum->planes, static_cast<i32>(frustum->plane_count));
                                    if (clip_result != 0) {
                                        PortalVisiFlags[instance_index >> 3] |= 1 << (instance_index & 7);
                                    }
                                    break;
                                default:
                                    break;
                            }
                        }
                    } else {
                        PortalVisiFlags[instance_index >> 3] |= 1 << (instance_index & 7);
                    }
                }
            }
        }
    }
}
