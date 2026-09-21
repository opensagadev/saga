#pragma once

#include <stdarg.h>

#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nucore/common.h"

typedef enum {
    NUQFNT_CSMODE_UNINITIALISED = 0,
    NUQFNT_CSMODE_PS2 = 1,
    NUQFNT_CSMODE_PIXEL = 2,
    NUQFNT_CSMODE_NORMALISED = 3,
    NUQFNT_CSMODE_ABSOLUTE = 4,
    NUQFNT_CSMODE_CNT = 5,
} NUQFNT_CSMODE;

typedef struct vufnt_android_s {
    u32 colour;
    f32 x;
    f32 y;
    f32 z;
    NUMTX mtx;
    f32 x_scale;
    f32 y_scale;
} VUFNT_ANDROID;

typedef struct vucharidx_s {
    u16 unicode;
    u16 index;
} VUCHARIDX;

typedef struct vufntchar_s {
    f32 x;
    f32 y;
    f32 width;
} VUFNTCHAR;

typedef struct nufntchar_s {
    f32 u0;
    f32 v0;
    f32 u1;
    f32 v1;
    u32 unknown_10;
} NUFNTCHAR;

typedef struct nufnt_s {
    u8 unknown_00[0x0c];
    i16 height;
    u8 unknown_0e[6];
    i16 baseline;
    i16 ic_gap;
    NUMTL *mtl;
    i32 glyph_count;
    u8 character_map[256];
    NUFNTCHAR glyphs[256];
} NUFNT;

typedef struct vufnthdr_s {
    f32 ic_gap;
    f32 height;
    f32 inverse_texture_width;
    f32 inverse_texture_height;
    f32 unknown_10;
    f32 unknown_14;
    f32 space_width;
    f32 unknown_1c;
} VUFNTHDR;

typedef struct vufnt_s {
    u8 filler0[6];
    u16 flags;
    i32 size;
    i32 glyph_count;
    i32 unicode_count;
    f32 height;

    f32 baseline;

    f32 space_width;

    i32 mode;

    f32 ic_gap; // 0x24

    NUFNT *legacy_font;

    f32 *x_scale;
    f32 *y_scale;

    VUFNTCHAR *glyphs; // 0x34

    union {
        VUCHARIDX *unicode_map;
        u8 *character_map;
    };

    VUFNTHDR *hdr;

    NUMTL *mtl; // 0x40

    u32 *color_abgr; // 0x44

    VUFNT_ANDROID *platform_data; // 0x48
} VUFNT;

typedef void NUQFNT;

#ifdef __cplusplus

i32 NuQFntReadPS(VUFNT *font, i32 tex_id, i32 flags, i32 render_plane, VARIPTR *buf, VARIPTR buf_end);
i32 UnicodeToIndexFast(VUCHARIDX *map, i32 count, u16 unicode);

