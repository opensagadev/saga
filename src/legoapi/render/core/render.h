#ifndef LEGOAPI_RENDER_CORE_RENDER_H
#define LEGOAPI_RENDER_CORE_RENDER_H

#include "decomp.h"
#include "legoapi/legoapi_types.h"

// Core rendering helpers.

extern void (*DisguiseAdjustFn)(i32 character_id, i32 hat, NUVEC *scale, NUVEC *offset);

void DrawPaintLights(void);
void Draw3DObject(WORLDINFO_s *world, i32 object_index, NUVEC *position, u16 x_rotation, u16 y_rotation, u16 z_rotation,
                  f32 scale_x, f32 scale_y, f32 scale_z, i32 rotate_order);
void Draw3DObjectAlpha(WORLDINFO_s *world, i32 object_index, NUVEC *position, u16 x_rotation, u16 y_rotation,
                       u16 z_rotation, f32 scale_x, f32 scale_y, f32 scale_z, i32 rotate_order, f32 alpha);
void DrawGameMessage_Targets(GAMEMESSAGE_s *message, NUVEC *position, f32 scale);
extern i32 dco_locatorposonly;
void DrawObjectOnCharacter(WORLDINFO_s *world, GameObject_s *object, i32 object_id, nuhspecial_s *special, i32 locator,
                           i32 second_locator, NUMTX *joints, i32 reflect, u32 layers, NUMTX *rotation,
                           NUVEC *translation, f32 alpha, f32 scale);
void BackDrop_ResetColours(void);
extern f32 KITPOSY;
void DrawStatusIcons(STATUSPACKET_s *status, f32 y, f32 alpha);
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
void DrawPanel3DObjectMtx(nuhspecial_s *special, numtx_s *matrix, f32 alpha);
i32 DrawPanel3DObjectNoAlpha(f32 x, f32 y, f32 z, f32 scale_x, f32 scale_y, f32 scale_z, u16 rotate_x, u16 rotate_y,
                             u16 rotate_z, nuhspecial_s *special, i32 rotate_order);
void DrawMiniKitCount(f32 position, f32 scale, i32 count, i32 maximum);
struct STATUSMINIKITPART_s {
    NUMTX matrix;
    nuhspecial_s *special;
    i32 enabled;
};
extern STATUSMINIKITPART_s KitPart[10];
void DrawTouchPrompt(char *, char *, bool, bool);
void DrawCharIcon(i32, float, float, float, float, i32, float, float, i32, nuhspecial_s *);
void DrawPauseFade(void);

#endif
