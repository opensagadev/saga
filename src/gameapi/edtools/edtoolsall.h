#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct FS_FILEENTRYHDR;
struct edcam_s;
struct edui_colour_pick_s;
struct edui_interact_s;
struct eduiitem_s;
struct eduimenu_s;
struct numtl_s;
struct nupad_s;
struct nuvec_s;
char *FS_GetFilterString(char *input, char *output);
f32 FS_GetDirTextWidth();
i32 FS_FileNameFilter(char *);
i32 FS_GetPadWithRepeat(nupad_s *pad, float repeat, float elapsed);
i32 FS_PrevNameLen(char *name);
void EdFileFillBuffer();
void EdFileFlushBuffer();
void EdFileResetBuffers();
void FS_BuildFilterBlocks(char *);
void FS_BuildFilterOutBlocks(char *);
void FS_GetDirList(char *path, char *filter, char *filter_out);
void FS_MakeDateString(FS_FILEENTRYHDR *entry, char *output);
void FS_MakeDateTimeString(FS_FILEENTRYHDR *, char *);
void FS_MakeTimeString(FS_FILEENTRYHDR *entry, char *output);
void FS_MoveCursorDown(i32 steps);
void FS_MoveCursorUp(i32 steps);
void FS_SetCursorToLastFileName();
void FS_SortStrings(char *, char *, i32);
void edDrawLine(nuvec_s *, nuvec_s *, unsigned char, unsigned char, unsigned char);
extern "C" {
    extern i32 edmain_auto_speed;
    extern i32 near_clip_at_cursor;
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    extern numtl_s *uimtls[5];
    i32 eduicbInteractSlider(edui_interact_s *);
    void AddColourPick(eduimenu_s *parent, eduiitem_s *item, f32 *red, f32 *green, f32 *blue, u32 *rgba);
    void CreateColourPicker(void);
    void CreateTestMenu(void *font);
    void FS_SetFileSelPathFromName(char *name);
    void TestMenu(nupad_s *pad);
    void cbCancelSubMenu(eduimenu_s *, eduimenu_s *);
    void cbCancelSubMenuFromItem(eduimenu_s *menu, eduiitem_s *, u32);
    void cbNearClipAtCursor(eduimenu_s *, eduiitem_s *item, u32);
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
    void cbTriggerSubMenu(eduimenu_s *menu, eduiitem_s *item, u32);
    void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
    void do_maya_mouse_camera(edcam_s *camera);
    void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
    void edDrawCross(nuvec_s *centre, u8 red, u8 green, u8 blue);
    void edmainSetReturn(i32 result);
    void eduiAddPropTextPickEnt(eduimenu_s *, eduiitem_s *);
    void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
    void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight, i32 second_highlight, i32 third_highlight);
    void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
    void eduiItemColourPickSetRGB(edui_colour_pick_s *, f32, f32, f32);
    void eduiSetCameraEnabled(i32);
    void eduiSetCursorColour(u32);
    void eduiSetFontScale(f32, f32);
    void eduiSetRenderPlane(i8 plane);
    void eduiShowCursor(i32 show);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
}