extern "C" {
#endif
    extern NUQFNT_CSMODE NuQFntCSMode;
    extern NUQFNT *system_qfont;
    extern i32 NuQFntCSModeStackIndex;
    extern NUQFNT_CSMODE NuQFntCSModeStack[16];
    NUQFNT_CSMODE NuQFntGetCoordinateSystem(void);
    void NuQFntPushCoordinateSystem(NUQFNT_CSMODE mode);
    void NuQFntPopCoordinateSystem(void);

    extern f32 qfnt_rezscale_w;
    extern f32 qfnt_rezscale_h;

    extern f32 qfnt_offscale_x;
    extern f32 qfnt_offscale_y;

    extern f32 qfnt_len_scale;
    extern f32 qfnt_height_scale;
    extern f32 nuqfnt_space_width;
    extern u32 NuQFntMode;
    extern u32 NuQFntModeStack[16];
    extern i32 NuQFntModeStackIndex;
    extern NUQFNT *system_qfont;

    void NuQFntInit(VARIPTR *buf, VARIPTR buf_end);
    VUFNT *NuQFntCreate(NUFNT *font, i32 flags, i32 render_options, VARIPTR *buf, VARIPTR *buf_end);

    NUQFNT *NuQFntRead(char *filename, VARIPTR *buf, VARIPTR buf_end);
    NUQFNT *NuQFntReadBuffer(VARIPTR *font, VARIPTR *buf, VARIPTR buf_end);
    VUFNT *NuQFntDuplicate(VUFNT *font, i32 flags, i32 render_plane, VARIPTR *buf, VARIPTR *buf_end);

    NUQFNT_CSMODE NuQFntSetCoordinateSystem(NUQFNT_CSMODE mode);
    void NuQFntSetICGap(NUQFNT *font, float ic_gap);
    void NuQFntSetJustifiedTolerances(f32 squash, f32 stretch);
    void NuQFntSetMtx(NUQFNT *font, NUMTX *mtx);
    void NuQFntSetMtxRS(RNDRSTREAM *stream, NUQFNT *font, NUMTX *mtx);
    void NuQFntSetRS(RNDRSTREAM *stream, NUQFNT *font);
    void NuQFntSetColourRS(RNDRSTREAM *stream, NUQFNT *font, u32 colour);
    void NuQFntSetScaleRS(RNDRSTREAM *stream, NUQFNT *font, f32 x_scale, f32 y_scale);
    void NuQFntMoveRS(RNDRSTREAM *stream, NUQFNT *font, f32 x, f32 y, f32 z);
    f32 NuQFntHeight(NUQFNT *font);
    f32 NuQFntPrintLenW(NUQFNT *font, u16 *text);
    f32 NuQFntPrintLenU(NUQFNT *font, char *text);
    void NuQFntPrintCharW(NUQFNT *font, u16 *text, u32 flags);
    void NuQFntPrintRSW(RNDRSTREAM *stream, NUQFNT *font, u16 *text, u32 flags);
    void NuQFntUTF8toQCode(NUQFNT *font, char *text, u16 *encoded);

    f32 NuQFntBaseline(NUQFNT *font);
    void NuQFntSet(NUQFNT *font);
    void NuQFntSetColour(NUQFNT *font, u32 colour);
    void NuQFntSetScale(NUQFNT *font, f32 x_scale, f32 y_scale);
    void NuQFntMove(NUQFNT *font, f32 x, f32 y, f32 z);
    void NuQFntPrintW(NUQFNT *font, u16 *text);
    void NuQFntPrintU(NUQFNT *font, char *text);
    void NuQFntPrintEx(NUQFNT *font, i32 x, i32 y, i32 flags, const char *format, ...);
    void NuQFntPushPrintMode(u32 mode);
    void NuQFntPopPrintMode(void);
    f32 NuQFntHeightScale(void);
    f32 NuQFntLenScale(void);
    void NuQFntMove2d(NUQFNT *font, f32 x, f32 y, f32 z);
    void NuQFntSetPrintMode(u32 mode);
    u32 NuQFntGetPrintMode(void);
    void NuQFntEncodeUnicodeString(NUQFNT *font, u16 *text);
    f32 NuQFntPrintLenV(NUQFNT *font, const char *format, va_list arguments);
    void NuQFntWrite(char *path, VUFNT *font);
    void NuQFntWriteUniversalFont(char *path, VUFNT *font, char *texture_path);
    void NuQFntSet2d(NUQFNT *font);
    void NuQFntSetScale2d(NUQFNT *font, f32 x_scale, f32 y_scale);
    void NuQFntSetPointSize(NUQFNT *font, f32 width, f32 height);
    void NuQFntPrint2dU(NUQFNT *font, char *text);
    void NuQFntSetColour2d(NUQFNT *font, u32 colour);
    void NuQFntSetSpaceWidth(NUQFNT *font, f32 width);
    u16 NuQFntEncodeUnicodeChar(NUQFNT *font, u16 character);
    NUQFNT *NuQFntLoadPtr(char *path, char *name, i32 flags, i32 render_plane, VARIPTR *buf, VARIPTR *buf_end);
    f32 NuQFntPrintJustifiedW(NUQFNT *font, u16 *text, f32 x, f32 y, f32 z, f32 sx, f32 sy, f32 width, f32 line_spacing,
                              u32 colour, NUMTX *mtx);

    void NuFntInit(void);
    void NuFntSetFixedWidthNumerals(void);
    i32 NuFntToUpper(void);
    i32 NuFntToLower(void);
    void NuFntSetPen(i32 colour);
    void NuFntSet(i32);
    void NuFntScale(i32, i32);
    i32 NuFntGetScreenHeight(void);
    void NuFntPointSize(void);
    void NuFntMoveAbs(void);
    void NuFntMoveRel(void);
    void NuFntPos(void);
    i32 NuFntPrintLenV(void);
    i32 NuFntPrintLen(void);
    i32 NuFntPrintV(void);
    i32 NuFntPrint(void);
    void NuFntClose(void);
    void NuFntPrintEx(void);
    void *NuFntCreate(void);
    void NuFntDestroy(void);
    void NuFntWrite(void);
    void *NuFntLoadPtr(void);
#ifdef __cplusplus
}

struct nufnt_s;
struct nutex_s;
void NuFntSave(nufnt_s *font, i32 texture_id, char *path);
void NuFntDumpReadable(nufnt_s *font, char *path);
i32 NuFntFindStart(nutex_s *texture, i32 *x, i32 *y, i32 width, i32 height);
i32 NuFntFindEnd(nutex_s *texture, i32 *x, i32 *y, i32 width, i32 height);
i32 NuFntPrintChar(char character);
void NuQFntSetMtx2d(void *font, numtx_s *matrix);
#endif
