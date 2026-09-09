#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/numath/numtx.h"

typedef struct nuviewport_s {
    i32 x;
    i32 y;
    i32 width;
    i32 height;
    f32 min_z;
    f32 max_z;
    f32 center_x;
    f32 center_y;
    f32 clip_min_x;
    f32 clip_min_y;
    f32 clip_max_x;
    f32 clip_max_y;
    f32 clip_width;
    f32 clip_height;
    f32 scissor_width;
    f32 scissor_height;
} NUVIEWPORT;

typedef struct nuviewport2_s {
    f32 x;
    f32 y;
    f32 width;
    f32 height;
    f32 min_z;
    f32 max_z;
    f32 center_x;
    f32 center_y;
    f32 clip_min_x;
    f32 clip_min_y;
    f32 clip_max_x;
    f32 clip_max_y;
    f32 clip_width;
    f32 clip_height;
    f32 scissor_width;
    f32 scissor_height;
} NUVIEWPORT2;

typedef struct nuvpregion_s {
    f32 source_x;
    f32 source_y;
    f32 source_width;
    f32 source_height;
    f32 dest_x;
    f32 dest_y;
    f32 dest_width;
    f32 dest_height;
    f32 width_scale;
    f32 height_scale;
    f32 x_offset;
    f32 y_offset;
    f32 projection_x_scale;
    f32 projection_y_scale;
    f32 projection_x_offset;
    f32 projection_y_offset;
} NUVPREGION;

#ifdef __cplusplus

void NuPs2GetViewport(NUVIEWPORT *vp);
void NuVpSetScalingMtx(void);

extern NUVPREGION g_NuVpRegion;
extern i32 PS2_VREZ_H;
extern i32 PS2_VREZ_W;
extern i32 PS2_VCNTR_X;
extern i32 PS2_VCNTR_Y;

extern "C" {
#endif

    void NuVpRestore(void);
    NUVIEWPORT *NuVpGetCurrentViewport(void);
    void NuVpGetCurrent(NUVIEWPORT *viewport);
    void NuVpSetCurrent(NUVIEWPORT *viewport);
    void NuVpSetCurrent2(NUVIEWPORT2 *viewport);
    void NuVpGetCurrent2(NUVIEWPORT2 *viewport);
    void NuVpGetRegions(f32 *source_x, f32 *source_y, f32 *source_right, f32 *source_bottom, f32 *dest_x, f32 *dest_y,
                        f32 *dest_right, f32 *dest_bottom);
    void NuVpSetCentre(f32 x, f32 y);
    void NuVpSetZRange(f32 minimum, f32 maximum);
    void NuVpSetClipping(f32 left, f32 top, f32 right, f32 bottom);
    f32 NuVpPixelWidth(f32 value);
    f32 NuVpPixelHeight(f32 value);
    f32 NuVpVirtualWidth(f32 value);
    f32 NuVpVirtualHeight(f32 value);
    void NuVpGetPosition2(f32 *x, f32 *y);
    void NuVpSetPosition2(f32 x, f32 y);
    void NuVpSetPosition(f32 x, f32 y);
    void NuVpGetSize2(f32 *width, f32 *height);
    void NuVpSetSize2(f32 width, f32 height);
    void NuVpSetSize(f32 width, f32 height);
    void NuViewPortSet(f32 left, f32 top, f32 right, f32 bottom);
    void NuVpGetScalingMtx(NUMTX *dest);
    void NuVpGetClippingMtx(NUMTX *dest);
    void NuVpUpdate(void);
    void NuVpInit(void);
    void NuVpResetRegions(void);
    void NuVpSetRegions(f32 source_x, f32 source_y, f32 source_right, f32 source_bottom, f32 dest_x, f32 dest_y,
                        f32 dest_right, f32 dest_bottom);

#ifdef __cplusplus
}
#endif
