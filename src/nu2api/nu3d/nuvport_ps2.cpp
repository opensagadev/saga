#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nucamera.h"

extern NUVIEWPORT vpCurrent;
extern i32 vport_inval;
extern NUMTX vp_smtx;
extern "C" i32 NuRndrStateUpdateCameraState(void);

i32 PS2_VCNTR_X = 640;
i32 PS2_VCNTR_Y = 360;
i32 PS2_VREZ_W = 640;
i32 PS2_VREZ_H = 224;

void NuVpSetScalingMtx(void) {
    f32 vp_x = (f32)(vpCurrent.x >> 4);
    f32 vp_y = (f32)(vpCurrent.y >> 4);
    f32 vp_w = (f32)(vpCurrent.width >> 4);
    f32 vp_h = (f32)(vpCurrent.height >> 4);
    f32 vp_minz = vpCurrent.min_z;
    f32 vp_maxz = vpCurrent.max_z;

    vp_smtx.m00 = vp_w * 0.5f;
    vp_smtx.m01 = 0.0f;
    vp_smtx.m02 = 0.0f;
    vp_smtx.m03 = 0.0f;
    vp_smtx.m10 = 0.0f;
    vp_smtx.m11 = -vp_h * 0.5f;
    vp_smtx.m12 = 0.0f;
    vp_smtx.m13 = 0.0f;
    vp_smtx.m20 = 0.0f;
    vp_smtx.m21 = 0.0f;
    vp_smtx.m22 = vp_maxz - vp_minz;
    vp_smtx.m23 = 0.0f;
    vp_smtx.m30 = vpCurrent.center_x * vp_w + vp_x;
    vp_smtx.m31 = vpCurrent.center_y * vp_h + vp_y;
    vp_smtx.m32 = vp_minz;
    vp_smtx.m33 = 1.0f;
}

static void NuVpSetClippingMtx(void) {
    cmtx.m00 = 2.0f / (f32)vpCurrent.width;
    cmtx.m01 = 0.0f;
    cmtx.m02 = 0.0f;
    cmtx.m03 = 0.0f;
    cmtx.m10 = 0.0f;
    cmtx.m11 = 2.0f / (f32)vpCurrent.height;
    cmtx.m12 = 0.0f;
    cmtx.m13 = 0.0f;
    cmtx.m20 = 0.0f;
    cmtx.m21 = 0.0f;
    cmtx.m22 = 1.0f / (vpCurrent.max_z - vpCurrent.min_z);
    cmtx.m23 = 0.0f;
    cmtx.m30 = -1.0f - 2.0f * (vpCurrent.clip_min_x / (vpCurrent.clip_max_x - vpCurrent.clip_min_x));
    cmtx.m31 = 1.0f - 2.0f * (vpCurrent.clip_min_y / (vpCurrent.clip_max_y - vpCurrent.clip_min_y));
    cmtx.m32 = -vpCurrent.min_z / (vpCurrent.max_z - vpCurrent.min_z);
    cmtx.m33 = 1.0f;
}

void NuVpUpdate(void) {
    if (vport_inval != 0) {
        vport_inval = 0;
        NuVpSetScalingMtx();
        NuVpSetClippingMtx();
        vpCurrent.clip_width = vpCurrent.clip_height = 1.0f;
        NuRndrStateUpdateCameraState();
    }
}

void NuVpSetRegions(f32 source_x, f32 source_y, f32 source_right, f32 source_bottom, f32 dest_x, f32 dest_y,
                    f32 dest_right, f32 dest_bottom) {
    g_NuVpRegion.x_offset = dest_x;
    g_NuVpRegion.y_offset = dest_y;
    g_NuVpRegion.width_scale = (dest_right - dest_x) / (f32)PS2_VREZ_W;
    g_NuVpRegion.height_scale = (dest_bottom - dest_y) / (f32)PS2_VREZ_H;
    g_NuVpRegion.projection_x_scale = (f32)PS2_VREZ_W / (source_right - source_x);
    g_NuVpRegion.projection_y_scale = (f32)PS2_VREZ_H / (source_bottom - source_y);
    g_NuVpRegion.projection_x_offset = (((f32)PS2_VREZ_W - source_right) - source_x) * 2.0f / (f32)PS2_VREZ_W;
    g_NuVpRegion.projection_y_offset = (((f32)PS2_VREZ_H - source_bottom) - source_y) * -2.0f / (f32)PS2_VREZ_H;
}

void NuPs2GetViewport(NUVIEWPORT *vp) {
    vp->x = (i32)(((f32)PS2_VCNTR_X - (f32)nurndr_pixel_width * 0.5f) * 16.0f);
    vp->y = (i32)(((f32)PS2_VCNTR_Y - (f32)nurndr_pixel_height * 0.5f) * 16.0f);
    vp->width = (i32)((f32)nurndr_pixel_width * 10240.0f / (f32)PS2_VREZ_W);
    vp->height = (i32)((f32)nurndr_pixel_height * 3584.0f / (f32)PS2_VREZ_H);
    vp->min_z = 0.0f;
    vp->max_z = 1.0f;
}
