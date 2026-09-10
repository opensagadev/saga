#include "nu2api/nu3d/nurndrstat.h"
#include <string.h>
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuvport.h"

extern i32 nurndr_pixel_width;
extern i32 nurndr_pixel_height;

extern "C" i32 NuRndrStateUpdateCameraState(void) {
    NUMTX *projection = NuCameraGetProjectionMtx();
    NUMTX *view = NuCameraGetViewMtx();
    render_state.view = *view;
    render_state.proj_00 = projection->m00;
    render_state.proj_11 = projection->m11;
    render_state.proj_22 = projection->m22;
    render_state.proj_23 = projection->m23;
    render_state.proj_32 = projection->m32;
    render_state.proj_20 = projection->m20;
    render_state.proj_21 = projection->m21;
    NuVpGetPosition2(&render_state.vpx, &render_state.vpy);
    NuVpGetSize2(&render_state.vpw, &render_state.vph);
    render_state.vpx *= (f32)nurndr_pixel_width / 640.0f;
    render_state.vpw *= (f32)nurndr_pixel_width / 640.0f;
    render_state.vpy *= (f32)nurndr_pixel_height / 224.0f;
    render_state.vph *= (f32)nurndr_pixel_height / 224.0f;
    render_state.camera_state = nullptr;
    render_state.state.global_id++;
    render_state.state.camera_id++;
    return 1;
}

extern "C" void NuRndrStateInit(void) {
    memset(&render_state, 0, sizeof(render_state));
}

extern "C" i32 NuRndrStateGetFogEnabled(void) {
    return render_state.fog_enabled;
}

extern "C" void NuRndrStateSetFogEnabled(i32 enabled) {
    render_state.fog_enabled = enabled;
    render_state.fog_state = NULL;
    ++render_state.state.global_id;
    ++render_state.state.fog_id;
}

extern "C" void NuRndrStateSetFogState(f32 near_distance, f32 far_distance, u32 colour, f32 density) {
    render_state.fog_near = near_distance;
    render_state.fog_far = far_distance;
    render_state.fog_rgba = colour;
    render_state.fog_density = density;
    render_state.fog_state = NULL;
    ++render_state.state.global_id;
    ++render_state.state.fog_id;
}
