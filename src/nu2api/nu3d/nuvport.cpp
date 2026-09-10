#include "nu2api/nu3d/nuvport.h"

#include "decomp.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"

extern "C" i32 NuRndrStateUpdateCameraState(void);

static constexpr f32 kVirtualWidth = 640.0f;
static constexpr f32 kVirtualHeight = 224.0f;

NUVIEWPORT vpCurrent = {0};
NUVIEWPORT vpDevice = {0};
i32 vport_inval = 0;
NUMTX vp_smtx = {0}; // scaling matrix
NUMTX vp_cmtx = {
    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};
NUVPREGION g_NuVpRegion = {
    0.0f, 0.0f, kVirtualWidth, kVirtualHeight, 0.0f, 0.0f, kVirtualWidth, kVirtualHeight,
    1.0f, 1.0f, 0.0f,          0.0f,           1.0f, 1.0f, 0.0f,          0.0f,
};

void NuVpInit(void) {
    NuPs2GetViewport(&vpDevice);

    vpDevice.center_x = 0.5f;
    vpDevice.center_y = 0.5f;
    vpDevice.clip_min_x = 0.0f;
    vpDevice.clip_min_y = 0.0f;
    vpDevice.clip_max_x = 1.0f;
    vpDevice.clip_max_y = 1.0f;
    vpCurrent = vpDevice;
    vport_inval = 1;

    NuVpUpdate();
}

void NuVpResetRegions(void) {
    f32 height = (f32)PS2_VREZ_H;
    f32 width = (f32)PS2_VREZ_W;

    NuVpSetRegions(0.0f, 0.0f, width, height, 0.0f, 0.0f, width, height);
}

void NuVpRestore(void) {
    vpCurrent = vpDevice;
    vport_inval = 1;
}

void NuVpGetScalingMtx(NUMTX *dest) {
    if (dest == NULL) {
        return;
    }

    *dest = vp_smtx;
}

void NuVpGetClippingMtx(NUMTX *dest) {
    if (dest == NULL) {
        return;
    }
    *dest = vp_cmtx;
}

NUVIEWPORT *NuVpGetCurrentViewport(void) {
    return &vpCurrent;
}

void NuVpGetCurrent(NUVIEWPORT *viewport) {
    *viewport = vpCurrent;
}

void NuVpSetCurrent(NUVIEWPORT *viewport) {
    vpCurrent = *viewport;
    vport_inval = 1;
}

void NuVpSetCurrent2(NUVIEWPORT2 *viewport) {
    NuVpSetPosition2(viewport->x, viewport->y);
    NuVpSetSize2(viewport->width, viewport->height);
    // The original leaves the depth range unchanged.
    vpCurrent.center_x = viewport->center_x;
    vpCurrent.center_y = viewport->center_y;
    vpCurrent.clip_min_x = viewport->clip_min_x;
    vpCurrent.clip_min_y = viewport->clip_min_y;
    vpCurrent.clip_max_x = viewport->clip_max_x;
    vpCurrent.clip_max_y = viewport->clip_max_y;
    vpCurrent.clip_width = viewport->clip_width;
    vpCurrent.clip_height = viewport->clip_height;
    vpCurrent.scissor_width = viewport->scissor_width;
    vpCurrent.scissor_height = viewport->scissor_height;
    vport_inval = 1;
}

void NuVpGetCurrent2(NUVIEWPORT2 *viewport) {
    NuVpGetPosition2(&viewport->x, &viewport->y);
    NuVpGetSize2(&viewport->width, &viewport->height);
    // The original does not write the depth range in this representation.
    viewport->center_x = vpCurrent.center_x;
    viewport->center_y = vpCurrent.center_y;
    viewport->clip_min_x = vpCurrent.clip_min_x;
    viewport->clip_min_y = vpCurrent.clip_min_y;
    viewport->clip_max_x = vpCurrent.clip_max_x;
    viewport->clip_max_y = vpCurrent.clip_max_y;
    viewport->clip_width = vpCurrent.clip_width;
    viewport->clip_height = vpCurrent.clip_height;
    viewport->scissor_width = vpCurrent.scissor_width;
    viewport->scissor_height = vpCurrent.scissor_height;
}

void NuVpGetRegions(f32 *source_x, f32 *source_y, f32 *source_right, f32 *source_bottom, f32 *dest_x, f32 *dest_y,
                    f32 *dest_right, f32 *dest_bottom) {
    *source_x = g_NuVpRegion.source_x;
    *source_y = g_NuVpRegion.source_y;
    *source_right = g_NuVpRegion.source_width;
    *source_bottom = g_NuVpRegion.source_height;
    *dest_x = g_NuVpRegion.dest_x;
    *dest_y = g_NuVpRegion.dest_y;
    *dest_right = g_NuVpRegion.dest_width;
    *dest_bottom = g_NuVpRegion.dest_height;
}

