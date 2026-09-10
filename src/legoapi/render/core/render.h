#ifndef LEGOAPI_RENDER_CORE_RENDER_H
#define LEGOAPI_RENDER_CORE_RENDER_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

// Core rendering helpers (module legoapi/render/core, render.cpp).

void DrawPaintLights(void);
void DrawTimer(i32 mins, i32 secs, i32 unknown);
void DrawBossHitPoints(GameObject_s *obj);
void DrawMiniSnowTroopers(WORLDINFO_s *world);
void DrawForceBackEffect(nuhspecial_s *special);
void DrawSaveSlots(MENU_s *menu, float y);
void Draw_AUTOSAVEWARNING(void);
#ifdef __cplusplus
extern "C" {
#endif
    void RndrCircleXZ(NUVEC *centre, f32 radius, i32 colour, i32 segments);
    void RndrOSphere(NUVEC *centre, f32 radius, i32 colour, i32 segments, i32 unused);
    void RndrOSquare(NUVEC *centre, f32 radius, i32 colour);
    void LocaledbitsDrawCircleXY(NUVEC *centre, f32 radius, u32 colour, i32 unused, i32 segments);
    void LocaledbitsDrawSolidEllipseXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 rotation, f32 lower_y,
                                       f32 upper_y, u32 colour, i32 unused, i32 segments);
    void LocaledbitsDrawSolidCircleXY(NUVEC *centre, f32 radius, f32 lower_y, f32 upper_y, u32 colour, i32 unused,
                                      i32 segments);
    void Text3DEx(char *, f32, f32, f32, f32, f32, f32, u32, u8, u8, u8, i32);
#ifdef __cplusplus
}
#endif
i32 DrawPanel3DObject(float, float, float, float, float, float, u16, u16, u16, nuhspecial_s *, i32, float);
void DrawTouchPrompt(char *, char *, bool, bool);
void DrawCharIcon(i32, float, float, float, float, i32, float, float, i32, nuhspecial_s *);
void DrawPauseFade(void);
void DrawPanel(void);

#endif