void NuVpSetCentre(f32 x, f32 y) {
    vpCurrent.center_x = x;
    vpCurrent.center_y = y;
    vport_inval = 1;
}

void NuVpSetZRange(f32 minimum, f32 maximum) {
    vpCurrent.min_z = minimum;
    vpCurrent.max_z = maximum;
    vport_inval = 1;
}

void NuVpSetClipping(f32 left, f32 top, f32 right, f32 bottom) {
    vpCurrent.clip_min_x = left < 0.0f ? 0.0f : left;
    vpCurrent.clip_min_y = top < 0.0f ? 0.0f : top;
    vpCurrent.clip_max_x = right > 1.0f ? 1.0f : right;
    vpCurrent.clip_max_y = bottom > 1.0f ? 1.0f : bottom;
    vport_inval = 1;
}

f32 NuVpPixelWidth(f32 value) {
    return (f32)vpCurrent.width * value + (f32)vpCurrent.x;
}

f32 NuVpPixelHeight(f32 value) {
    return (f32)vpCurrent.height * value + (f32)vpCurrent.y;
}

f32 NuVpVirtualWidth(f32 value) {
    return value / (f32)vpCurrent.width;
}

f32 NuVpVirtualHeight(f32 value) {
    return value / (f32)vpCurrent.height;
}

void NuVpGetPosition2(f32 *x, f32 *y) {
    *x = NuFdiv((f32)vpCurrent.x * 0.0625f - ((f32)PS2_VCNTR_X - (f32)(nurndr_pixel_width >> 1)),
                NuFdiv((f32)nurndr_pixel_width, (f32)PS2_VREZ_W));
    *y = NuFdiv((f32)vpCurrent.y * 0.0625f - ((f32)PS2_VCNTR_Y - (f32)(nurndr_pixel_height >> 1)),
                NuFdiv((f32)nurndr_pixel_height, (f32)PS2_VREZ_H));
}

void NuVpSetPosition2(f32 x, f32 y) {
    x += g_NuVpRegion.x_offset;
    y += g_NuVpRegion.y_offset;
    vpCurrent.x =
        (i32)(((f32)nurndr_pixel_width * x / (f32)PS2_VREZ_W + ((f32)PS2_VCNTR_X - (f32)(nurndr_pixel_width >> 1))) *
              16.0f);
    vpCurrent.y =
        (i32)(((f32)nurndr_pixel_height * y / (f32)PS2_VREZ_H + ((f32)PS2_VCNTR_Y - (f32)(nurndr_pixel_height >> 1))) *
              16.0f);
    vport_inval = 1;
}

void NuVpSetPosition(f32 x, f32 y) {
    NuVpSetPosition2(NuFdiv(x * 0.0625f - ((f32)PS2_VCNTR_X - (f32)(nurndr_pixel_width >> 1)),
                            NuFdiv((f32)nurndr_pixel_width, (f32)PS2_VREZ_W)),
                     NuFdiv(y * 0.0625f - ((f32)PS2_VCNTR_Y - (f32)(nurndr_pixel_height >> 1)),
                            NuFdiv((f32)nurndr_pixel_height, (f32)PS2_VREZ_H)));
}

void NuVpGetSize2(f32 *width, f32 *height) {
    *width = ((f32)vpCurrent.width * 0.0625f) / ((f32)nurndr_pixel_width / (f32)PS2_VREZ_W);
    *height = ((f32)vpCurrent.height * 0.0625f) / ((f32)nurndr_pixel_height / (f32)PS2_VREZ_H);
}

void NuVpSetSize2(f32 width, f32 height) {
    width *= g_NuVpRegion.width_scale;
    height *= g_NuVpRegion.height_scale;
    vpCurrent.width = (i32)((f32)nurndr_pixel_width * width / (f32)PS2_VREZ_W * 16.0f);
    vpCurrent.height = (i32)((f32)nurndr_pixel_height * height / (f32)PS2_VREZ_H * 16.0f);
    vport_inval = 1;
}

void NuVpSetSize(f32 width, f32 height) {
    NuVpSetSize2(width * 0.0625f / ((f32)nurndr_pixel_width / (f32)PS2_VREZ_W),
                 height * 0.0625f / ((f32)nurndr_pixel_height / (f32)PS2_VREZ_H));
}

void NuViewPortSet(f32 left, f32 top, f32 right, f32 bottom) {
    static NUVIEWPORT2 vp = {
        0.0f, 0.0f, 640.0f, 224.0f, 8388607.0f, 0.0f, 0.5f, 0.5f, 0.01f, 0.01f, 0.99f, 0.99f, 0.0f, 0.0f, 0.0f, 0.0f,
    };
    vp.x = left;
    vp.y = top;
    vp.width = right - left;
    vp.height = bottom - top;
    NuVpSetCurrent2(&vp);
    NuVpUpdate();
}
