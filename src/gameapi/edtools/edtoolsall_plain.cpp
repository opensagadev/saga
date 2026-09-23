#include "decomp.h"
#include "gameapi/edtools/edanim_internal.h"
#include "gameapi/edtools/edgra.h"
#include "gameapi/edtools/edui.h"
#include "gameapi/edtools/edui_colour.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/nupad.h"
#include "gameapi/edtools/gameapi_edtools_types.h"
#include "gameapi/edtools/edfile.h"
#include "gameapi/edtools/edcam.h"
#include "gameapi/edtools/edbri_internal.h"
#include "gameapi/edtools/edrender.h"
#include "gameapi/edtools/edgra_internal.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/core/terrain.h"
#include "legoapi/render/fx.h"
#include "legoapi/render/fx/game_deb.h"
#include "legoapi/world/levels/levels.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nuprim_internal.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/android/nuobject_android.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nufile/nufile.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The original animation editor callbacks and lifecycle share file-local state.
#include "gameapi/edtools/edanimall.cpp"
#include "gameapi/edtools/edmenucallbacks.cpp"

float edanimPlayerAnimDistance(i32 parameter_index);
void edpartDestroy(i32 index);
void edgraDoInput(nupad_s *pad);
void edgraDetermineNearestInstance(f32 distance);
void edgraDrawCursor();
extern "C" void edgraClumpsReset(void);
extern "C" void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                       i32 rotation_y);
extern "C" i32 NuRndrDoingScreenGrab;
static i32 edbits_cubecount;
static i32 edgra_clumpthin = 1;
static i32 edgra_elementthin = 1;
static VARIPTR gra_ptr;
static VARIPTR gra_end;
static i32 editor_return;
static i32 bShowCursor = 1;
static NUQFNT *edui_font;
static i32 edui_donotdraw;
static i32 item_width;
static void *dir_list;
static i32 dir_size;
static eduimenu_s *filepick_menu;
static edui_file_pick_s *filepick_item;

struct FilePickDirectoryEntry {
    u8 flags;
    u8 reserved_01[3];
    i32 size;
    u8 reserved_08[9];
    i8 minute;
    i8 hour;
    i8 month;
    i8 day;
    u8 reserved_15;
    i16 year;
    char name[0x100];
};
DECOMP_ASSERT(sizeof(FilePickDirectoryEntry) == 0x118, "file picker directory entry ABI");
static char ed_levelfile[256];
static f32 edui_font_scale_x = 0.9f;
static f32 edui_font_scale_y = 0.9f;
static f32 edmain_menu_scale = 1.0f;
static u32 edui_cursor_colour = 0xff000000;
static char eduiPropTextEdit[256];
static char eduiPropTextStore[256];
static char *textrow[8];
static char TextPickCopyBuffer[0x40];
static i32 cursor_flash;
static char *edpp_save_names[6];
static i32 edptl_count;
static NUVEC entry_position;

static inline i32 edppCurveSegment(const debris_float_key_s *keys, f32 time) {
    if (time >= keys[0].time && time <= keys[1].time)
        return 0;
    if (time >= keys[1].time && time <= keys[2].time)
        return 1;
    if (time >= keys[2].time && time <= keys[3].time)
        return 2;
    if (time >= keys[3].time && time <= keys[4].time)
        return 3;
    if (time >= keys[4].time && time <= keys[5].time)
        return 4;
    if (time >= keys[5].time && time <= keys[6].time)
        return 5;
    if (time >= keys[6].time && time <= keys[7].time)
        return 6;
    return 7;
}

#include "gameapi/edtools/edptlall.cpp"

static inline f32 edppInterpolateTorusCurve(const debris_float_key_s *keys, f32 time) {
    i32 index = edppCurveSegment(keys, time);
    if (index == 7)
        return 0.0f;
    return ((time - keys[index].time) / (keys[index + 1].time - keys[index].time)) *
               (keys[index + 1].value - keys[index].value) +
           keys[index].value;
}

void edppDoInput(nupad_s *pad);
void edppDetermineNearest(f32 distance);
void edppDrawCursor();
void edppHighlightNearest();
extern "C" void DebrisStatusNormal(i32 *handle);

extern "C" {
    __attribute__((visibility("hidden"))) void tpentOnEnter(eduimenu_s *, eduiitem_s *, u32);
    __attribute__((visibility("hidden"))) void tpentPropOnEnter(eduimenu_s *, eduiitem_s *, u32);
    extern numtl_s *uimtls[5];
    extern i32 ui_bgmtl;
    extern i32 ui_outmtl;
    void NuRndrRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrGradRect2di(i32 x, i32 y, i32 width, i32 height, i32 *colours, numtl_s *material);
    void NuRndrLineRect2di(i32 x, i32 y, i32 width, i32 height, i32 colour, numtl_s *material);
    void NuRndrLine2di(i32 x0, i32 y0, i32 x1, i32 y1, i32 colour, numtl_s *material);
    void NuRndrLineStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
    void NuRndrTriStrip2di(i32 *points, f32 *uvs, i32 count, i32 colour, numtl_s *material);
}

// The original local font helpers read the same private draw state as the
// editor callbacks, so they belong to this translation unit.
static __used__ void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...) {
    if (!edui_donotdraw) {
        char text[1024];
        NuQFntPushPrintMode(2);
        va_list arguments;
        va_start(arguments, format);
        NuVSPrintf(text, format, arguments);
        va_end(arguments);
        i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
        if (alignment == 32)
            x -= width;
        else if (alignment == 64)
            x -= width / 2;
        NuQFntMove(font, x, y, 0.0f);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }
}

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...) {
    if (!edui_donotdraw) {
        char text[1024];
        i32 screen_x = static_cast<i32>(x * 16.0f);
        i32 screen_y = static_cast<i32>(y * 8.0f);
        i32 screen_clip_x = static_cast<i32>(clip_x * 16.0f);
        i32 screen_clip_width = static_cast<i32>(clip_width * 16.0f);
        NuQFntPushPrintMode(2);
        va_list arguments;
        va_start(arguments, format);
        NuVSPrintf(text, format, arguments);
        va_end(arguments);
        i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
        if (alignment == 32)
            screen_x -= width;
        else if (alignment == 64)
            screen_x -= width / 2;
        if (screen_x < screen_clip_x)
            screen_x = screen_clip_x;
        i32 length = NuStrLen(text);
        if (width > screen_clip_width && length > 1) {
            char *end = text + length - 1;
            do {
                *end-- = '\0';
                width = static_cast<i32>(NuQFntPrintLenU(font, text));
            } while (width > screen_clip_width && end != text);
        }
        NuQFntMove(font, screen_x, screen_y, 0.0f);
        NuQFntPrintU(font, text);
        NuQFntPopPrintMode();
    }
}
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x, i32 clip_width, char *format, ...) {
    if (edui_donotdraw)
        return;

    char text[1024];
    NuQFntPushPrintMode(2);
    va_list arguments;
    va_start(arguments, format);
    NuVSPrintf(text, format, arguments);
    va_end(arguments);
    i32 width = static_cast<i32>(NuQFntPrintLenU(font, text));
    if (alignment == 32)
        x -= width;
    else if (alignment == 64)
        x -= width / 2;
    if (x < clip_x)
        x = clip_x;
    i32 length = NuStrLen(text);
    if (width > clip_width) {
        char *end = text + length - 1;
        while (end > text) {
            *end-- = '\0';
            width = static_cast<i32>(NuQFntPrintLenU(font, text));
            if (width <= clip_width)
                break;
        }
    }
    NuQFntMove(font, static_cast<f32>(x), static_cast<f32>(y), 0.0f);
    NuQFntPrintU(font, text);
    NuQFntPopPrintMode();
}
static i32 show_x_scale_help;
static i32 show_y_scale_help;

extern "C" i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
extern "C" i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
extern "C" f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);

struct EDBITS_GAME_SOUND {
    char name[16];
    i32 id;
};

static void edgraInit();
static void edgraClose();
static void edgraEnter();
static i32 edgraProc(f32 delta_time, nupad_s *pad);
static void edgraRender();
static void edppInit();
static void edppClose();
static void edppEnter();
static void edppApply();
static i32 edppProc(f32 delta_time, nupad_s *pad);
static void edppRender();
static void edbriInit();
static void edbriClose();
static void edbriEnter();
static i32 edbriProc(f32 delta_time, nupad_s *pad);
static void edbriRender();
static void edanimInit();
static void edanimClose();
static void edanimEnter();
static i32 edanimProc(f32 delta_time, nupad_s *pad);
static void edanimRender();

extern "C" {
    void nugraphInit(nugraph_s *graph);
    i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
    void eduicbItemDestroy(eduimenu_s *, eduiitem_s *);
    void eduicbItemDestroyProp(eduimenu_s *, eduiitem_s *);
    i32 eduicbInteractSlider(edui_interact_s *);
    static i32 eduicbProcessSel(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGradPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *);
    static i32 eduicbProcessSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGraph(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessGreyPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessTexturePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessColourSlider(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessExpander(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbItemExpanderClose(edui_expander_s *);
    static i32 eduicbProcessFilter(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessFilePick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static void eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *);
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *);
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32);
    static void eduicbItemSliderDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemFilePickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemTextPickDestroy(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyExpander(eduimenu_s *, eduiitem_s *);
    static void eduicbItemDestroyFilter(eduimenu_s *, eduiitem_s *);
    static i32 eduicbInteractSel(edui_interact_s *);
    static i32 eduicbInteractColourPick(edui_interact_s *);
    static i32 eduicbInteractExpander(edui_interact_s *);
    static i32 eduicbInteractFilter(edui_interact_s *);
    static i32 eduicbInteractProp(edui_interact_s *);
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
    typedef void (*EDBITSPLAYSOUNDCALLBACK)(NUVEC *, i32);
    typedef i32 (*EDBITSREQUESTSOUNDCALLBACK)(char *);

    // GCC 4.7 emits these file-scope objects in reverse declaration order, so
    // each group below mirrors the original linked order in reverse.
    i32 bCameraEnabled = 1;
    i32 edui_filepick_listsize = 8;
    u32 eduiitem_context_outline_colour = 0x0000ff00;
    u32 eduiitem_selected_outline_colour = 0x80ffffff;
    u32 eduimenu_selected_outline_colour = 0x80ffffff;
    u32 eduimenu_outline_colour = 0x80404040;
    f32 edmain_default_camera_speed = 1.0f;
    i32 ed_main_menu_h = 320;
    i32 ed_main_menu_w = 180;
    i32 ed_main_menu_y = 50;
    i32 ed_main_menu_x = 220;
    eduiiattr_s ed_attr = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    i32 eduiPropTextPos = -1;
    i32 edmain_render_interacts;
    ed_module_s edanimdesc = {NULL, NULL, "Anim Editor", edanimInit, edanimClose, edanimEnter,  NULL,
                              NULL, NULL, NULL,          0x6d696e61, edanimProc,  edanimRender, NULL};
    ed_module_s edbridesc = {NULL, NULL, "Bridge Editor", edbriInit, edbriClose,  edbriEnter, NULL, NULL,
                             NULL, NULL, 0x64697262,      edbriProc, edbriRender, NULL};
    ed_module_s edgradesc = {NULL, NULL, "Grass Editor", edgraInit,  edgraClose, edgraEnter,  NULL,
                             NULL, NULL, NULL,           0x73617267, edgraProc,  edgraRender, NULL};
    ed_module_s edptldesc = {NULL,       NULL,       "Particle Positioner",
                             edppInit,   edppClose,  edppEnter,
                             NULL,       edppApply,  NULL,
                             NULL,       0x706c7470, edppProc,
                             edppRender, NULL};
    i32 edpp_create_type = -1;
    f32 edpp_copy_size = 0.2f;
    f32 edpp_scale_factor = 1.0f;
    i32 edptl_repeatboxxzlock = 1;
    i32 edptl_clipboard_entry = -1;

    eduimenu_s *edui_messagemenu;
    u32 edblack[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    u32 edgrey[4] = {0x80303030, 0x80303030, 0x80808080, 0x80808080};
    u32 eddarkred[4] = {0x80000050, 0x80ff0000, 0x80808080, 0x80404040};
    NUMTL *uimtls[5];
    i32 ui_bgmtl;
    i32 ui_outmtl;

    eduimenu_s *edgra_active_menu;
    eduimenu_s *edgra_options_menu;
    eduimenu_s *edgra_instance_menu;
    eduimenu_s *edgra_changeinstance_menu;
    eduimenu_s *edgra_clumpproperties_menu;
    eduimenu_s *edgra_clumpsizes_menu;
    eduimenu_s *edgra_clumparea_menu;
    eduimenu_s *edgra_clumpdist_menu;
    eduimenu_s *edgra_clumpfade_menu;
    eduimenu_s *edgra_clumpterrain_menu;
    eduimenu_s *edgra_clumpmode_menu;
    eduimenu_s *edgra_dpadmode_menu;
    eduimenu_s *edgra_sscale_menu;
    eduimenu_s *edgra_globals_menu;
    i32 edgra_filter;
    struct numtl_s *edgra_mtl;
    NUMTL *edgra_mtl_zoff;
    NUVEC edgra_cam_pos;
    f32 edgra_cam_dist;
    i32 edgra_cam_ax;
    i32 edgra_cam_ay;
    f32 edgra_size;
    i32 edgra_nearest;
    i32 edgra_nearest_instance;
    i32 edgra_instance_type;
    i32 edgra_fadeunits_used;
    i32 edgra_clumps_used;
    i32 edgra_fadeclumps_used;
    i32 edgra_clump_size;
    i32 edgra_pageid;
    i32 edgra_rotz;
    i32 edgra_roty;
    i32 edgra_dpadmode;
    i32 edgra_editormode;
    void *edgra_free_vecbuffer;
    NUMTX *edgra_mtxbuffer;
    NUVEC *edgra_vecbuffer;
    NUGSCN *edgra_page_scene[8];
    void *edgra_page_terrain[8];
    NUMTX *edgra_page_matrix_stack[8];
    i32 EDGRA_MAX_CLUMPS;
    i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
    i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
    edgra_clump_s *GrassClumps;
    edgra_individual_s *IndGrassClumps;
    i32 *IndGrassClumpsUsed;
    i32 edgra_ind_clumps_used;

    i32 edpp_page_used[8];
    i32 edpp_page_on[8];
    usize edpp_page_scene[8];
    i32 edpp_types_used;
    i32 edpp_copy_source[8];
    i32 edpp_usememcard;
    edpp_particle_s edpp_ptls[512];
    NUVEC edpp_cam_pos;
    f32 edpp_cam_dist;
    i32 edpp_cam_ax;
    i32 edpp_cam_ay;
    i32 edpp_first_time_this_level;
    i32 edpp_numparticles;
    NUMTL *edpp_mtl;
    NUMTL *edpp_boxmtl;
    i32 edpp_nearest;
    i32 edpp_curr;
    i32 edpp_snap_enabled;
    u8 edpp_effect_list;
    i32 edpp_instances_used;
    i32 edpp_copy_mode;
    i32 edpp_copy_enclosed;
    i32 edpp_copy_source_count;
    NUVEC edpp_copy_source_vec;
    i32 edpp_num_orphans;
    i32 edpp_showAllPlaced;
    i32 edpp_dpad_mode;
    i32 edpp_readout;
    eduimenu_s *edpp_active_menu;
    eduimenu_s *ptltypemenu;
    eduimenu_s *ptloptmenu;
    eduimenu_s *ptlgsortmenu;
    eduimenu_s *ptlgcodemenu;
    eduimenu_s *ptlemitmenu;
    eduimenu_s *ptlsizemenu;
    eduimenu_s *ptlrotmenu;
    eduimenu_s *ptljibmenu;
    eduimenu_s *ptlcolmenu;
    eduimenu_s *ptlemitvelmenu;
    eduimenu_s *ptlgravmenu;
    eduimenu_s *ptlvaremitmenu;
    eduimenu_s *ptlvarstartmenu;
    eduimenu_s *ptlstartvelmenu;
    eduimenu_s *ptlcutoffmenu;
    eduimenu_s *ptldatamenu;
    eduimenu_s *ptlreadoutmenu;
    eduimenu_s *memcardmenu;
    eduimenu_s *messagemenu;
    eduimenu_s *loadeffectsmenu;
    eduimenu_s *mergeeffectsmenu;
    eduimenu_s *deleteeffectsmenu;
    eduimenu_s *filenamemenu;
    eduimenu_s *namemenu;
    eduimenu_s *confirmmenu;
    eduimenu_s *changegenratemenu;
    eduimenu_s *totalptlsmenu;
    eduimenu_s *etimemenu;
    eduimenu_s *emittimemenu;
    eduimenu_s *sscalemenu;
    eduimenu_s *texturemenu;
    eduimenu_s *textureselectmenu;
    eduimenu_s *collmenu;
    eduimenu_s *effectlistmenu;
    eduimenu_s *ptlclipmenu;
    eduimenu_s *dpadmodemenu;
    eduimenu_s *edptl_switch_menu;
    eduimenu_s *edptl_switchtype_menu;
    eduimenu_s *edptl_sounds_menu;
    eduimenu_s *edptl_soundx_menu;
    eduimenu_s *edptl_soundid_menu;
    eduimenu_s *edptl_soundcontrol_menu;
    eduimenu_s *edptl_bounce_menu;
    eduimenu_s *edptl_group_menu;
    eduimenu_s *edptl_ghost_menu;
    eduimenu_s *edptl_star_menu;
    eduimenu_s *edptl_page_menu;
    eduimenu_s *edptl_drawflag_menu;
    eduimenu_s *edptl_scaleeffect_menu;
    eduimenu_s *edptl_orphanlist_menu;
    eduimenu_s *edptl_instancesettings_menu;
    eduimenu_s *edptl_torus_menu;
    eduimenu_s *edptl_damage_menu;
    eduimenu_s *edptl_damageflag_menu;
    eduimenu_s *edptl_repeatbox_menu;
    eduimenu_s *edptl_quickdel_menu;
    eduimenu_s *edptl_detail_menu;
    eduimenu_s *edptl_testdetail_menu;
    i32 edpp_emitrotz;
    i32 edpp_emitroty;
    i32 edpp_emitrotx;
    i32 edpp_rotz;
    i32 edpp_roty;
    f32 edpp_offset;
    i32 edpp_refrotz;
    i32 edpp_refroty;
    i32 edpp_copyrotz;
    i32 edpp_copyroty;
    i32 edpp_facrotx;
    i32 edpp_facroty;

    void edgraInitAllClumps(void);
}

static void edgraInit() {
    edgraClumpsReset();

    edgra_mtl = NuMtlCreate3D(1);
    edgra_mtl->diffuse_color = {0.5f, 0.5f, 0.5f};
    edgra_mtl->opacity = 1.0f;
    auto *material_attributes = reinterpret_cast<u8 *>(&edgra_mtl->attribs);
    material_attributes[0] &= 0xf0;
    material_attributes[1] = (material_attributes[1] & 0x0f) | 0x20;
    NuMtlUpdate(edgra_mtl);

    edgra_mtl_zoff = NuMtlCreate(1);
    edgra_mtl_zoff->diffuse_color = {0.5f, 0.5f, 0.5f};
    edgra_mtl_zoff->opacity = 1.0f;
    material_attributes = reinterpret_cast<u8 *>(&edgra_mtl_zoff->attribs);
    material_attributes[0] &= 0xf0;
    material_attributes[1] = (material_attributes[1] & 0xcf) | 0xe0;
    NuMtlUpdate(edgra_mtl_zoff);

    if (gra_ptr.addr == 0) {
        edgra_mtxbuffer = static_cast<NUMTX *>(NU_ALLOC(0xc0080, 4, 1, "", 0));
        edgra_vecbuffer = static_cast<NUVEC *>(NU_ALLOC(0x24000, 4, 1, "", 0));
    } else {
        usize aligned = (gra_ptr.addr + 15) & ~usize(15);
        edgra_mtxbuffer = reinterpret_cast<NUMTX *>(aligned);
        edgra_vecbuffer = reinterpret_cast<NUVEC *>(aligned + 0xc0080);
        gra_ptr.addr = aligned + 0xe4080;
    }
    for (i32 index = 0; index < EDGRA_MAX_CLUMPS; ++index)
        GrassClumps[index].vector_buffer = NULL;
    edgra_free_vecbuffer = edgra_vecbuffer;
    edgra_instance_type = -1;
}

static void edgraClose() {
    if (edgra_options_menu) {
        eduiMenuDestroy(edgra_options_menu);
        edgra_options_menu = NULL;
    }
}

static void edgraEnter() {
    edgra_nearest = -1;
    edgra_size = 0.5f;
    edgra_clump_size = 64;
}

static i32 edgraProc(f32 delta_time, nupad_s *pad) {
    if (edgra_active_menu) {
        eduiMenuProcess(edgra_active_menu, delta_time, pad);
        return 0;
    }
    edgraDoInput(pad);
    if (edgra_editormode == 1)
        edgraDetermineNearestInstance(1.0f);
    else
        edgraDetermineNearestClump(1.0f);
    return (pad->digital_buttons_pressed >> 11) & 1;
}

static void edgraRender() {
    edcamSet();
    edgraDrawCursor();
    if (edgra_nearest != -1) {
        edgra_clump_s &clump = GrassClumps[edgra_nearest];
        if (clump.kind == 3) {
            if (edgra_editormode == 1) {
                edbitsDrawDiagonalCross(clump.position.x, clump.position.y, clump.position.z, 0.125f, 0xffff0000,
                                        edgra_mtl);
                if (edgra_nearest_instance != -1) {
                    edgra_individual_s *individual = GetIndGrassClump(clump.individual_index, edgra_nearest_instance);
                    edbitsDrawCross(clump.position.x + individual->position.x,
                                    clump.position.y + individual->position.y,
                                    clump.position.z + individual->position.z, 0.125f, 0xff80ffff, edgra_mtl);
                }
            } else {
                edbitsDrawDiagonalCross(clump.position.x, clump.position.y, clump.position.z, 0.125f, 0xff80ffff,
                                        edgra_mtl);
            }
        } else {
            edbitsDrawDiagonalCross(clump.position.x, clump.position.y, clump.position.z, 0.125f, 0xff80ffff,
                                    edgra_mtl);
            if (static_cast<u8>(clump.unknown_25 - 3) < 2) {
                edbitsDrawCube(clump.position.x, clump.position.y, clump.position.z, clump.size, 0.0f, clump.size, 0, 0,
                               0, clump.rotation_z, clump.rotation_y, 0xff80ffff, edgra_mtl);
            } else {
                edbitsDrawCircleTilted(&clump.position, clump.size, 0xff80ffff, edgra_mtl, clump.rotation_z,
                                       clump.rotation_y);
            }
        }
    }
    if (edgra_active_menu)
        eduiMenuRender(edgra_active_menu);
}

static void edppInit() {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edpp_first_time_this_level = 1;

    edpp_mtl = NuMtlCreate3D(1);
    edpp_mtl->diffuse_color = {0.5f, 0.5f, 0.5f};
    edpp_mtl->opacity = 1.0f;
    u8 *attributes = reinterpret_cast<u8 *>(&edpp_mtl->attribs);
    attributes[0] &= 0xf0;
    attributes[1] = (attributes[1] & 0x0f) | 0x20;
    NuMtlUpdate(edpp_mtl);

    edpp_boxmtl = NuMtlCreate(1);
    edpp_boxmtl->diffuse_color = {0.5f, 0.5f, 0.5f};
    edpp_boxmtl->opacity = 1.0f;
    attributes = reinterpret_cast<u8 *>(&edpp_boxmtl->attribs);
    attributes[0] &= 0xf0;
    attributes[1] = (attributes[1] & 0xcf) | 0xe0;
    NuMtlUpdate(edpp_boxmtl);

    edpp_nearest = -1;
    edpp_curr = -1;
    edpp_create_type = -1;
    edpp_effect_list = 0;
    edpp_emitrotz = 0;
    edpp_emitroty = 0;
    edpp_emitrotx = 0;

    ptlgcodemenu = eduiMenuCreate(70, 70, 180, 250, ed_fnt, NULL, "GenCode Type");
    if (ptlgcodemenu != NULL) {
        eduiMenuAddItem(ptlgcodemenu, eduiItemCheckCreate(1, colours, 0, 1, cbPtlSelGCode, "None"));
        eduiMenuAddItem(ptlgcodemenu, eduiItemCheckCreate(2, colours, 0, 1, cbPtlSelGCode, "Pos"));
        eduiMenuAddItem(ptlgcodemenu, eduiItemCheckCreate(3, colours, 0, 1, cbPtlSelGCode, "PosRev"));
        eduiMenuAddItem(ptlgcodemenu, eduiItemCheckCreate(4, colours, 0, 1, cbPtlSelGCode, "Splash"));
        eduiMenuAddItem(ptlgcodemenu, eduiItemCheckCreate(5, colours, 0, 1, cbPtlSelGCode, "AshRock"));
        eduiMenuAddItem(ptlgcodemenu, eduiItemCheckCreate(6, colours, 0, 1, cbPtlSelGCode, "PosRevTree"));
        eduiMenuAddItem(ptlgcodemenu, eduiItemCheckCreate(7, colours, 0, 1, cbPtlSelGCode, "PosAll"));
    }

    ptloptmenu = eduiMenuCreate(70, 70, 250, 300, ed_fnt, cbPtlCancel, "Options");
    if (ptloptmenu == NULL)
        return;
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbEffectListMenu, "Effect List..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlTypeMenu, "Particle Type..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlDataMenu, "Data Menu..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlGSortMenu, "GenSort Type..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlColMenu, "Particle Colour..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlTextureMenu, "Particle Texture..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlEmitMenu, "Emitter Settings..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlSizeMenu, "Particle Size..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlRotMenu, "Particle Rotation..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlJibMenu, "Particle Jibber..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbBounceMenu, "Particle Bounces..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbSoundsMenu, "Attached Sounds..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlDamageMenu, "Particle Damage..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlInstanceSettingsMenu, "Instance Settings..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, cbPtlSScaleMenu, "Super Scale..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbDpadModeMenu, "Dpad Mode..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbSwitchMenu, "Switch Menu..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbGroupMenu, "Render Settings..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbDetailMenu, "Detail Level Settings..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbPageMenu, "Test Page Menu..."));
    eduiMenuAddItem(ptloptmenu, eduiItemSelCreate(1, colours, 0, 0, edptlcbResetParticles, "Reset Particles"));
    eduiMenuAddItem(ptloptmenu, eduiItemToggleCreate(1, colours, 0, 1, cbPtlSnapToggle, "Snap"));
    eduiMenuAddItem(ptloptmenu,
                    eduiItemSelCreate(1, colours, 0, 0, edptlcbJumpToGameLocation, "Jump To Game Location"));
}

static void edppClose() {
    eduiMenuDestroy(ptloptmenu);
    eduiMenuDestroy(ptlgcodemenu);
}

static void edppEnter() {
    NUVEC origin = {0.0f, 0.0f, 0.0f};
    if (edmainQueryLocVec() != NULL) {
        entry_position = *edmainQueryLocVec();
    } else {
        entry_position.x = global_camera.mtx.m30;
        entry_position.y = global_camera.mtx.m31;
        entry_position.z = global_camera.mtx.m32;
    }
    if (edpp_first_time_this_level != 0) {
        if (edmainQueryLocVec() != NULL)
            edcamSetPosAng(edmainQueryLocVec(), 0, 0);
        else
            edcamSetPosAng(&origin, 0, 0);
        edpp_first_time_this_level = 0;
    }
    edpp_nearest = -1;
    edpp_create_type = -1;
}

static void edppApply() {
}

static i32 edppProc(f32 delta_time, nupad_s *pad) {
    edptl_count += 5;
    if (edpp_active_menu != NULL) {
        eduiMenuProcess(edpp_active_menu, delta_time, pad);
        return 0;
    }
    edppDoInput(pad);
    edppDetermineNearest(1.0f);
    if (edpp_nearest != -1)
        DebrisStatusNormal(&edpp_ptls[edpp_nearest].instance_id);
    return (pad->digital_buttons_pressed >> 11) & 1;
}

static void edppRender() {
    edcamSet();
    edppDrawCursor();
    edppHighlightNearest();
    if (edpp_active_menu != NULL)
        eduiMenuRender(edpp_active_menu);
}

void edbriDoInput(nupad_s *);
void edbriDetermineNearest(f32);
void edbriDrawCursor();
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}

static void edbriInit() {
    edbriBridgesReset();
    edbri_mtl = NuMtlCreate3D(1);
    edbri_mtl->diffuse_color = {0.5f, 0.5f, 0.5f};
    edbri_mtl->opacity = 1.0f;
    u8 *attributes = reinterpret_cast<u8 *>(&edbri_mtl->attribs);
    attributes[0] &= 0xf0;
    attributes[1] = (attributes[1] & 0x0f) | 0x20;
    NuMtlUpdate(edbri_mtl);
    edbri_mtl_zoff = NuMtlCreate(1);
    edbri_mtl_zoff->diffuse_color = {0.5f, 0.5f, 0.5f};
    edbri_mtl_zoff->opacity = 1.0f;
    attributes = reinterpret_cast<u8 *>(&edbri_mtl_zoff->attribs);
    attributes[0] &= 0xf0;
    attributes[1] = (attributes[1] & 0xcf) | 0xe0;
    NuMtlUpdate(edbri_mtl_zoff);
    edbri_plank_instance_type = -1;
    edbri_post_instance_type = -1;
}

static void edbriClose() {
    if (edbri_options_menu) {
        eduiMenuDestroy(edbri_options_menu);
        edbri_options_menu = NULL;
    }
}

static i32 edbriProc(f32 delta_time, nupad_s *pad) {
    if (edbri_active_menu) {
        eduiMenuProcess(edbri_active_menu, delta_time, pad);
        return 0;
    }
    edbriDoInput(pad);
    edbriDetermineNearest(1.0f);
    return (pad->digital_buttons_pressed >> 11) & 1;
}

static void edbriRender() {
    edcamSet();
    edbriDrawCursor();
    if (edbri_nearest != -1) {
        const NUVEC &position = edBridges[edbri_nearest].position;
        edbitsDrawDiagonalCross(position.x, position.y, position.z, 0.125f, 0xff80ffff, edbri_mtl_zoff);
    }
    if (edbri_active_menu)
        eduiMenuRender(edbri_active_menu);
}

static void edanimInit() {
    u32 colours[4] = {0x80000000, 0x80ff0000, 0x80808080, 0x80404040};
    edanimParamReset();

    edanim_mtl = NuMtlCreate3D(1);
    edanim_mtl->diffuse_color = {0.5f, 0.5f, 0.5f};
    edanim_mtl->opacity = 1.0f;
    edanim_mtl->attribs.alpha_mode = 0;
    edanim_mtl->attribs.cull_mode = 2;
    edanim_mtl->attribs.z_mode = 0;
    NuMtlUpdate(edanim_mtl);

    edanim_mtl_zoff = NuMtlCreate(1);
    edanim_mtl_zoff->diffuse_color = {0.5f, 0.5f, 0.5f};
    edanim_mtl_zoff->opacity = 1.0f;
    edanim_mtl_zoff->attribs.alpha_mode = 0;
    edanim_mtl_zoff->attribs.cull_mode = 2;
    edanim_mtl_zoff->attribs.z_mode = 3;
    NuMtlUpdate(edanim_mtl_zoff);

    edanim_options_menu =
        eduiMenuCreate(70, 70, 220, 300, ed_fnt, edanimcbCancelOptMenu, const_cast<char *>("Options"));
    if (!edanim_options_menu) {
        return;
    }

    eduiMenuAddItem(edanim_options_menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbParticleMenu,
                                                           const_cast<char *>("Attached Particles...")));
    eduiMenuAddItem(edanim_options_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbSoundMenu, const_cast<char *>("Attached Sounds...")));
    eduiMenuAddItem(edanim_options_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbBouncyMenu, const_cast<char *>("Bounciness...")));
    eduiMenuAddItem(edanim_options_menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbSwitchMenu,
                                                           const_cast<char *>("Switch Properties...")));
    eduiMenuAddItem(edanim_options_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbFileSave, const_cast<char *>("Save Params")));
    eduiMenuAddItem(edanim_options_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbFileLoad, const_cast<char *>("Load Params")));
    eduiMenuAddItem(edanim_options_menu,
                    eduiItemSelCreate(1, colours, 0, 0, edanimcbCubeMap, const_cast<char *>("Dump Cube Map")));
    eduiMenuAddItem(edanim_options_menu, eduiItemSelCreate(1, colours, 0, 0, edanimcbMCTBMenu,
                                                           const_cast<char *>("Memory Card Test Bed...")));
}

static void edanimClose() {
    eduiMenuDestroy(edanim_options_menu);
}

static i32 edanimProc(f32 delta_time, nupad_s *pad) {
    if (edanim_active_menu) {
        eduiMenuProcess(edanim_active_menu, delta_time, pad);
        return 0;
    }

    edanimDoInput(pad);
    if (edanim_particle_mode) {
        edanimDetermineNearestParticle(1.0f);
    } else if (edanim_sound_mode) {
        edanimDetermineNearestSound(1.0f);
    } else {
        edanimDetermineNearestAnim(1.0f);
    }

    if (pad->digital_buttons_pressed & 0x800) {
        edanim_particle_mode = 0;
        return 1;
    }
    return 0;
}

static void edanimRender() {
    if (edbitsProcessCubemapDump()) {
        return;
    }
    edcamSet();
    edanimDrawCursor();

    if (edanim_nearest != -1) {
        i32 colour = 0xffff0000;
        if (edanim_particle_mode) {
            colour = 0xff00ff00;
        } else if (!edanim_sound_mode) {
            colour = edanim_nearest_param_id == -1 ? 0xff808080 : 0xffffffff;
        }

        nuhspecial_s special;
        NuGScnGetSpecial(&special, edbits_base_scene, edanim_nearest);
        NUVEC *position = NuSpecialGetPos(&special);
        edbitsDrawCube(position->x, position->y, position->z, 0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 0, colour, edanim_mtl);

        if (edanim_particle_mode) {
            edanimRenderParticleEmitters(edanim_nearest_param_id);
        } else if (edanim_sound_mode) {
            edanimRenderSoundEmitters(edanim_nearest_param_id);
        }
    }

    if (edanim_active_menu) {
        eduiMenuRender(edanim_active_menu);
    }
}

void edgraClumpReseed(i32 index) {
    edgra_clump_s *clump = &GrassClumps[index];
    clump->seed = NuRand(NULL);
    edgraInitAllClumps();
}

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element) {
    return &IndGrassClumps[clump * EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP + element];
}

void edgraClumpDestroy(i32 index) {
    edgra_clump_s *clump = &GrassClumps[index];
    clump->element_count = 0;
    edgra_free_vecbuffer = clump->vector_buffer;
    clump->vector_buffer = NULL;
    --edgra_clumps_used;
    if (clump->kind == 3) {
        IndGrassClumpsUsed[clump->individual_index] = 0;
        --edgra_ind_clumps_used;
    }
    edgraInitAllClumps();
}
void edbitsDoSingleDump(i32 face);
void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);

static NUVEC *ed_loc;

extern "C" {
    extern debinftype *effecttypes;
    extern debinftype **debtab;
    extern i32 EDPP_MAX_TYPES;
    i32 edbitsLookupSoundFX(char *name);
    void edbitsSoundPlay(NUVEC *position, i32 sound);
    void edanimSoundDestroy(i32 parameter_index, i32 sound_index);
    void AddVariableShotDebrisEffectTimed3(i32, NUVEC *, NUVEC *, i32, f32, NUMTX *, NUMTX *);
    extern part_type_s part_types[128];
    extern i32 part_types_used;
    extern i32 part_emits_used;
    extern NUGSCN *part_scene[32];
    extern i32 part_scene_pageid[32];
    void PlatInstBounce(i32, f32, f32, f32);
    void CheckPartCount(void);
    void KillPartsByScene(NUGSCN *);
    extern i32 DEBPAGE_GENERAL;
    extern i32 DEBPAGE_CHARACTER;
    extern i32 DEBPAGE_AREA;
    part_emit_s part_emits[512];
    i32 part_page_on[8];
    i32 part_page_used[8];
    i32 edpart_instances_used;
    edanim_param_s AnimParams[64];
    eduimenu_s *edanim_active_menu;
    eduimenu_s *edanim_options_menu;
    eduimenu_s *edanim_particle_menu;
    eduimenu_s *edanim_particletype_menu;
    eduimenu_s *edanim_localparticletype_menu;
    eduimenu_s *edanim_localparticle_menu;
    eduimenu_s *edanim_switch_menu;
    eduimenu_s *edanim_switchtype_menu;
    eduimenu_s *edanim_sound_menu;
    eduimenu_s *edanim_soundtype_menu;
    eduimenu_s *edanim_localsoundtype_menu;
    eduimenu_s *edanim_localsound_menu;
    eduimenu_s *edanim_bouncy_menu;
    eduimenu_s *edanim_mctb_menu;
    NUGSCN *edanim_page_scene[8];
    i32 edanim_page_used[8];
    i32 edanim_page_on[8];
    NUMTL *edanim_mtl;
    NUMTL *edanim_mtl_zoff;
    NUVEC edanim_cam_pos;
    f32 edanim_cam_dist;
    i32 edanim_cam_ax;
    i32 edanim_cam_ay;
    i32 edanim_nearest;
    i32 edanim_nearest_param_id;
    i32 edanim_nearest_particle;
    i32 edanim_nearest_sound;
    i32 edanim_next_param;
    i32 edanim_particle_mode;
    i32 edanim_particle_type;
    i32 edanim_sound_mode;
    i32 edanim_sound_type;
    i32 edanim_params_used;
    i32 edanim_emitrotz;
    i32 edanim_emitroty;
}

u8 object_switches[0x80];

extern "C" {
    u8 edbits_what_game;
    char edbits_level_filename[256];
    char edbits_general_save_directory[256];
    char edbits_general_save_name[256];
    char edbits_general_save_extension[256];
    char edbits_level_save_directory[256];
    char edbits_level_save_name[256];
    char edbits_level_save_extension[256];
    char edbits_datapath[256];
    i32 edSfxAllCount;
    NUGSCN *edbits_base_scene;
    NUGSCN *edbits_things_scene;
    void *edbits_base_terrain;
    i32 edbits_particle_general_page;
    i32 edbits_anim_page;
    i32 edbits_editmode;
    i32 edbits_part_general_page;
    NUCAMERA *cubemapcam;
    i32 edbits_types_error;
    i32 edbits_duplicates_error;
    EDBITS_GAME_SOUND edbitsGameSound[320];
    i32 edbits_numsounds;
    EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
    EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
    i32 edbits_override_backups;
    eduimenu_s *edbri_active_menu;
    eduimenu_s *edbri_options_menu;
    eduimenu_s *edbri_dpadmode_menu;
    eduimenu_s *edbri_plankinstance_menu;
    eduimenu_s *edbri_postinstance_menu;
    eduimenu_s *edbri_plankcount_menu;
    eduimenu_s *edbri_bridgeproperties_menu;
    eduimenu_s *edbri_ropecolour_menu;
    NUMTL *edbri_mtl;
    NUMTL *edbri_mtl_zoff;
    NUVEC edbri_cam_pos;
    f32 edbri_cam_dist;
    i32 edbri_cam_ax;
    i32 edbri_cam_ay;
    i32 edbri_nearest;
    i32 edbri_plank_instance_type;
    i32 edbri_post_instance_type;
    i32 edbri_bridges_used;
    i32 edbri_page_used[8];
    i32 edbri_page_on[8];
    i32 edbri_pageid;
    i32 edbri_rotz;
    i32 edbri_roty;
    i32 edbri_mode;
    NUGSCN *edbri_page_scene[8];
    edbridge_s edBridges[64];
    f32 edbri_length = 1.0f;
    f32 edbri_width = 0.5f;
    i32 edbri_planks = 11;
    i32 edbri_post_interval = 5;
}

static void edbriEnter() {
    edbri_nearest = -1;
}

static void edanimEnter() {
    edanim_nearest = -1;
    edanim_nearest_param_id = -1;
    edanim_nearest_particle = -1;
    edanim_particle_mode = 0;
    edanim_particle_type = -1;
    edanim_sound_type = -1;
}

void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);
extern "C" void NuBridgeRemove(i32 bridge);
extern "C" i32 NuBridgeCreate(NUGSCN *, nuhspecial_s *, nuhspecial_s *, NUVEC *, NUVEC *, f32, i16, f32, f32, f32, f32,
                              i32, f32, f32, i32, u32);

void edbriBridgeUpdate(i32 index, NUGSCN *scene) {
    if (edBridges[index].instance_id != -1) {
        NuBridgeRemove(edBridges[index].instance_id);
        edBridges[index].instance_id = -1;
    }
    NUVEC end;
    NUVEC delta;
    delta.x = edBridges[index].length;
    delta.y = 0.0f;
    delta.z = 0.0f;
    NuVecRotateZ(&delta, &delta, edBridges[index].rotation_z);
    NuVecRotateY(&delta, &delta, edBridges[index].rotation_y);
    NuVecAdd(&end, &edBridges[index].position, &delta);
    nuhspecial_s first = {};
    u32 colour = edBridges[index].red + ((edBridges[index].green << 8) + (edBridges[index].blue << 16) + 0x80000000u);
    if (edBridges[index].special_20 != -1)
        NuGScnGetSpecial(&first, edbits_base_scene, edBridges[index].special_20);
    nuhspecial_s second = {};
    if (edBridges[index].special_24 != -1)
        NuGScnGetSpecial(&second, edbits_base_scene, edBridges[index].special_24);
    edBridges[index].instance_id =
        NuBridgeCreate(scene, &first, &second, &edBridges[index].position, &end, edBridges[index].field_3c,
                       edBridges[index].rotation_y, edBridges[index].field_28, edBridges[index].field_2c,
                       edBridges[index].field_30, edBridges[index].field_34, edBridges[index].field_1d,
                       edBridges[index].field_14, edBridges[index].field_38, edBridges[index].field_1e, colour);
}

void edbriBridgePlace(i32 index, NUVEC *position) {
    edbridge_s *bridge = &edBridges[index];
    bridge->position = *position;
    bridge->rotation_z = edbri_rotz;
    bridge->rotation_y = edbri_roty;
    bridge->length = edbri_length;
    bridge->field_14 = edbri_width;
    bridge->field_1d = edbri_planks;
    bridge->field_1e = edbri_post_interval;
    bridge->special_20 = edbri_plank_instance_type;
    bridge->special_24 = edbri_post_instance_type;
    edbriBridgeUpdate(index, edbits_base_scene);
}

i32 edbriBridgeCreate(NUVEC *position) {
    if (edbri_bridges_used == 64)
        return -1;
    i32 index = 0;
    while (edBridges[index].connection_index != 0xff)
        ++index;
    edbridge_s *bridge = &edBridges[index];
    bridge->connection_index = edbri_pageid;
    ++edbri_bridges_used;
    bridge->field_28 = 0.18f;
    bridge->field_2c = 0.1f;
    bridge->field_30 = -0.01f;
    bridge->field_34 = 4.0f;
    bridge->field_38 = 0.5f;
    bridge->field_3c = 0.5f;
    bridge->red = 0x50;
    bridge->green = 0x50;
    bridge->blue = 0x30;
    bridge->field_43 = 0x80;
    edbri_page_used[edbri_pageid] = 1;
    edbri_page_scene[edbri_pageid] = edbits_base_scene;
    edbriBridgePlace(index, position);
    return index;
}

void edbriBridgeDestroy(i32 index) {
    edbridge_s *bridge = &edBridges[index];
    NuBridgeRemove(bridge->instance_id);
    bridge->connection_index = 0xff;
    bridge->instance_id = -1;
    --edbri_bridges_used;
}

void edanimDetermineNearestAnim(f32);
void edppDetermineNearest(float);
void edppPtlDestroy(i32);
void edppPtlShelve(i32);
extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern i32 maxdebkeys;
    void DebFreeOrphansInstantly(debinftype *);
    void DebFreeInstantly(i32 *);
    void AddDebrisEffect(i32 *, i32, f32, f32, f32);
    void DebrisOrientation(i32, i16, i16);
    void DebrisEmitterOrientation(i32, i16, i16, i16);
    void DebrisStartOffset(i32, f32);
    void DebrisSetTrigger(i32, i32, i32, f32);
    void DebrisReflectionOrientation(i32, i16, i16, f32, f32);
    void DebrisSetFacing(i32, u8, i16, i16);
    void DebrisSetGroupID(i32, i16);
    void DebrisSetRoomID(i32, nugscn_s *);
}

extern "C" void do_Pad_Standard_camera(edcam_s *camera, f32 delta_time, nupad_s *pad);
extern "C" void do_maya_mouse_camera(edcam_s *camera);
extern "C" void do_mouse_flymode_camera(edcam_s *camera, f32 delta_time);
extern "C" i32 NuKeyboard(i32 key);

static edcam_s gp_cam = {
    {0.0f, 0.0f, 0.0f},
    0,
    0,
    -2.0f,
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f},
    1,
    1,
    {0.0f, 0.0f, 0.0f},
    0,
    0,
    {0.0015625f, 0.0015625f, 0.0015625f},
    2,
    2,
    0.0078125f,
    0.5f,
    0.2f,
    0.2f,
    4.0f,
    0.15f,
    0.2f,
    0.01f,
    0.1f,
    EDCAM_FREEDOM_POSITION_X | EDCAM_FREEDOM_POSITION_Y | EDCAM_FREEDOM_POSITION_Z | EDCAM_FREEDOM_PITCH |
        EDCAM_FREEDOM_YAW | EDCAM_FREEDOM_DISTANCE,
    {0, 0, 0},
};

void edcamSetContoller(i32 invert_pitch) {
    gp_cam.invert_pad_pitch = invert_pitch;
}

static NUCAMERA *edmaincam = NULL;
static NUCAMERA *edinternalcam = NULL;
static eduimenu_s *active_menu;
static eduimenu_s *default_active_menu;
static eduimenu_s *processing_menu;
static f32 edui_cursor_x = 50.0f;
static f32 edui_cursor_y = 50.0f;
static f32 edui_cursor_dx;
static f32 edui_cursor_dy;
static i32 edui_cursor_locked;
static u32 edui_cursor_buttons;
static u32 edui_cursor_buttons_old;
static u32 edui_cursor_buttons_db;
static f32 slider_acceleration;

static i32 numInteracts;
static eduimenu_s *ed_main_menu;
static eduimenu_s *ed_cfg_menu;
static i32 ed_init;
static ed_module_s *ed_list;
static i32 ed_module_active;
static ed_module_s *ed_curr;
static edui_interact_s eduiInteracts[64];
static edui_interact_s *eduiInteractLocked;

extern "C" {
    static void cbMMCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *, u32);
    static void cbmcfgCancel(eduimenu_s *, eduimenu_s *);
    static void cbMMRegSel(eduimenu_s *, eduiitem_s *, u32);
    static void cbMMEditorConfig(eduimenu_s *, eduiitem_s *, u32);
    static i32 cbInteractMenuCancelChild(edui_interact_s *);
    extern i32 edmain_auto_speed;
    void cbSetAutoSpeed(eduimenu_s *, eduiitem_s *, u32);
}

extern "C" {
    i32 PadFlyMode = 0;
    NUMTX *ed_remap_mtx = NULL;
    i32 edmain_cursor_enabled = 0;

    eduimenu_s *edGetMainMenu(void) {
        return ed_main_menu;
    }
    void edGraDisableTerrainSwap(void) {
    }
    void edGraEnableTerrainSwap(void) {
    }
    void edGraInitTerrainSwapProtection(void) {
    }
    i32 edanimLoadPage(char *path, NUGSCN *scene) {
        i32 page;
        if (edanim_page_used[0] == 0)
            page = 0;
        else if (edanim_page_used[1] == 0)
            page = 1;
        else if (edanim_page_used[2] == 0)
            page = 2;
        else if (edanim_page_used[3] == 0)
            page = 3;
        else if (edanim_page_used[4] == 0)
            page = 4;
        else if (edanim_page_used[5] == 0)
            page = 5;
        else if (edanim_page_used[6] == 0)
            page = 6;
        else if (edanim_page_used[7] == 0)
            page = 7;
        else
            return -1;
        EdFileSetMedia(1);
        if (EdFileOpen(path, NUFILE_READ) == 0)
            return -1;
        EdFileSetReadWrongEndianess(1);
        i32 version = EdFileReadInt();
        if (version > 6) {
            EdFileSetReadWrongEndianess(0);
            EdFileClose();
            return -1;
        }
        i32 count = EdFileReadInt();
        if (count + edanim_params_used > 64)
            count = 64 - edanim_params_used;
        i32 slot = 0;
        for (i32 index = 0; index < count; ++index) {
            while (AnimParams[slot].instance_id != -1 && slot < 64)
                ++slot;
            if (slot >= 64)
                continue;
            char name[20];
            EdFileRead(name, 20);
            edanim_param_s *param = &AnimParams[slot];
            param->instance_id = edanimLookupSpecial(name, scene);
            param->effect_count = EdFileReadInt();
            param->sound_count = version > 1 ? EdFileReadInt() : 0;
            param->field_00c = EdFileReadInt();
            param->field_010 = EdFileReadInt();
            param->field_014 = EdFileReadFloat();
            param->field_018 = EdFileReadFloat();
            if (param->effect_count > 8)
                param->effect_count = 8;
            for (i32 effect = 0; effect < param->effect_count; ++effect) {
                EdFileRead(param->effect_names[effect], 16);
                param->effect_ids[effect] = -1;
                if (version > 4) {
                    param->effect_intervals[effect] =
                        version == 5 ? static_cast<i32>(EdFileReadFloat()) : EdFileReadInt();
                } else {
                    i32 interval = EdFileReadInt();
                    param->effect_intervals[effect] = interval > 0 ? interval * 60 : interval == 0 ? 0 : -60 / interval;
                }
                param->effect_flags[effect] = EdFileReadInt();
                EdFileReadNuVec(reinterpret_cast<NUVEC *>(param->effect_positions[effect]));
                if (version > 2) {
                    param->effect_angles[effect] = EdFileReadShort();
                    param->effect_angle_ranges[effect] = EdFileReadShort();
                } else {
                    param->effect_angles[effect] = 0;
                    param->effect_angle_ranges[effect] = 0;
                }
            }
            param->field_17c = 0.99f;
            if (version > 1) {
                if (param->sound_count > 8)
                    param->sound_count = 8;
                for (i32 sound = 0; sound < param->sound_count; ++sound) {
                    EdFileRead(param->sound_names[sound], 16);
                    param->sound_ids[sound] = -1;
                    param->sound_flags[sound] = EdFileReadInt();
                    param->sound_values[sound] = EdFileReadFloat();
                    EdFileReadNuVec(reinterpret_cast<NUVEC *>(param->sound_positions[sound]));
                }
            }
            nuhspecial_s special;
            NuGScnGetSpecial(&special, scene, param->instance_id);
            param->platform_id = FindPlatInst(NuSpecialGetInstanceix(&special));
            if (version > 3) {
                param->bounce_impulse = EdFileReadFloat();
                param->bounce_spring = EdFileReadFloat();
                param->bounce_damping = EdFileReadFloat();
            } else {
                param->bounce_impulse = 0.0f;
                param->bounce_spring = 0.0f;
                param->bounce_damping = 0.0f;
            }
            if (param->platform_id != -1)
                PlatInstBounce(param->platform_id, param->bounce_impulse, param->bounce_spring, param->bounce_damping);
            param->page = static_cast<i8>(page);
            ++edanim_params_used;
        }
        EdFileSetReadWrongEndianess(0);
        EdFileClose();
        edanim_page_used[page] = 1;
        edanim_page_scene[page] = scene;
        edbits_anim_page = page;
        edanim_next_param = count;
        edanim_particle_mode = 0;
        edanim_nearest = -1;
        edanim_nearest_param_id = -1;
        edanimDetermineNearestAnim(1.0f);
        return page;
    }

    void edanimClearPage(i32 page) {
        if (edanim_page_on[page] != 0)
            edanimStopPage(page);
        for (i32 index = 0; index < 64; ++index) {
            if (AnimParams[index].page == page) {
                AnimParams[index].instance_id = -1;
                --edanim_params_used;
            }
        }
        edanim_page_used[page] = 0;
        edanim_page_scene[page] = NULL;
    }
    i32 edanimLookupSpecial(char *name, NUGSCN *scene) {
        if (scene != NULL) {
            nuhspecial_s special;
            for (i32 index = 0; index < NuGScnNumSpecials(scene); ++index) {
                NuGScnGetSpecial(&special, scene, index);
                if (NuStrNICmp(NuSpecialGetName(&special), name, 19) == 0)
                    return index;
            }
        }
        return -1;
    }
    void edanimParamReset(void) {
        for (i32 i = 0; i < 64; ++i) {
            AnimParams[i].instance_id = -1;
        }
        edanim_next_param = 0;
        edanim_params_used = 0;
        memset(edanim_page_used, 0, sizeof(edanim_page_used));
        memset(edanim_page_on, 0, sizeof(edanim_page_on));
    }
    void edanimParticleDestroy(i32 parameter_index, i32 particle_index) {
        edanim_param_s *parameters = AnimParams;
        for (i32 index = particle_index; index < parameters[parameter_index].effect_count - 1; ++index) {
            parameters[parameter_index].effect_ids[index] = parameters[parameter_index].effect_ids[index + 1];
            parameters[parameter_index].effect_intervals[index] =
                parameters[parameter_index].effect_intervals[index + 1];
            parameters[parameter_index].effect_flags[index] = parameters[parameter_index].effect_flags[index + 1];
            memcpy(parameters[parameter_index].effect_positions[index],
                   parameters[parameter_index].effect_positions[index + 1], sizeof(NUVEC));
            strcpy(parameters[parameter_index].effect_names[index],
                   parameters[parameter_index].effect_names[index + 1]);
        }
        --parameters[parameter_index].effect_count;
    }
    void edanimRegisterCubeDumpInfo(void) {
    }
    void edanimStartPage(i32 page) {
        if (edanim_page_used[page] != 0 && edanim_page_scene[page] != NULL && edanim_page_on[page] == 0)
            edanim_page_on[page] = 1;
    }
    void edanimStopPage(i32 page) {
        edanim_page_on[page] = 0;
    }
    void edanimUpdateObjects(float elapsed) {
        static i32 localframecount;
        float seconds;
        if (NuVideoGetMode() == 3)
            seconds = elapsed / 50.0f;
        else
            seconds = elapsed / 60.0f;
        for (i32 index = 0; index < 64; ++index) {
            edanim_param_s *param = &AnimParams[index];
            if (param->instance_id == -1 || edanim_page_on[param->page] == 0)
                continue;
            nuhspecial_s special;
            nuinstanim_s *animation = NULL;
            if (edanim_page_scene[param->page] != NULL) {
                NuGScnGetSpecial(&special, edanim_page_scene[param->page], param->instance_id);
                animation = NuSpecialGetInstAnim(&special);
                NuSpecialGetInstanceix(&special);
            }
            if (animation != NULL) {
                switch (param->field_00c) {
                    case 1:
                        animation->playing = 0;
                        if (param->field_010 != -1 && object_switches[param->field_010] != 0)
                            animation->playing = 1;
                        break;
                    case 2:
                        animation->repeating = 0;
                        if (param->field_010 != -1 && object_switches[param->field_010] != 0 && !animation->playing) {
                            animation->ltime = 1.0f;
                            animation->playing = 1;
                            animation->backwards = 0;
                            animation->waiting = 0;
                        }
                        break;
                    case 3:
                        animation->repeating = 1;
                        if (param->field_010 != -1 && object_switches[param->field_010] != 0)
                            animation->playing = 1;
                        break;
                    case 4:
                        animation->playing = param->field_014 > edanimPlayerAnimDistance(index);
                        break;
                    case 5:
                        animation->repeating = 0;
                        if (param->field_014 > edanimPlayerAnimDistance(index) && !animation->playing) {
                            animation->playing = 1;
                            animation->backwards = 0;
                            animation->waiting = 0;
                            animation->ltime = 1.0f;
                        }
                        break;
                    case 6:
                        animation->repeating = 1;
                        if (param->field_014 > edanimPlayerAnimDistance(index))
                            animation->playing = 1;
                        break;
                    case 10:
                        animation->ltime = 1.0f;
                        animation->playing = 0;
                        break;
                    case 11:
                        animation->playing = 1;
                        break;
                    case 12:
                        animation->playing = 1;
                        animation->repeating = 1;
                        break;
                }
                if (index == edanim_nearest_param_id && (edanim_particle_mode != 0 || edanim_sound_mode != 0))
                    animation->ltime = 1.0f;
            }
            if (NuSpecialGetVisibilityFn(&special) != NULL) {
                i32 particle_index = 0;
                while (particle_index < param->effect_count) {
                    i32 effect = param->effect_ids[particle_index];
                    if (effect != -1 && debtab[effect] != NULL) {
                        if (param->effect_flags[particle_index] == 0 || (animation != NULL && animation->playing)) {
                            NUMTX matrix;
                            NuMtxInvRSS(&matrix, NuSpecialGetMtx(&special));
                            NuMtxMul(&matrix, &matrix, NuSpecialGetDrawMtx(&special));
                            NUVEC position = *reinterpret_cast<NUVEC *>(param->effect_positions[particle_index]);
                            NuVecMtxTransform(&position, &position, &matrix);
                            NUMTX orientation = numtx_identity;
                            NuMtxRotateZ(&orientation, param->effect_angles[particle_index]);
                            NuMtxRotateY(&orientation, param->effect_angle_ranges[particle_index]);
                            NuMtxMul(&matrix, &orientation, &matrix);
                            AddVariableShotDebrisEffectTimed3(param->effect_ids[particle_index], &position, &nuvec_zero,
                                                              param->effect_intervals[particle_index], seconds, &matrix,
                                                              &numtx_identity);
                        }
                    } else if (param->effect_names[particle_index][0] != 0) {
                        param->effect_ids[particle_index] = LookupDebrisEffect(param->effect_names[particle_index]);
                        if (param->effect_ids[particle_index] == -1) {
                            edanimParticleDestroy(index, particle_index);
                            continue;
                        }
                    }
                    ++particle_index;
                }
                for (i32 sound_index = 0; sound_index < param->sound_count; ++sound_index) {
                    i32 sound = param->sound_ids[sound_index];
                    if (sound == -1) {
                        if (param->sound_names[sound_index][0] != 0) {
                            param->sound_ids[sound_index] = edbitsLookupSoundFX(param->sound_names[sound_index]);
                            if (param->sound_ids[sound_index] == -1) {
                                edanimSoundDestroy(index, sound_index);
                                --sound_index;
                            }
                        }
                        continue;
                    }
                    i32 play = -1;
                    if (param->sound_flags[sound_index] == 1) {
                        i32 interval = static_cast<i32>(param->sound_values[sound_index]);
                        if (static_cast<i32>(static_cast<float>(localframecount) + elapsed) / interval >
                            localframecount / interval)
                            play = sound;
                    } else if (animation != NULL) {
                        float frame = param->sound_values[sound_index];
                        if (animation->ltime >= frame && frame > param->field_17c)
                            play = sound;
                        if (animation->oscillate && frame >= animation->ltime && fabsf(param->field_17c) > frame)
                            play = sound;
                    }
                    if (play != -1) {
                        NUMTX matrix;
                        NuMtxInvRSS(&matrix, NuSpecialGetMtx(&special));
                        NuMtxMul(&matrix, &matrix, NuSpecialGetDrawMtx(&special));
                        // The original uses the completed particle-loop index here (0x35d332).
                        NUVEC position = *reinterpret_cast<NUVEC *>(param->sound_positions[particle_index]);
                        NuVecMtxTransform(&position, &position, &matrix);
                        edbitsSoundPlay(&position, play);
                    }
                }
            }
            if (animation != NULL)
                param->field_17c = animation->oscillate ? -animation->ltime : animation->ltime;
        }
        localframecount += static_cast<i32>(elapsed);
    }
    void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material) {
        edbitsDrawCube((minimum->x + maximum->x) * 0.5f, (minimum->y + maximum->y) * 0.5f,
                       (minimum->z + maximum->z) * 0.5f, (maximum->x - minimum->x) * 0.5f,
                       (maximum->y - minimum->y) * 0.5f, (maximum->z - minimum->z) * 0.5f, 0, 0, 0, 0, 0, colour,
                       material);
    }
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
    void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y) {
        edbitsDrawOvalTilted(centre, radius, radius, colour, material, rotation_z, rotation_y);
    }
    void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material) {
        edbitsDrawOvalXY(centre, radius, radius, colour, material);
    }
    void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material) {
        edbitsDrawOvalTilted(centre, radius_x, radius_z, colour, material, 0, 0);
    }
    void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material) {
        edbitsDrawCircleXY(centre, radius, colour, material);
        edbitsDrawCircleTilted(centre, radius, colour, material, 0x2000, 0);
        edbitsDrawCircleTilted(centre, radius, colour, material, 0x4000, 0);
        edbitsDrawCircleTilted(centre, radius, colour, material, 0x6000, 0);
        edbitsDrawCircleTilted(centre, radius, colour, material, 0x4000, 0x2000);
        edbitsDrawCircleTilted(centre, radius, colour, material, 0x4000, 0x4000);
        edbitsDrawCircleTilted(centre, radius, colour, material, 0x4000, 0x6000);
    }
    char *edbitsGetSoundName(i32) {
        return NULL;
    }
    i32 edbitsLookupInstance(char *name, NUGSCN *scene) {
        if (scene) {
            nuhspecial_s special;
            for (i32 index = 0; index < NuGScnNumSpecials(scene); ++index) {
                NuGScnGetSpecial(&special, scene, index);
                if (NuStrNICmp(NuSpecialGetName(&special), name, 19) == 0) {
                    return index;
                }
            }
        }
        return -1;
    }
    i32 edbitsLookupSound(char *name) {
        for (i32 index = 0; index < edbits_numsounds; ++index) {
            if (NuStrNICmp(edbitsGameSound[index].name, name, 15) == 0) {
                return index;
            }
        }
        return -1;
    }
    i32 edbitsLookupSoundFX(char *) {
        return -1;
    }
    i32 edbitsProcessCubemapDump(void) {
        if (edbits_cubecount == 0)
            return 0;
        if (cubemapcam == NULL)
            cubemapcam = NuCameraCreate();
        cubemapcam->fov = 1.5708f;
        cubemapcam->aspect = 1.0f;
        switch (edbits_cubecount) {
            case 60:
                edmainExtCamera(cubemapcam);
                edcamSetDist(0.0f);
                edcamSetAng(0, 0x4000);
                edcamSet();
                break;
            case 55:
                edbitsDoSingleDump(0);
                break;
            case 50:
                edcamSetAng(0, 0xc000);
                edcamSet();
                break;
            case 45:
                edbitsDoSingleDump(1);
                break;
            case 40:
                edcamSetAng(-0x4000, 0);
                edcamSet();
                break;
            case 35:
                edbitsDoSingleDump(2);
                break;
            case 30:
                edcamSetAng(0x4000, 0);
                edcamSet();
                break;
            case 25:
                edbitsDoSingleDump(3);
                break;
            case 20:
                edcamSetAng(0, 0);
                edcamSet();
                break;
            case 15:
                edbitsDoSingleDump(4);
                break;
            case 10:
                edcamSetAng(0, 0x8000);
                edcamSet();
                break;
            case 5:
                edbitsDoSingleDump(5);
                break;
            case 1:
                edmainExtCamera(NULL);
                NuRndrDoingScreenGrab = 0;
                break;
        }
        return --edbits_cubecount;
    }
    void edbitsRegisterDataPath(char *path) {
        if (path) {
            NuStrCpy(edbits_datapath, path);
        } else {
            edbits_datapath[0] = '\0';
        }
    }
    void edbitsRegisterBaseScene(NUGSCN *scene) {
        edbits_base_scene = scene;
    }
    static i32 edbits_local_editor_enabled;
    i32 *edbits_editor_enabled = &edbits_local_editor_enabled;

    void edbitsRegisterEditMode(i32 mode) {
        edbits_editmode = mode;
    }
    void edbitsRegisterEditorEnabledFlag(i32 *enabled) {
        edbits_editor_enabled = enabled;
    }
    void edbitsRegisterLevel(char *filename, i32 game) {
        if (filename) {
            NuStrCpy(edbits_level_filename, filename);
        } else {
            edbits_level_filename[0] = '\0';
        }
        edbits_what_game = game;
    }
    void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback) {
        edbitsPlaySound = callback;
    }
    void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback) {
        edbitsRequestSound = callback;
    }
    void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension) {
        if (general_save_directory) {
            NuStrCpy(edbits_general_save_directory, general_save_directory);
        } else {
            edbits_general_save_directory[0] = '\0';
        }
        if (general_save_name) {
            NuStrCpy(edbits_general_save_name, general_save_name);
        } else {
            edbits_general_save_name[0] = '\0';
        }
        if (general_save_extension) {
            NuStrCpy(edbits_general_save_extension, general_save_extension);
        } else {
            edbits_general_save_extension[0] = '\0';
        }
        if (level_save_directory) {
            NuStrCpy(edbits_level_save_directory, level_save_directory);
        } else {
            edbits_level_save_directory[0] = '\0';
        }
        if (level_save_name) {
            NuStrCpy(edbits_level_save_name, level_save_name);
        } else {
            edbits_level_save_name[0] = '\0';
        }
        if (level_save_extension) {
            NuStrCpy(edbits_level_save_extension, level_save_extension);
        } else {
            edbits_level_save_extension[0] = '\0';
        }
    }
    void edbitsRegisterSoundEffect(char *name, i32 id) {
        i32 index = edbits_numsounds;
        EDBITS_GAME_SOUND *sound = &edbitsGameSound[index];
        strncpy(sound->name, name, 16);
        sound->name[15] = '\0';
        sound->id = id;
        edbits_numsounds = index + 1;
    }
    void edbitsRegisterThingsScene(NUGSCN *scene) {
        edbits_things_scene = scene;
    }
    void edbitsRegisterBaseTerrain(void *terrain) {
        edbits_base_terrain = terrain;
    }
    i32 edbitsSfxVol = 100;
    void edbitsSetSoundFxVolume(i32 volume) {
        edbitsSfxVol = volume;
    }
    i32 edbitsStartCubemapDump(void) {
        if (edbits_cubecount != 0)
            return 0;
        edbits_cubecount = 65;
        NuRndrDoingScreenGrab = 1;
        return 1;
    }
    void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z) {
        *rotation_y = -NuAtan2DA(z, x);
        *rotation_z = NuAtan2DA(y, NuFsqrt(x * x + z * z));
    }
    void edbriBridgesReset(void) {
        NuBridgeInit();
        for (i32 i = 0; i < 64; ++i) {
            edBridges[i].instance_id = -1;
            edBridges[i].connection_index = 0xff;
        }
        memset(edbri_page_used, 0, sizeof(edbri_page_used));
        memset(edbri_page_scene, 0, sizeof(edbri_page_scene));
        memset(edbri_page_on, 0, sizeof(edbri_page_on));
        edbri_bridges_used = 0;
    }
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
    void edbriClearPage(i8 page) {
        edbriStopPage(page);
        for (i32 i = 0; i < 64; ++i) {
            if (edBridges[i].connection_index == static_cast<u8>(page)) {
                edBridges[i].connection_index = 0xff;
                --edbri_bridges_used;
            }
        }
        edbri_page_used[page] = 0;
        edbri_page_scene[page] = 0;
    }
    void edbriStartPage(i32 page_number) {
        i8 page = static_cast<i8>(page_number);
        if (edbri_page_used[page] && edbri_page_scene[page]) {
            for (i32 i = 0; i < 64; ++i) {
                if (edBridges[i].connection_index == static_cast<u8>(page)) {
                    edbriBridgeUpdate(i, edbri_page_scene[page]);
                }
            }
        }
    }
    void edbriStopPage(i8 page) {
        if (edbri_page_used[page]) {
            for (i32 i = 0; i < 64; ++i) {
                if (edBridges[i].connection_index == static_cast<u8>(page) && edBridges[i].instance_id != -1) {
                    NuBridgeRemove(edBridges[i].instance_id);
                    edBridges[i].instance_id = -1;
                }
            }
        }
    }
    f32 edcamGetDist(void) {
        return gp_cam.distance;
    }
    edcam_s *edcamGetEdCam(void) {
        return &gp_cam;
    }
    void edcamGetOffset(NUVEC *offset) {
        *offset = gp_cam.offset;
    }
    void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw) {
        if (position != NULL) {
            *position = gp_cam.position;
        }
        if (pitch != NULL) {
            *pitch = gp_cam.pitch;
        }
        if (yaw != NULL) {
            *yaw = gp_cam.yaw;
        }
    }
    void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw) {
        if (position != NULL) {
            *position = gp_cam.snapped_position;
        }
        if (pitch != NULL) {
            *pitch = gp_cam.snapped_pitch;
        }
        if (yaw != NULL) {
            *yaw = gp_cam.snapped_yaw;
        }
    }
    NUVEC *edcamGetPosPointer(void) {
        return &gp_cam.position;
    }
    void edcamMove(nupad_s *pad) {
        edcamMoveEx(pad, NuTimeGetFrameTime());
    }
    void edcamMoveEx(nupad_s *pad, f32 delta_time) {
        if (edmainGetCursorEnabled() != 0) {
            if (PadFlyMode == 0 || NuKeyboard(0x38) != 0) {
                do_maya_mouse_camera(&gp_cam);
            } else {
                do_mouse_flymode_camera(&gp_cam, delta_time);
            }
        }
        if (pad != NULL) {
            if (PadFlyMode == 0) {
                do_Pad_Standard_camera(&gp_cam, delta_time, pad);
            } else {
                do_Pad_flymode_camera(&gp_cam, delta_time, pad);
            }
        }
    }
    void edcamMtx(NUMTX *matrix) {
        NUVEC distance = {0.0f, 0.0f, gp_cam.distance};
        NuMtxSetTranslation(matrix, &distance);
        NuMtxRotateX(matrix, gp_cam.pitch);
        NuMtxRotateY(matrix, gp_cam.yaw);
        NuMtxTranslate(matrix, &gp_cam.position);
        NuMtxTranslate(matrix, &gp_cam.offset);
        if (ed_remap_mtx != NULL) {
            NuMtxMul(matrix, matrix, ed_remap_mtx);
            ed_remap_mtx = NULL;
        }
    }
    void edcamSet(void) {
        NUMTX matrix;
        edcamMtx(&matrix);
        edmainSetCamera(&matrix);
    }
    void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance) {
        gp_cam.allow_position_x = position_x;
        gp_cam.allow_position_y = position_y;
        gp_cam.allow_position_z = position_z;
        gp_cam.allow_pitch = pitch;
        gp_cam.allow_yaw = yaw;
        gp_cam.allow_distance = distance;
    }
    void edcamSetAng(i32 pitch, i32 yaw) {
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }
    void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale) {
        gp_cam.auto_move_base = move_base;
        gp_cam.auto_move_dist_scale = move_distance_scale;
        gp_cam.auto_zoom_base = zoom_base;
        gp_cam.auto_zoom_dist_scale = zoom_distance_scale;
    }
    void edcamSetDist(f32 distance) {
        gp_cam.distance = distance;
    }
    void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement) {
        gp_cam.mouse_pitch_speed = pitch;
        gp_cam.mouse_yaw_speed = yaw;
        gp_cam.mouse_move_speed = movement;
    }
    void edcamSetOffset(NUVEC *offset) {
        gp_cam.offset = *offset;
    }
    void edcamSetPos(NUVEC *position) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
    }
    void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw) {
        gp_cam.position = *position;
        gp_cam.offset = {0.0f, 0.0f, 0.0f};
        gp_cam.pitch = pitch;
        gp_cam.yaw = yaw;
    }
    void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance) {
        gp_cam.position_speed = {position_x, position_y, position_z};
        gp_cam.distance_speed = distance;
    }
    void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z) {
        gp_cam.position_speed = {position_x, position_y, position_z};
    }
    i32 edgraBufferUsage(void) {
        i32 count = 0;
        for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i)
            count += GrassClumps[i].element_count;
        return count * 0x4c + 0x10;
    }
    i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end) {
        i32 page = -1;
        for (i32 index = 0; index < 8; ++index) {
            if (edgra_page_used[index] == 0) {
                page = index;
                break;
            }
        }
        if (page == -1) {
            return -1;
        }

        VARIPTR *buffer = static_cast<VARIPTR *>(buf);
        usize matrix_base = ALIGN(buffer->addr, 16);
        if (edbits_editmode == 1 && edgra_mtxbuffer != NULL) {
            matrix_base = reinterpret_cast<usize>(edgra_mtxbuffer);
        }
        edgra_page_matrix_stack[page] = reinterpret_cast<NUMTX *>(matrix_base);

        EdFileSetMedia(1);
        if (EdFileOpen(path, NUFILE_READ) == 0) {
            return -1;
        }
        EdFileSetReadWrongEndianess(1);
        const i32 version = EdFileReadInt();
        if (version < 1 || version > 9) {
            EdFileSetReadWrongEndianess(0);
            EdFileClose();
            return -1;
        }

        if (version == 9) {
            edgra_global_fadein = EdFileReadFloat();
            edgra_global_fadeout = EdFileReadFloat();
        } else {
            edgra_global_fadein = 15.0f;
            edgra_global_fadeout = 25.0f;
        }

        const i32 file_clump_count = EdFileReadInt();
        const i32 available = EDGRA_MAX_CLUMPS - edgra_clumps_used;
        const i32 clump_count = MIN(file_clump_count, available);
        const i32 matrix_count = version >= 8 ? EdFileReadInt() : 0x3000;
        usize vector_cursor = matrix_base + static_cast<usize>(matrix_count) * sizeof(NUMTX);
        if (edbits_editmode == 1 && edgra_vecbuffer != NULL) {
            vector_cursor = reinterpret_cast<usize>(edgra_vecbuffer);
        }

        i32 slot = 0;
        for (i32 file_index = 0; file_index < clump_count; ++file_index) {
            while (slot < EDGRA_MAX_CLUMPS && GrassClumps[slot].element_count != 0) {
                ++slot;
            }
            if (slot == EDGRA_MAX_CLUMPS) {
                break;
            }

            edgra_clump_s *clump = &GrassClumps[slot];
            char instance_name[20];
            EdFileRead(instance_name, sizeof(instance_name));
            clump->special_index = edbitsLookupInstance(instance_name, static_cast<NUGSCN *>(gscn));
            clump->element_count = EdFileReadInt();
            EdFileReadNuVec(&clump->position);
            clump->size = EdFileReadFloat();
            clump->field_18 = EdFileReadFloat();

            clump->flags = version == 1 ? 1 : EdFileReadInt();
            clump->field_20 = version <= 2 ? 1.0f : EdFileReadFloat();
            clump->page = static_cast<u8>(page);
            if (version < 5) {
                clump->unknown_25 = 1;
                clump->unknown_26 = 1;
                clump->kind = 1;
            } else {
                clump->unknown_25 = EdFileReadChar();
                clump->unknown_26 = EdFileReadChar();
                clump->kind = version == 5 ? 1 : EdFileReadChar();
            }

            if (version < 4) {
                clump->seed = 0;
                clump->field_2c = 0.2f;
                clump->field_30 = 1.7625f;
            } else {
                clump->seed = EdFileReadInt();
                clump->field_2c = EdFileReadFloat();
                clump->field_30 = EdFileReadFloat();
            }

            if (version < 5) {
                clump->rotation_z = 0;
                clump->rotation_y = 0;
            } else {
                clump->rotation_z = EdFileReadShort();
                clump->rotation_y = EdFileReadShort();
            }
            if (version < 6) {
                clump->near_distance = 15.0f;
                clump->far_distance = 25.0f;
            } else {
                clump->near_distance = EdFileReadFloat();
                clump->far_distance = EdFileReadFloat();
            }
            if (version < 7) {
                if (clump->kind == 1) {
                    clump->field_42 = 0;
                    clump->field_43 = 0;
                    clump->field_44 = 0.0f;
                    clump->near_distance = 20.0f;
                    clump->far_distance = 20.0f;
                } else if (clump->kind == 2) {
                    clump->field_42 = 1;
                    clump->field_43 = 1;
                    clump->field_44 = 0.05f;
                } else {
                    clump->field_42 = 0;
                    clump->field_43 = 0;
                    clump->field_44 = 0.0f;
                }
            } else {
                clump->field_42 = EdFileReadChar();
                clump->field_43 = EdFileReadChar();
                clump->field_44 = EdFileReadFloat();
            }

            i32 skipped_individuals = 0;
            if (clump->kind == 3) {
                i32 individual = 0;
                while (individual < EDGRA_MAX_INDIVIDUAL_CLUMPS && IndGrassClumpsUsed[individual] != 0) {
                    ++individual;
                }
                if (individual == EDGRA_MAX_INDIVIDUAL_CLUMPS) {
                    skipped_individuals = clump->element_count;
                    clump->element_count = 0;
                } else {
                    clump->individual_index = static_cast<i16>(individual);
                    IndGrassClumpsUsed[individual] = 1;
                    if (clump->element_count > EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP) {
                        skipped_individuals = clump->element_count - EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
                        clump->element_count = EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
                    }
                    for (i32 element = 0; element < clump->element_count; ++element) {
                        edgra_individual_s *blade = GetIndGrassClump(individual, element);
                        EdFileReadNuVec(&blade->position);
                        blade->field_0c = EdFileReadFloat();
                        blade->field_10 = EdFileReadShort();
                        blade->field_12 = EdFileReadShort();
                    }
                }
                for (i32 element = 0; element < skipped_individuals; ++element) {
                    EdFileReadFloat();
                    EdFileReadFloat();
                    EdFileReadFloat();
                    EdFileReadFloat();
                    EdFileReadShort();
                    EdFileReadShort();
                }
            }

            clump->vector_buffer = reinterpret_cast<void *>(vector_cursor);
            edgra_page_vectors_valid[page] = version >= 8;
            if (version >= 8) {
                NUVEC *vectors = static_cast<NUVEC *>(clump->vector_buffer);
                for (i32 element = 0; element < clump->element_count; ++element) {
                    EdFileReadNuVec(&vectors[element]);
                }
            }
            if (clump->element_count < 4) {
                clump->element_count = 4;
            }

            if (clump->special_index == -1) {
                clump->element_count = 0;
            }
            if (file_index % edgra_clumpthin != 0 && edbits_editmode == 0) {
                clump->element_count = 0;
            } else if (edgra_elementthin != 1 && edbits_editmode == 0) {
                clump->element_count = (clump->element_count + edgra_elementthin - 1) / edgra_elementthin;
            }

            if (clump->element_count > 0) {
                vector_cursor += static_cast<usize>(clump->element_count) * sizeof(NUVEC);
                ++edgra_clumps_used;
                edgra_last_clump_in_buffer = slot;
                edgra_free_vecbuffer = reinterpret_cast<void *>(vector_cursor);
            }
            ++slot;
        }

        EdFileSetReadWrongEndianess(0);
        EdFileClose();
        edgra_nearest = -1;
        edgraDetermineNearestClump(1.0f);
        edgra_page_used[page] = 1;
        edgra_page_scene[page] = static_cast<NUGSCN *>(gscn);
        edgra_page_terrain[page] = reinterpret_cast<void *>(static_cast<usize>(static_cast<u32>(terrain)));
        edgra_page_calculate_done[page] = 0;
        if (edbits_editmode != 1 || edgra_vecbuffer == NULL) {
            buffer->addr = vector_cursor;
        }
        (void)buf_end;
        return page;
    }
    void edgraClearPage(i8 page) {
        edgraStopPage(page);
        for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i) {
            edgra_clump_s *clump = &GrassClumps[i];
            if (clump->element_count && clump->page == static_cast<u8>(page)) {
                if (clump->kind == 3) {
                    IndGrassClumpsUsed[clump->individual_index] = 0;
                    --edgra_ind_clumps_used;
                }
                clump->element_count = 0;
                --edgra_clumps_used;
            }
        }
        edgra_page_used[page] = 0;
        edgra_page_scene[page] = NULL;
        edgra_page_matrix_stack[page] = NULL;
        edgra_page_vectors_valid[page] = 0;
        edgra_page_calculate_done[page] = 0;
    }
    void edgraClumpsReset(void) {
        for (i32 i = 0; i < EDGRA_MAX_CLUMPS; ++i)
            GrassClumps[i].element_count = 0;
        memset(edgra_page_used, 0, sizeof(edgra_page_used));
        memset(edgra_page_scene, 0, sizeof(edgra_page_scene));
        memset(edgra_page_on, 0, sizeof(edgra_page_on));
        edgra_clumps_used = 0;
        for (i32 i = 0; i < EDGRA_MAX_INDIVIDUAL_CLUMPS; ++i)
            IndGrassClumpsUsed[i] = 0;
        edgra_ind_clumps_used = 0;
    }
    void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end) {
        gra_ptr = start;
        gra_end = end;
    }
    void edgraSetThinning(i32 clumps, i32 elements) {
        edgra_clumpthin = clumps;
        edgra_elementthin = elements;
        if (edgra_clumpthin <= 0)
            edgra_clumpthin = 1;
        if (edgra_elementthin <= 0)
            edgra_elementthin = 1;
    }
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
    void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump) {
        EDGRA_MAX_CLUMPS = clumps;
        EDGRA_MAX_INDIVIDUAL_CLUMPS = individual_clumps;
        EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP = units_per_clump;
        if (units_per_clump == 0)
            EDGRA_MAX_INDIVIDUAL_CLUMPS = 0;
        buffer->addr = (buffer->addr + 15) & ~static_cast<usize>(15);
        GrassClumps = static_cast<edgra_clump_s *>(buffer->void_ptr);
        buffer->char_ptr += EDGRA_MAX_CLUMPS * sizeof(edgra_clump_s);
        IndGrassClumps = static_cast<edgra_individual_s *>(buffer->void_ptr);
        buffer->char_ptr +=
            EDGRA_MAX_INDIVIDUAL_CLUMPS * EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP * sizeof(edgra_individual_s);
        IndGrassClumpsUsed = static_cast<i32 *>(buffer->void_ptr);
        buffer->char_ptr += EDGRA_MAX_INDIVIDUAL_CLUMPS * sizeof(i32);
        NuWindSetup(buffer, end, 0x3000, clumps);
        NuFadeObjSetup(buffer, end, 0x3000, clumps);
    }
    i32 edmainActivate(ed_module_s *module, i32 notify) {
        if (module) {
            for (ed_module_s *entry = ed_list; entry; entry = entry->next) {
                if (entry == module) {
                    ed_curr = module;
                    ed_module_active = 1;
                    if (module->activate && notify)
                        module->activate();
                    return 1;
                }
            }
        } else if (ed_curr) {
            if (ed_curr->deactivate && notify)
                ed_curr->deactivate();
            ed_curr = NULL;
            ed_module_active = 0;
        }
        return 0;
    }
    void edmainClose(void) {
        if (ed_init) {
            NuCameraDestroy(edinternalcam);
            while (ed_list) {
                if (ed_list->close)
                    ed_list->close();
                ed_list = ed_list->next;
            }
            if (ed_main_menu) {
                eduiMenuDestroy(ed_main_menu);
                ed_main_menu = NULL;
            }
            if (ed_cfg_menu) {
                eduiMenuDestroy(ed_cfg_menu);
                ed_cfg_menu = NULL;
            }
            ed_list = NULL;
            ed_curr = NULL;
            ed_init = 0;
        }
    }
    ed_module_s *edmainCurrent(void) {
        return ed_module_active ? ed_curr : NULL;
    }
    void edmainExtCamera(NUCAMERA *camera) {
        edmaincam = camera != NULL ? camera : edinternalcam;
    }
    NUCAMERA *edmainGetCamera(void) {
        return edmaincam;
    }
    i32 edmainGetCursorEnabled(void) {
        return edmain_cursor_enabled;
    }
    // Shared editor font; edmainInit receives and stores the font handle.
    void *ed_fnt;

    void edmainInit(void *font, char *configuration_file) {
        if (ed_init)
            edmainClose();

        ed_module_active = 0;
        ed_fnt = font;
        for (ed_module_s *module = ed_list; module; module = module->next) {
            if (module->init)
                module->init();
        }

        edinternalcam = NuCameraCreate();
        edmainExtCamera(edinternalcam);
        eduiInit();

        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSetScale(system_qfont, edmain_menu_scale, edmain_menu_scale);

        ed_cfg_menu = eduiMenuCreate(220, 50, 180, 320, ed_fnt, cbmcfgCancel, const_cast<char *>("Editor Config"));
        if (ed_cfg_menu) {
            eduiMenuAddItem(ed_cfg_menu,
                            eduiItemSliderCreate(0, &ed_attr, 0, cbmcfgCameraSpeed, 1.0f, 20.0f,
                                                 edmain_default_camera_speed, const_cast<char *>("Camera Speed")));
            eduiMenuAddItem(ed_cfg_menu, eduiItemToggleCreate(0, &ed_attr, edmain_auto_speed, 1, cbSetAutoSpeed,
                                                              const_cast<char *>("Proportional Cursor Speed")));
            eduiMenuFitWidth(ed_cfg_menu, 8);
        }

        ed_main_menu = eduiMenuCreate(ed_main_menu_x, ed_main_menu_y, ed_main_menu_w, ed_main_menu_h, ed_fnt,
                                      cbMMCancel, const_cast<char *>("Editor Options"));
        if (ed_main_menu) {
            for (ed_module_s *module = ed_list; module; module = module->next) {
                eduiitem_s *item =
                    eduiMenuAddItem(ed_main_menu, eduiItemSelCreate(reinterpret_cast<usize>(module), &ed_attr, 0, 1,
                                                                    cbMMRegSel, const_cast<char *>(module->name)));
                if (module == ed_curr)
                    eduiMenuHighlight(ed_main_menu, item);
            }
            eduiMenuAddItem(ed_main_menu, eduiItemSelCreate(0, &ed_attr, 0, 0, cbMMEditorConfig,
                                                            const_cast<char *>("Editor Config")));
            eduiMenuAddItem(ed_main_menu,
                            eduiItemSelCreate(0, &ed_attr, 0, 0, cbMMReturnToApp, const_cast<char *>("Return To App")));
            eduiMenuFitWidth(ed_main_menu, 8);
        }

        NuQFntPopCoordinateSystem();

        if (!configuration_file) {
            ed_levelfile[0] = '\0';
        } else {
            NuStrCpy(ed_levelfile, configuration_file);
            if (ed_levelfile[0]) {
                i32 file = NuFileOpen(ed_levelfile, NUFILE_READ);
                if (file) {
                    NuFileBeginBlkRead(file, 0x3032554e);
                    i32 block;
                    while ((block = NuFileBeginBlkRead(file, 0)) != 0) {
                        for (ed_module_s *module = ed_list; module; module = module->next) {
                            if (module->block_id == static_cast<u32>(block)) {
                                if (module->read)
                                    module->read(file);
                                break;
                            }
                        }
                        NuFileEndBlkRead(file);
                    }
                    NuFileEndBlkRead(file);
                    NuFileClose(file);
                }
            }
        }
        ed_init = 1;
    }
    void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height) {
        ed_attr = *colours;
        ed_main_menu_x = x;
        ed_main_menu_y = y;
        ed_main_menu_w = width;
        ed_main_menu_h = height;
        edmainInit(font, configuration_file);
    }
    i32 edmainProcess(f32 delta_time, nupad_s *pad) {
        editor_return = 0;
        if (edmain_cursor_enabled)
            eduiProcessCursor(0.0f, NULL);

        if (ed_module_active && ed_curr) {
            if (!ed_curr->process) {
                ed_module_active = 0;
                return editor_return;
            }
            i32 result = ed_curr->process(delta_time, pad);
            if (!result)
                return editor_return;
            if (ed_curr->deactivate)
                ed_curr->deactivate();
            if (result == 2)
                editor_return = 1;
            ed_module_active = 0;
            return editor_return;
        }

        if (!ed_curr) {
            ed_curr = ed_list;
            eduiSetActiveMenu(ed_main_menu);
            eduiSetDefaultActiveMenu(ed_main_menu);
        }

        if (ed_main_menu) {
            for (eduiitem_s *item = ed_main_menu->first; item; item = item->next) {
                if (item->data_ptr) {
                    ed_module_s *module = static_cast<ed_module_s *>(item->data_ptr);
                    item->disabled = (reinterpret_cast<usize>(module->reserved) & 1) != 0;
                }
            }
            eduiMenuProcess(ed_main_menu, delta_time, pad);
        }
        return editor_return;
    }
    NUVEC *edmainQueryLocVec(void) {
        return ed_loc;
    }
    i32 edmainRegister(ed_module_s *module) {
        if (ed_init)
            edmainClose();
        if (ed_list)
            ed_list->previous = module;
        module->next = ed_list;
        ed_list = module;
        module->previous = NULL;
        return 1;
    }
    void edmainRegisterLocVec(NUVEC *position) {
        ed_loc = position;
    }
    void edmainRender(void) {
        if (edmain_cursor_enabled)
            eduiFlushInteracts();

        if (ed_module_active && ed_curr) {
            if (ed_curr->render)
                ed_curr->render();
        } else if (ed_main_menu) {
            NuQFntPushPrintMode(2);
            NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
            NuQFntSet(system_qfont);
            NuQFntSetScale(system_qfont, edmain_menu_scale, edmain_menu_scale);
            eduiMenuRender(ed_main_menu);
            NuQFntPopCoordinateSystem();
            NuQFntPopPrintMode();
        }

        if (edmain_cursor_enabled) {
            eduiRenderCursor();
            if (edmain_render_interacts)
                eduiRenderInteracts();
        }
    }
    void edmainSetCamera(NUMTX *matrix) {
        edmaincam->mtx = *matrix;
        NuCameraSet(edmaincam);
    }
    void edmainSetCursorEnabled(i32 enabled) {
        edmain_cursor_enabled = enabled;
    }
    f32 edmainSetMainMenuScale(f32 scale) {
        f32 previous = edmain_menu_scale;
        edmain_menu_scale = scale;
        if (ed_main_menu)
            eduiMenuFitWidth(ed_main_menu, 8);
        return previous;
    }
    void edmainSetReturn(i32 result) {
        editor_return = result;
    }
    void edpartClearPage(i8 page) {
        NuThreadDisableThreadSwap();
        CheckPartCount();
        if (part_page_on[page] != 0)
            edpartStopPage(page);
        for (i32 index = 0; index < 128; ++index) {
            if (part_types[index].page == page && part_types[index].name[0] != 0) {
                part_types[index].name[0] = 0;
                part_types[index].effect_ids[0] = -1;
                --part_types_used;
            }
        }
        for (i32 index = 0; index < 40; ++index) {
            if (part_emits[index].page == page && part_emits[index].effect_id != -1) {
                part_emits[index].effect_id = -1;
                --part_emits_used;
            }
        }
        CheckPartCount();
        for (i32 index = 0; index < 32; ++index) {
            if (part_scene_pageid[index] == page) {
                KillPartsByScene(part_scene[index]);
                part_scene_pageid[index] = -1;
                part_scene[index] = NULL;
            }
        }
        NuThreadEnableThreadSwap();
        part_page_used[page] = 0;
    }
    void edpartDestroyAllParticles(void) {
        for (i32 index = 0; index < 512; ++index) {
            edpartDestroy(index);
        }
    }
    void edpartParticleReset(void) {
        part_emit_s *emit = part_emits;
        part_emit_s *const emit_end = part_emits + 512;
        do {
            emit->instance_id = -1;
        } while (++emit != emit_end);
        memset(part_page_used, 0, sizeof(part_page_used));
        memset(part_page_on, 0, sizeof(part_page_on));
        edpart_instances_used = 0;
    }
    void edpartRegisterPointerToGameCharLocation(NUVEC *position) {
        edmainRegisterLocVec(position);
    }
    void edppClearPage(i8 page) {
        edpp_page_on[page] = 0;
        edpp_page_used[page] = 0;
        for (i32 index = 0; index < 512; ++index) {
            if (edpp_ptls[index].page == page)
                edppPtlDestroy(index);
        }
        for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
            if (debtab[index] == NULL || debtab[index]->page != static_cast<u8>(page))
                continue;
            debtab[index]->disabled = 1;
            for (i32 key = 0; key < maxdebkeys; ++key) {
                if (debkeydata[key].effect_index == index) {
                    i32 handle = key;
                    DebFreeInstantly(&handle);
                }
            }
            DebFreeOrphansInstantly(debtab[index]);
            if (debtab[index]->native_data != NULL) {
                DmaDebTypes[--freeDmaDebType] = debtab[index]->native_data;
                debtab[index]->native_data = NULL;
            }
            debtab[index] = NULL;
            --edpp_types_used;
        }
    }
    void edppDeleteEffect(i32 index) {
        if (edpp_ptls[edpp_nearest].effect_index == index)
            edpp_nearest = -1;
        DebFreeOrphansInstantly(debtab[index]);
        i32 replacement = LookupDebrisEffectPageIgnore(debtab[index]->name, 1, index);
        if (replacement != -1) {
            for (i32 i = 0; i < 512; ++i) {
                if (edpp_ptls[i].effect_index == index) {
                    i32 handle = edpp_ptls[i].instance_id;
                    if (handle != 99999 && handle != -1)
                        debkeydata[handle].effect_index = replacement;
                    edpp_ptls[i].effect_index = replacement;
                }
            }
            for (i32 i = 0; i < maxdebkeys; ++i)
                if (debkeydata[i].effect_index == index)
                    debkeydata[i].effect_index = replacement;
        } else {
            for (i32 i = 0; i < 512; ++i)
                if (edpp_ptls[i].effect_index == index)
                    edppPtlDestroy(i);
            for (i32 i = 0; i < maxdebkeys; ++i) {
                if (debkeydata[i].effect_index == index) {
                    i32 handle = i;
                    DebFreeInstantly(&handle);
                }
            }
        }
        debtab[index] = NULL;
        --edpp_types_used;
        edppDetermineNearest(1.0f);
    }
    void edppDestroyAllEffects(void) {
        for (i32 index = 1; index < EDPP_MAX_TYPES; ++index)
            debtab[index] = NULL;
        edpp_types_used = 1;
    }
    void edppDestroyAllParticles(void) {
        for (i32 index = 0; index < 512; ++index)
            edppPtlDestroy(index);
    }
    extern f32 globaltime;

    void edppDrawSpheres(debinftype *effect, i32 key_index) {
        if (effect->process_spheres == 0)
            return;
        for (i32 sphere_index = 0; sphere_index < static_cast<i8>(effect->process_spheres); ++sphere_index) {
            debris_process_sphere_s *sphere = &debkeydata[key_index].process_spheres[sphere_index];
            f32 time = globaltime - sphere->time;
            if (time >= 0.0f && time <= effect->particle_lifetime) {
                NUVEC position;
                position.x = sphere->position.x + sphere->momentum.x * time;
                position.y = sphere->position.y + (sphere->momentum.y * time + time * time * effect->field_0a0);
                position.z = sphere->position.z + sphere->momentum.z * time;
                time /= effect->particle_lifetime;
                i32 first = edppCurveSegment(effect->collision_keys, time);
                f32 radius = ((time - effect->collision_keys[first].time) /
                              (effect->collision_keys[first + 1].time - effect->collision_keys[first].time)) *
                                 (effect->collision_keys[first + 1].value - effect->collision_keys[first].value) +
                             effect->collision_keys[first].value;
                edbitsDrawSphere(&position, radius, 0xffff0000, edpp_mtl);
            }
        }
    }
    void edppDrawTorus(debinftype *effect, i32 key_index) {
        if (effect->torus_lifetime == 0.0f)
            return;
        debkeydatatype_s *key = &debkeydata[key_index];
        f32 time;
        if (globaltime > key->emission_time)
            time = globaltime - key->emission_time;
        else
            time = globaltime - key->previous_emission_time;
        if (time > 0.0f && time < effect->torus_lifetime) {
            time /= effect->torus_lifetime;
            f32 radius = edppInterpolateTorusCurve(effect->torus_keys1, time);
            f32 radial_extent = edppInterpolateTorusCurve(effect->torus_keys2, time);
            f32 vertical_extent = edppInterpolateTorusCurve(effect->torus_keys3, time);
            NUVEC position = key->position;
            edbitsDrawTorus(&position, radius * effect->torus_radius1, radial_extent * effect->torus_radius2,
                            vertical_extent * effect->torus_radius2, 0xffff0000, edpp_mtl);
        }
    }
    i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip) {
        i32 count = 0;
        for (i32 index = 0; index < 512; ++index) {
            edpp_particle_s *particle = &edpp_ptls[index];
            if (page != -1 && particle->page != page)
                continue;
            if (particle->instance_id == 99999 || particle->instance_id == -1)
                continue;
            debkeydatatype_s *key = &debkeydata[particle->instance_id];
            if (key->process_collision_sound == 0)
                continue;
            debinftype *effect = debtab[key->effect_index];
            i32 output_index = count - skip;
            if (output_index >= 0 && output_index < capacity) {
                if (positions != NULL) {
                    positions[output_index].x = key->position.x;
                    positions[output_index].y = key->position.y;
                    positions[output_index].z = key->position.z;
                }
                if (sounds != NULL) {
                    sounds[output_index][0] = effect->sound_data[0];
                    sounds[output_index][1] = effect->sound_data[3];
                    sounds[output_index][2] = effect->sound_data[6];
                    sounds[output_index][3] = effect->sound_data[9];
                }
            }
            ++count;
        }
        return count - skip;
    }
    // Parts-page loader (edppLoadPage @0x36c630).
    i32 edppLoadPage(char *path, i32 flag, usize scene) {
        i8 category = static_cast<i8>(flag);
        i32 page_index = -1;
        if (flag == 0) {
            category = 0;
            page_index = 0;
        } else if (flag == 5) {
            category = 5;
            page_index = 1;
        } else {
            if (flag == 6)
                category = 1;
            for (i32 page = 2; page < 8; ++page) {
                if (edpp_page_used[page] == 0) {
                    page_index = page;
                    break;
                }
            }
            if (page_index == -1)
                return -1;
        }

        EdFileSetMedia(edpp_usememcard == 0 ? 1 : 2);
        if (EdFileOpen(path, NUFILE_READ) == 0) {
            return -1;
        }
        EdFileSetReadWrongEndianess(1);

        i32 version = EdFileReadInt();
        if (version < 5 || version > 41) {
            EdFileSetReadWrongEndianess(0);
            EdFileClose();
            return -1;
        }

        edpp_page_used[page_index] = 1;
        edpp_page_scene[page_index] = scene;

        i32 requested = EdFileReadInt();
        i32 skipped = 0;
        if (requested + edpp_types_used > EDPP_MAX_TYPES) {
            skipped = requested + edpp_types_used - EDPP_MAX_TYPES;
            requested = EDPP_MAX_TYPES - edpp_types_used;
            edbits_types_error = 300;
        }

        for (i32 n = 0; n < requested; n++) {
            i32 index = 1;
            while (index < EDPP_MAX_TYPES && debtab[index] != NULL) {
                index++;
            }
            if (index >= EDPP_MAX_TYPES) {
                break;
            }

            debinftype *effect = &effecttypes[index];
            FileLoadSingleEffectType(effect, version, static_cast<char>(category));
            effect->native_data = NULL;
            effect->last_render_time = 0.0f;
            effect->page = static_cast<u8>(page_index);
            debtab[index] = effect;
            f32 elapsed_time = 0.0f;
            f32 active_time = 0.0f;
            while (effect->particle_lifetime > elapsed_time) {
                f32 remaining_time = effect->particle_lifetime - elapsed_time;
                f32 emission_time = effect->emission_period_random + effect->emission_pause;
                f32 emitted = remaining_time < emission_time ? remaining_time : emission_time;
                active_time += emitted;
                elapsed_time += emitted;
                remaining_time = effect->particle_lifetime - elapsed_time;
                elapsed_time +=
                    remaining_time < effect->emission_pause_random ? remaining_time : effect->emission_pause_random;
            }
            i16 particle_count = static_cast<i16>(static_cast<i32>(
                static_cast<f32>(effect->frequency) * (active_time / elapsed_time) * effect->particle_lifetime));
            if (particle_count < 1)
                particle_count = 1;
            effect->max_particles = static_cast<i16>(particle_count * (effect->trail_count + 1));
            for (i32 particle_index = 0; particle_index < 512; ++particle_index) {
                i32 instance_id = edpp_ptls[particle_index].instance_id;
                if (instance_id == -1 || instance_id == 99999)
                    continue;
                debkeydatatype_s *key = &debkeydata[instance_id];
                if (debtab[key->effect_index] == effect)
                    DebReAlloc(key, effect->max_particles);
            }
            edpp_types_used++;
        }

        debinftype discarded;
        for (i32 n = 0; n < skipped; ++n)
            FileLoadSingleEffectType(&discarded, version, category);

        if (flag == 1 || flag == 2) {
            i32 instances = EdFileReadInt();
            if (instances + edpp_instances_used > 512)
                instances = 512 - edpp_instances_used;
            edpp_instances_used = 0;
            for (i32 n = 0; n < instances; ++n) {
                i32 index = 0;
                while (index < 512 && edpp_ptls[index].instance_id != -1)
                    ++index;
                if (index == 512)
                    break;
                edpp_particle_s *particle = &edpp_ptls[index];
                particle->page = static_cast<i8>(page_index);
                particle->position.x = EdFileReadFloat();
                particle->position.y = EdFileReadFloat();
                particle->position.z = EdFileReadFloat();
                if (version <= 6) {
                    particle->rotation_z = 0;
                    particle->rotation_y = 0;
                    particle->emitter_rotation_z = static_cast<i16>(EdFileReadInt());
                    particle->emitter_rotation_y = static_cast<i16>(EdFileReadInt());
                } else {
                    particle->rotation_z = EdFileReadShort();
                    particle->rotation_y = EdFileReadShort();
                    particle->emitter_rotation_z = EdFileReadShort();
                    particle->emitter_rotation_y = EdFileReadShort();
                }
                particle->emitter_rotation_x = version > 21 ? EdFileReadShort() : 0;
                if (version > 33)
                    particle->start_offset = EdFileReadFloat();
                else if (version > 7)
                    particle->start_offset = static_cast<f32>(EdFileReadInt()) / 60.0f;
                else
                    particle->start_offset = 0.0f;
                EdFileRead(particle->name, 16);
                particle->instance_id = 99999;
                particle->effect_index = LookupDebrisEffectPageOnly(particle->name, static_cast<char>(page_index));
                if (particle->effect_index == -1)
                    ++edpp_instances_used;

                if (version > 8) {
                    particle->switch_type = EdFileReadInt();
                    particle->switch_id = EdFileReadInt();
                    particle->switch_variable = EdFileReadFloat();
                } else {
                    particle->switch_type = 0;
                    particle->switch_id = -1;
                    particle->switch_variable = 0.0f;
                }
                if (version > 11) {
                    particle->reflection_rotation_z = EdFileReadShort();
                    particle->reflection_rotation_y = EdFileReadShort();
                    particle->reflection_offset = EdFileReadFloat();
                    particle->reflection_bounce = version == 12 ? 0.9f : EdFileReadFloat();
                } else {
                    particle->reflection_rotation_z = 0;
                    particle->reflection_rotation_y = 0;
                    particle->reflection_offset = 0.0f;
                    particle->reflection_bounce = 0.9f;
                }
                particle->render_group = version > 14 ? EdFileReadShort() : 0;
                if (version > 26) {
                    particle->render_priority = EdFileReadUnsignedShort();
                } else {
                    particle->render_priority = 20000;
                    if (particle->effect_index != -1) {
                        u8 particle_type = debtab[particle->effect_index]->particle_type;
                        if (particle_type == 2)
                            particle->render_priority = 40000;
                        else if (particle_type == 3)
                            particle->render_priority = 30000;
                        else if (particle_type == 7)
                            particle->render_priority = 10000;
                    }
                }
                particle->dynamic_priority = version > 26 ? EdFileReadChar() : 0;
                particle->detail_levels = version > 35 ? EdFileReadChar() : 7;
                particle->facing_mode = version > 37 ? EdFileReadChar() : 0;
                particle->facing_rotation_x = version > 37 ? EdFileReadShort() : 0;
                particle->facing_rotation_y = version > 37 ? EdFileReadShort() : 0;
                ++edpp_instances_used;
            }
        }

        EdFileSetReadWrongEndianess(0);
        EdFileClose();
        edppDetermineNearest(1.0f);

        if (flag == 0) {
            DEBPAGE_GENERAL = page_index;
        } else if (flag == 5) {
            DEBPAGE_CHARACTER = page_index;
        } else if (flag == 1) {
            DEBPAGE_AREA = page_index;
        }
        return page_index;
    }
    void edppRegisterPointerToGameCharLocation(NUVEC *position) {
        edmainRegisterLocVec(position);
    }
    void edppRestartAllEffectsInLevel(void) {
        for (i32 index = 0; index < 512; ++index) {
            edpp_particle_s *particle = &edpp_ptls[index];
            if (particle->instance_id == -1)
                continue;
            particle->instance_id = -1;
            particle->effect_index = LookupDebrisEffect(particle->name);
            AddDebrisEffect(&particle->instance_id, particle->effect_index, particle->position.x, particle->position.y,
                            particle->position.z);
            if (particle->instance_id != -1) {
                debkeydata[particle->instance_id].field_2f9 = 0;
                DebrisOrientation(particle->instance_id, particle->rotation_z, particle->rotation_y);
                DebrisEmitterOrientation(particle->instance_id, particle->emitter_rotation_z,
                                         particle->emitter_rotation_y, particle->emitter_rotation_x);
                DebrisStartOffset(particle->instance_id, particle->start_offset);
                DebrisSetTrigger(particle->instance_id, particle->switch_type, particle->switch_id,
                                 particle->switch_variable);
                DebrisReflectionOrientation(particle->instance_id, particle->reflection_rotation_z,
                                            particle->reflection_rotation_y, particle->reflection_offset,
                                            particle->reflection_bounce);
                DebrisSetFacing(particle->instance_id, particle->facing_mode, particle->facing_rotation_x,
                                particle->facing_rotation_y);
                DebrisSetGroupID(particle->instance_id, particle->render_group);
                DebrisSetRoomID(particle->instance_id, reinterpret_cast<nugscn_s *>(edpp_page_scene[particle->page]));
            } else {
                particle->instance_id = 99999;
            }
            edpp_page_on[particle->page] = 1;
        }
        edpp_create_type = -1;
        edppDetermineNearest(1.0f);
    }
    void edppSetSaveName(u32 page, char *name) {
        if (page < 6)
            edpp_save_names[page] = name;
    }
    void edppStopPage(i32 page_id) {
        i8 page = page_id;
        edpp_page_on[page] = 0;
        for (i32 index = 0; index < 512; ++index) {
            edpp_particle_s *particle = &edpp_ptls[index];
            if (particle->instance_id == 99999 || particle->instance_id == -1)
                continue;
            if (particle->page == page) {
                edppPtlShelve(index);
            } else if (particle->effect_index > 0 && debtab[particle->effect_index] != NULL &&
                       debtab[particle->effect_index]->page == static_cast<u8>(page)) {
                edppPtlDestroy(index);
            }
        }
        for (i32 index = 1; index < EDPP_MAX_TYPES; ++index) {
            if (debtab[index] == NULL || debtab[index]->page != static_cast<u8>(page))
                continue;
            for (i32 key = 0; key < maxdebkeys; ++key) {
                if (debkeydata[key].effect_index == index) {
                    i32 handle = key;
                    DebFreeInstantly(&handle);
                }
            }
            DebFreeOrphansInstantly(debtab[index]);
        }
    }
    i32 edqseed = 12345;
    void edqrand(void) {
        edqseed = (edqseed * 0x24cd + 1) & 0xffff;
    }
    edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback) {
        static eduimenu_s *tpent;
        tpent =
            eduiMenuCreate(source->x + 10, source->y + 10, 180, 50, menu->font, NULL, const_cast<char *>("Enter Text"));
        tpent->flags |= 2;
        auto *picker = static_cast<edui_textpicker_s *>(
            eduiItemTextPickCreate(reinterpret_cast<usize>(source), &ed_attr, callback, const_cast<char *>("")));
        eduiMenuAddItem(tpent, picker);
        eduiMenuAttach(menu, tpent);
        picker->editing = 1;
        eduiMenuFitWidth(tpent, 8);
        picker->cursor = NuStrLen(picker->value);
        return picker;
    }
    void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source) {
        auto *picker = eduiAddTextPickEntEx(menu, source, tpentPropOnEnter);
        if (!picker)
            return;
        NuStrCpy(picker->value, static_cast<edui_prop_s *>(source)->property_text);
        picker->cursor = NuStrLen(picker->value);
        memcpy(picker->colours, source->colours, sizeof(picker->colours));
    }
    void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source) {
        auto *picker = eduiAddTextPickEntEx(menu, source, tpentOnEnter);
        if (!picker)
            return;
        NuStrCpy(picker->value, static_cast<edui_textpicker_s *>(source)->value);
        picker->cursor = NuStrLen(picker->value);
        memcpy(picker->colours, source->colours, sizeof(picker->colours));
        picker->max_length = static_cast<edui_textpicker_s *>(source)->max_length;
        picker->keyboard_flags =
            (picker->keyboard_flags & ~2u) | (static_cast<edui_textpicker_s *>(source)->keyboard_flags & 2u);
    }
    i32 eduiClearActiveMenu(void) {
        eduiSetActiveMenu(NULL);
        return 0;
    }
    i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad) {
        i32 result = 0;
        if (pad && (pad->digital_buttons_pressed & 0x10)) {
            eduimenu_s *parent = menu->parent;
            if (!eduiGetUsingMenuFocus() && menu->parent)
                eduiMenuDetach(menu);
            if (menu->callback)
                menu->callback(menu, parent);
            result = 1;
        }
        return result;
    }
    void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight) {
        eduiiattr_s normal{0x800000c0, 0x80ff0000, 0x80808080, 0x80404040};
        eduiiattr_s highlighted{0x8000c000, 0x80ff0000, 0x80808080, 0x80404040};
        edui_messagemenu =
            eduiMenuCreate(70, 70, 300, 250, parent->font, eduicbCancelMessageMenu, const_cast<char *>("Message"));
        if (!edui_messagemenu)
            return;
        if (first)
            eduiMenuAddItem(edui_messagemenu,
                            eduiItemSelCreate(1, first_highlight ? &highlighted : &normal, 0, 0, NULL, first));
        if (second)
            eduiMenuAddItem(edui_messagemenu,
                            eduiItemSelCreate(1, second_highlight ? &highlighted : &normal, 0, 0, NULL, second));
        if (third)
            eduiMenuAddItem(edui_messagemenu,
                            eduiItemSelCreate(1, third_highlight ? &highlighted : &normal, 0, 0, NULL, third));
        eduiMenuAttach(parent, edui_messagemenu);
        edui_messagemenu->x = parent->x + 10;
        edui_messagemenu->y = parent->y + 40;
    }
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted) {
        eduiiattr_s colours{highlighted == 1 ? 0x8000c000u : 0x800000c0u, 0x80ff0000, 0x80808080, 0x80404040};
        edui_messagemenu =
            eduiMenuCreate(70, 70, 180, 250, parent->font, eduicbCancelMessageMenu, const_cast<char *>("Message"));
        if (!edui_messagemenu)
            return;
        eduiMenuAddItem(edui_messagemenu, eduiItemSelCreate(1, &colours, 0, 0, NULL, message));
        eduiMenuAttach(parent, edui_messagemenu);
        edui_messagemenu->x = parent->x + 10;
        edui_messagemenu->y = parent->y + 40;
    }
    i32 eduiCursorOverMenu(eduimenu_s *menu) {
        return edui_cursor_x >= menu->x && edui_cursor_y >= menu->y && edui_cursor_x < menu->x + menu->width &&
               edui_cursor_y < menu->y + menu->height;
    }
    i32 cbInteractMenuTitle(edui_interact_s *interact) {
        static f32 offsetx;
        static f32 offsety;
        i32 result = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        eduimenu_s *menu = interact->menu;
        if (!(menu->flags & 1)) {
            if (!eduiInteractLocked) {
                offsetx = 0.0f - menu->x;
                offsety = 0.0f - menu->y;
            }
            menu->x = static_cast<i32>(0.0f - offsetx);
            menu->y = static_cast<i32>(0.0f - offsety);
        }
        return result;
    }
    i32 cbInteractMenuScrollUp(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu->field_0c) {
            if (menu->field_0c->previous)
                menu->field_0c = menu->field_0c->previous;
            menu->selected = menu->field_0c;
        }
        return 0;
    }
    i32 cbInteractMenuScrollDown(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu->field_10) {
            if (menu->field_10->next)
                menu->field_10 = menu->field_10->next;
            menu->selected = menu->field_10;
        }
        return 0;
    }
    void cbInteractMenuScrollTo(eduimenu_s *menu, char *text) {
        if (!text || !menu || !text[0])
            return;
        i32 comparison = menu->first && menu->first->text ? NuStrICmp(menu->first->text, text) : -1;
        menu->field_0c = menu->first;
        menu->selected = menu->first;
        while (menu->selected->next && comparison < 0) {
            menu->selected = menu->selected->next;
            comparison = menu->selected->text ? NuStrICmp(menu->selected->text, text) : -1;
        }
        menu->field_10 = menu->selected;
    }
    void cbInteractMenuKeySelect(eduimenu_s *menu) {
        char text[16];
        u32 modifiers;
        i32 key = NuKeyGet(&modifiers);
        if (key >= 0) {
            text[0] = NuKeyToAscii(key, modifiers & 1);
            text[1] = 0;
            cbInteractMenuScrollTo(menu, text);
        }
    }
    void eduiFlushInteracts(void) {
        numInteracts = 0;
    }
    eduimenu_s *eduiGetActiveMenu(void) {
        return active_menu;
    }
    eduimenu_s *eduiGetActiveMenuParent(void) {
        return eduiGetTopLevelParent(active_menu);
    }
    f32 eduiGetAnalougePadValue(nupad_s *pad) {
        f32 value = 0.0f;
        if (pad && (pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
            if (pad->analog_right_x > 192)
                value = (pad->analog_right_y - 128.0f) * 0.01f;
            else if (pad->analog_right_x < 64)
                value = (128.0f - pad->analog_right_y) * -0.01f;
            if (pad->analog_left_x > 192)
                value = (pad->analog_left_y - 128.0f) * 0.001f;
            else if (pad->analog_left_x < 64)
                value = (128.0f - pad->analog_left_y) * -0.001f;
        }
        return value;
    }
    i32 eduiGetCameraEnabled(void) {
        return bCameraEnabled;
    }
    void eduiGetCursorCoords(f32 *x, f32 *y) {
        *x = edui_cursor_x / 640.0f;
        *y = edui_cursor_y / 224.0f;
    }
    void eduiGetCursorDelta(f32 *x, f32 *y) {
        *x = edui_cursor_dx / 640.0f;
        *y = edui_cursor_dy / 224.0f;
    }
    eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu) {
        if (menu)
            while (menu->parent)
                menu = menu->parent;
        return menu;
    }
    i32 bUsingMenuFocus;
    i32 eduiGetUsingMenuFocus(void) {
        return bUsingMenuFocus;
    }
    i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity) {
        edui_gradient_node_s *node = static_cast<edui_gradient_pick_s *>(item)->first_stage;
        f32 red, green, blue;
        i32 count = 0;
        for (; node; node = node->next, ++count) {
            if (count < capacity) {
                edui_gradient_stage_s *stage = &stages[count];
                stage->time = node->time;
                stage->hue = node->hue;
                stage->saturation = node->saturation;
                stage->value = node->value;
                eduiHSVToRGB(node->hue, node->saturation, node->value, red, green, blue);
                stage->red = red;
                stage->green = green;
                stage->blue = blue;
                stage->colour = 0x80000000u + static_cast<i32>(red * 255.0f) + (static_cast<i32>(green * 255.0f) << 8) +
                                (static_cast<i32>(blue * 255.0f) << 16);
                stage->metadata = node->metadata;
            }
        }
        return count;
    }
    edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value) {
        edui_gradient_node_s *stage = static_cast<edui_gradient_node_s *>(NU_ALLOC(sizeof(*stage), 4, 1, "", 0));
        if (!stage)
            return NULL;
        memset(stage, 0, sizeof(*stage));
        edui_gradient_node_s *previous = NULL;
        edui_gradient_node_s *next = item->first_stage;
        while (next && !(next->time > time)) {
            previous = next;
            next = next->next;
        }
        if (previous)
            previous->next = stage;
        else
            item->first_stage = stage;
        if (next)
            next->previous = stage;
        stage->previous = previous;
        stage->time = time;
        stage->next = next;
        eduiGradStageSetHSV(stage, hue, saturation, value);
        item->selected_stage = stage;
        return stage;
    }
    edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue) {
        f32 hue = 0.0f, saturation = 0.0f, value;
        eduiRGBToHSV(red, green, blue, hue, saturation, value);
        return eduiGradStageAdd(item, time, hue, saturation, value);
    }
    void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage) {
        edui_gradient_node_s *previous = stage->previous;
        edui_gradient_node_s *next = stage->next;
        if (previous)
            previous->next = next;
        else
            item->first_stage = next;
        if (next)
            next->previous = previous;
        item->selected_stage = next ? next : previous;
        NU_FREE(stage);
    }
    void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value) {
        f32 red = 0.0f, green = 0.0f, blue = 0.0f;
        eduiHSVToRGB(hue, saturation, value, red, green, blue);
        stage->hue = hue;
        stage->saturation = saturation;
        stage->value = value;
        stage->colour = 0x80000000u | static_cast<u32>(static_cast<i32>(red * 255.0f)) |
                        (static_cast<u32>(static_cast<i32>(green * 255.0f)) << 8) |
                        (static_cast<u32>(static_cast<i32>(blue * 255.0f)) << 16);
    }
    void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue) {
        f32 hue = 0.0f, saturation = 0.0f, value;
        eduiRGBToHSV(red, green, blue, hue, saturation, value);
        stage->hue = hue;
        stage->saturation = saturation;
        stage->value = value;
        stage->colour = 0x80000000u | static_cast<u32>(static_cast<i32>(red)) |
                        (static_cast<u32>(static_cast<i32>(green)) << 8) |
                        (static_cast<u32>(static_cast<i32>(blue)) << 16);
    }
    void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth) {
        item->depth = depth;
        for (eduiitem_s *child = item->first_child; child; child = child->next) {
            if (child->type == 19)
                eduiIitemExpanderSetDepth(static_cast<edui_expander_s *>(child), depth + 1);
            if (child->type == 17)
                static_cast<edui_prop_s *>(child)->depth = depth + 1;
            if (child == item->last_child)
                break;
        }
    }
    void eduiInit(void) {
        eduiInitMaterials();
        if (edui_filepick_listsize && !dir_list)
            dir_list = NU_ALLOC(static_cast<usize>(edui_filepick_listsize) * 0x118, 4, 1, "", 0);
    }
    void eduiInitMaterials(void) {
        uimtls[0] = NuMtlCreate(1);
        uimtls[0]->opacity = 1.0f;
        uimtls[0]->attribs.alpha_mode = 0;
        uimtls[0]->attribs.cull_mode = 2;
        uimtls[0]->attribs.z_mode = 3;
        NuMtlUpdate(uimtls[0]);
        uimtls[1] = NuMtlCreate(1);
        uimtls[1]->opacity = 0.999f;
        uimtls[1]->diffuse_color.r = 0.2f;
        uimtls[1]->diffuse_color.g = 0.2f;
        uimtls[1]->diffuse_color.b = 0.2f;
        uimtls[1]->attribs.cull_mode = 2;
        uimtls[1]->attribs.z_mode = 3;
        uimtls[1]->attribs.alpha_mode = 3;
        NuMtlUpdate(uimtls[1]);
        uimtls[2] = NuMtlCreate(1);
        uimtls[2]->opacity = 0.999f;
        uimtls[2]->diffuse_color.r = 0.2f;
        uimtls[2]->diffuse_color.g = 0.2f;
        uimtls[2]->diffuse_color.b = 0.2f;
        uimtls[2]->attribs.cull_mode = 2;
        uimtls[2]->attribs.z_mode = 3;
        uimtls[2]->attribs.alpha_mode = 2;
        NuMtlUpdate(uimtls[2]);
        uimtls[3] = NuMtlCreate(1);
        uimtls[3]->opacity = 0.999f;
        uimtls[3]->diffuse_color.r = 0.2f;
        uimtls[3]->diffuse_color.g = 0.2f;
        uimtls[3]->diffuse_color.b = 0.2f;
        uimtls[3]->attribs.cull_mode = 2;
        uimtls[3]->attribs.z_mode = 3;
        uimtls[3]->attribs.alpha_mode = 1;
        NuMtlUpdate(uimtls[3]);
    }
    eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item) {
            item->type = 2;
            item->process = eduicbProcessSel;
            item->render = eduicbRenderCheck;
        }
        return item;
    }
    eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_colour_pick_s *item = static_cast<edui_colour_pick_s *>(NU_ALLOC(sizeof(edui_colour_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 10;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessColourPick;
        item->render = eduicbRenderColourPick;
        item->destroy = eduicbItemDestroy;
        item->interact = eduicbInteractColourPick;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->cursor_x = 0.5f;
        item->cursor_y = 0.5f;
        item->hue = 180.0f;
        item->value = 0.5f;
        item->saturation = 1.0f;
        item->changed = callback;
        return item;
    }
    void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value) {
        item->hue = hue;
        item->saturation = saturation;
        item->value = value;
        item->cursor_x = hue / 360.0f;
        item->cursor_y = value;
    }
    void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue) {
        eduiRGBToHSV(red, green, blue, item->hue, item->saturation, item->value);
        item->cursor_x = item->hue / 360.0f;
        item->cursor_y = item->value;
    }
    eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text) {
        edui_colour_slider_s *item =
            static_cast<edui_colour_slider_s *>(NU_ALLOC(sizeof(edui_colour_slider_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 13;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessColourSlider;
        item->render = eduicbRenderColourSlider;
        item->destroy = eduicbItemSliderDestroy;
        item->selection_group = group;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text);
        item->granularity = 1.0f;
        item->format = NULL;
        item->changed = callback;
        item->minimum = minimum;
        item->range = range;
        item->red = red;
        item->green = green;
        item->blue = blue;
        eduiItemSliderSetFmt(item, (char *)"(%d)");
        eduiItemSliderSetVal(item, value);
        return item;
    }
    eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text) {
        edui_gradient_pick_s *item =
            static_cast<edui_gradient_pick_s *>(eduiItemGradPickCreate(data, colours, callback, text));
        if (item) {
            item->type = 9;
            item->press = press;
            item->add = add;
            item->remove = remove;
            item->copy = copy;
            item->paste = paste;
        }
        return item;
    }
    void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child) {
        child->previous = item->last_child;
        if (item->last_child)
            item->last_child->next = child;
        item->last_child = child;
        if (!item->first_child)
            item->first_child = child;
        child->next = NULL;
        if (child->type == 19)
            eduiIitemExpanderSetDepth(static_cast<edui_expander_s *>(child), item->depth + 1);
        if (child->type == 17)
            static_cast<edui_prop_s *>(child)->depth = item->depth + 1;
    }
    eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_expander_s *item = static_cast<edui_expander_s *>(NU_ALLOC(sizeof(edui_expander_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 19;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessExpander;
        item->render = eduicbRenderExpander;
        item->destroy = eduicbItemDestroyExpander;
        item->interact = eduicbInteractExpander;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->changed = callback;
        return item;
    }
    eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        auto *item = static_cast<edui_file_pick_s *>(NU_ALLOC(sizeof(edui_file_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 1;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessFilePick;
        item->render = eduicbRenderFilePick;
        item->destroy = eduicbItemFilePickDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->changed = callback;
        item->format = NULL;
        item->name[0] = '\0';
        item->directory[0] = '\0';
        item->filename[0] = '\0';
        item->compare_entries = NULL;
        eduiItemFilePickSetFmt(item, const_cast<char *>("\"%s\""));
        return item;
    }
    void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format) {
        if (item->format) {
            i32 old_length = NuStrLen(item->format);
            i32 new_length = NuStrLen(format);
            if (old_length >= new_length)
                goto copy;
            NU_FREE(item->format);
        }
        item->format = static_cast<char *>(NU_ALLOC(NuStrLen(format) + 1, 4, 1, "", 0));
    copy:
        NuStrCpy(item->format, format);
    }
    void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child) {
        eduiitem_s *first = item->first_child;
        child->previous = NULL;
        child->next = first;
        if (first)
            first->previous = child;
        item->first_child = child;
    }
    eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value) {
        edui_filter_s *item = static_cast<edui_filter_s *>(NU_ALLOC(sizeof(edui_filter_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 20;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessFilter;
        item->render = eduicbRenderFilter;
        item->destroy = eduicbItemDestroyFilter;
        item->interact = eduicbInteractFilter;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        eduiItemPropSetText(item, value);
        item->label_width = 50.0f;
        return item;
    }
    void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child) {
        if (item->first_child == child)
            item->first_child = child->next;
        if (child->next)
            child->next->previous = child->previous;
        if (child->previous)
            child->previous->next = child->next;
        child->next = NULL;
        child->previous = NULL;
    }
    eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_gradient_pick_s *item =
            static_cast<edui_gradient_pick_s *>(NU_ALLOC(sizeof(edui_gradient_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 7;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessGradPick;
        item->render = eduicbRenderGradPick;
        item->destroy = eduicbItemGradPickDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->change_timer = 0;
        item->changed = callback;
        return item;
    }
    void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph) {
        if (item->type == 15) {
            for (i32 i = 0; i < 8; ++i) {
                if (!item->onion_skins[i]) {
                    item->onion_skins[i] = graph;
                    break;
                }
            }
        }
    }
    eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height) {
        edui_graph_s *item = static_cast<edui_graph_s *>(NU_ALLOC(sizeof(edui_graph_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 15;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessGraph;
        item->render = eduicbRenderGraph;
        item->destroy = eduicbItemDestroy;
        item->changed = callback;
        item->graph = graph;
        item->width = width;
        item->height = height;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        item->selected_point = -1;
        item->cursor_x = 0.5f;
        item->cursor_y = 0.5f;
        item->x_scale = 1.0f;
        item->y_scale = 1.0f;
        for (i32 i = 0; i != 8; ++i)
            item->onion_skins[i] = NULL;
        if (graph->point_count <= 1)
            nugraphInit(graph);
        nugraphCalcCurve(graph, 100);
        return item;
    }
    void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y) {
        if (item->type == 15) {
            item->cursor_x = x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);
            item->cursor_y = y < 0.0f ? 0.0f : (y > 1.0f ? 1.0f : y);
        }
    }
    void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title) {
        if (item->type == 15) {
            if (x)
                NuStrCpy(item->x_label, x);
            if (y)
                NuStrCpy(item->y_label, y);
            if (title)
                NuStrCpy(item->title, title);
        }
    }
    eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemGradPickCreate(data, colours, callback, text);
        if (item)
            item->type = 8;
        return item;
    }
    eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemColourPickCreate(data, colours, callback, text);
        if (item) {
            item->type = 11;
            item->process = eduicbProcessGreyPick;
            item->render = eduicbRenderGreyPick;
        }
        return item;
    }
    eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text) {
        eduiitem_s *item = eduiItemSliderCreate(data, colours, group, callback, minimum, range, value, text);
        if (item)
            item->render = eduicbRenderNumber;
        return item;
    }
    eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value) {
        return eduiItemPropCreateEx(data, colours, selected, changed, button, button_type, text, value, 0);
    }
    eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data) {
        edui_prop_s *item = static_cast<edui_prop_s *>(NU_ALLOC(sizeof(edui_prop_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 17;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessProp;
        item->render = eduicbRenderProp;
        item->destroy = eduicbItemDestroyProp;
        item->interact = eduicbInteractProp;
        item->highlighted = 0;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        eduiItemPropSetText(item, value);
        item->button_type = button_type;
        item->label_width = 50.0f;
        item->selected = selected;
        item->changed = changed;
        item->button = button;
        item->extra_data = extra_data;
        return item;
    }
    i32 eduiItemPropSetText(edui_prop_s *item, char *text) {
        if (item->property_text && NuStrLen(item->property_text) < NuStrLen(text)) {
            NU_FREE(item->property_text);
            item->property_text = NULL;
        }
        if (!item->property_text) {
            u32 size = NuStrLen(text) + 1;
            item->property_text = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        if (item->property_text) {
            NuStrCpy(item->property_text, text);
            return 1;
        }
        return 0;
    }
    i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected) {
        item_width = width;
        typedef i32 (*RenderCallback)(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
        i32 height = reinterpret_cast<RenderCallback>(item->render)(menu, item, x, y, width, selected);
        if (selected) {
            u32 colour = (item->flags & 4) ? eduiitem_context_outline_colour : eduiitem_selected_outline_colour;
            if (!edui_donotdraw)
                NuRndrLineRect2di(x << 4, (y << 3) + 8, item_width << 4, (height << 3) - 16, colour, uimtls[ui_outmtl]);
        }
        if (item->disabled) {
            i32 left = x << 4;
            i32 top = y << 3;
            i32 right = left + (width << 4);
            i32 bottom = top + (height << 3);
            if (!edui_donotdraw)
                NuRndrLine2di(left, top, left + (width << 2), bottom, item->colours[0], uimtls[ui_bgmtl]);
            if (!edui_donotdraw)
                NuRndrLine2di(left + (width << 2), top, left + (width << 3), bottom, item->colours[0],
                              uimtls[ui_bgmtl]);
            if (!edui_donotdraw)
                NuRndrLine2di(left + (width << 3), top, left + width * 12, bottom, item->colours[0], uimtls[ui_bgmtl]);
            if (!edui_donotdraw)
                NuRndrLine2di(left + width * 12, top, right, bottom, item->colours[0], uimtls[ui_bgmtl]);
        }
        return height;
    }
    eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text) {
        edui_sel_s *item = static_cast<edui_sel_s *>(NU_ALLOC(sizeof(edui_sel_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 0;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSel;
        item->render = eduicbRenderSel;
        item->destroy = eduicbItemDestroy;
        item->interact = eduicbInteractSel;
        item->selection_group = group;
        item->highlighted = selected;
        item->text_alignment = 0x40;
        item->text = NULL;
        eduiItemSetText(item, text ? text : (char *)"<no name>");
        item->selected = callback;
        item->held = NULL;
        return item;
    }
    eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item)
            item->render = eduicbRenderSelWithClipColour;
        return item;
    }
    eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours) {
        eduiitem_s *item = static_cast<eduiitem_s *>(NU_ALLOC(sizeof(eduiitem_s), 4, 1, "", 0));
        if (item) {
            memset(item, 0, sizeof(*item));
            item->type = 18;
            item->data = data;
            memcpy(item->colours, colours, sizeof(item->colours));
            item->process = eduicbProcessSeparator;
            item->render = eduicbRenderSeparator;
            item->destroy = eduicbItemDestroy;
            item->text_alignment = 0x40;
            item->selection_group = 0;
            item->highlighted = 0;
        }
        return item;
    }
    i32 eduiItemSetText(eduiitem_s *item, char *text) {
        if (item->text && NuStrLen(item->text) < NuStrLen(text)) {
            NU_FREE(item->text);
            item->text = NULL;
        }
        if (!item->text) {
            u32 size = NuStrLen(text) + 1;
            item->text = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        if (item->text) {
            NuStrCpy(item->text, text);
            return 1;
        }
        return 0;
    }
    eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text) {
        edui_slider_s *item = static_cast<edui_slider_s *>(NU_ALLOC(sizeof(edui_slider_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 6;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSlider;
        item->render = eduicbRenderSlider;
        item->destroy = eduicbItemSliderDestroy;
        item->interact = eduicbInteractSlider;
        item->selection_group = group;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text);
        item->granularity = 0.001f;
        item->changed = callback;
        item->minimum = minimum;
        item->range = range;
        item->format = NULL;
        eduiItemSliderSetFmt(item, (char *)"(%1.03f)");
        eduiItemSliderSetVal(item, value);
        return item;
    }
    eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text) {
        edui_slider_s *item = static_cast<edui_slider_s *>(
            eduiItemSliderCreate(data, colours, group, callback, minimum, range, value, text));
        if (item) {
            eduiItemSliderSetFmt(item, (char *)"(%d)");
            item->granularity = 1.0f;
            item->render = eduicbRenderSliderInt;
        }
        return item;
    }
    void eduiItemSliderSetFmt(edui_slider_s *item, char *format) {
        if (!item->format) {
            u32 size = NuStrLen(format) + 1;
            item->format = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        } else if (NuStrLen(item->format) < NuStrLen(format)) {
            NU_FREE(item->format);
            u32 size = NuStrLen(format) + 1;
            item->format = static_cast<char *>(NU_ALLOC(size, 4, 1, "", 0));
        }
        NuStrCpy(item->format, format);
    }
    void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity) {
        item->granularity = granularity;
    }
    void eduiItemSliderSetVal(edui_slider_s *item, f32 value) {
        eduiItemSliderSetValEx(item, value, 1, 1);
    }
    void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify) {
        item->normalized_value = (value - item->minimum) / item->range;
        item->value = value;
        if (reset_timer)
            item->change_timer = 60;
        if (notify && item->changed)
            item->changed(NULL, item, 0);
    }
    eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        auto *item = static_cast<edui_textpicker_s *>(NU_ALLOC(sizeof(edui_textpicker_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 1;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessTextPick;
        item->render = eduicbRenderTextPick;
        item->destroy = eduicbItemTextPickDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->max_length = 255;
        item->callback = callback;
        item->format = NULL;
        item->cursor = 0;
        eduiItemTextPickSetFmt(item, const_cast<char *>("\"%s\""));
        textrow[0] = const_cast<char *>("1234567890");
        textrow[1] = const_cast<char *>("QWERTYUIOP");
        textrow[2] = const_cast<char *>("ASDFGHJKL_");
        textrow[3] = const_cast<char *>("ZXCVBNM<>?");
        return item;
    }
    void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format) {
        if (item->format) {
            i32 old_length = NuStrLen(item->format);
            i32 new_length = NuStrLen(format);
            if (old_length >= new_length)
                goto copy;
            NU_FREE(item->format);
        }
        item->format = static_cast<char *>(NU_ALLOC(NuStrLen(format) + 1, 4, 1, "", 0));
    copy:
        NuStrCpy(item->format, format);
    }
    eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options) {
        edui_text_selector_s *item =
            static_cast<edui_text_selector_s *>(NU_ALLOC(sizeof(edui_text_selector_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 6;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessSlider;
        item->render = eduicbRenderTextSelector;
        item->destroy = eduicbItemSliderDestroy;
        item->interact = eduicbInteractSlider;
        item->selection_group = group;
        item->text_alignment = 0x40;
        eduiItemSetText(item, text);
        item->format = NULL;
        item->changed = callback;
        item->minimum = 0.0f;
        item->range = static_cast<f32>(count - 1);
        item->granularity = 1.0f;
        eduiItemSliderSetVal(item, static_cast<f32>(selected));
        item->options = options;
        return item;
    }
    eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text) {
        edui_texture_pick_s *item =
            static_cast<edui_texture_pick_s *>(NU_ALLOC(sizeof(edui_texture_pick_s), 4, 1, "", 0));
        if (!item)
            return NULL;
        memset(item, 0, sizeof(*item));
        item->type = 12;
        item->data = data;
        memcpy(item->colours, colours, sizeof(item->colours));
        item->process = eduicbProcessTexturePick;
        item->render = eduicbRenderTexturePick;
        item->destroy = eduicbItemDestroy;
        item->text_alignment = 0x40;
        item->selection_group = 0;
        eduiItemSetText(item, text);
        item->uv_x[0] = 0.4f;
        item->uv_y[0] = 0.4f;
        item->uv_x[1] = 0.5f;
        item->uv_y[1] = 0.5f;
        item->zoom = 1.0f;
        item->selected_corner = 0;
        item->changed = callback;
        return item;
    }
    eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text) {
        eduiitem_s *item = eduiItemSelCreate(data, colours, selected, group, callback, text);
        if (item) {
            item->type = 3;
            item->process = eduicbProcessSel;
            item->render = eduicbRenderCheck;
        }
        return item;
    }
    eduiitem_s *edui_last_item;
    eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last)
            menu->last->next = item;
        else
            menu->first = item;
        item->previous = menu->last;
        item->next = NULL;
        menu->last = item;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        edui_last_item = item;
        return item;
    }
    void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after) {
        if (after) {
            item->next = after->next;
            item->previous = after;
            after->next = item;
            if (item->next)
                item->next->previous = item;
            else
                menu->last = item;
            menu->field_0c = menu->first;
            menu->field_10 = 0;
            edui_last_item = item;
        } else
            eduiMenuAddItemFirst(menu, item);
    }
    void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before) {
        if (before) {
            item->previous = before->previous;
            item->next = before;
            before->previous = item;
            // The original does not update the preceding item's next link.
            if (!item->previous)
                menu->first = item;
            menu->field_0c = menu->first;
            menu->field_10 = 0;
            edui_last_item = item;
        } else
            eduiMenuAddItemLast(menu, item);
    }
    void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first)
            menu->first->previous = item;
        else
            menu->last = item;
        item->next = menu->first;
        item->previous = NULL;
        menu->first = item;
        menu->field_0c = item;
        menu->field_10 = 0;
        edui_last_item = item;
    }
    void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last)
            menu->last->next = item;
        else
            menu->first = item;
        item->previous = menu->last;
        item->next = NULL;
        menu->last = item;
        menu->field_0c = item;
        menu->field_10 = 0;
        edui_last_item = item;
    }
    i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child) {
        if (child->parent)
            return 0;
        while (menu->child)
            menu = menu->child;
        child->parent = menu;
        menu->child = child;
        eduiSetActiveMenu(child);
        return 1;
    }
    eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title) {
        u32 bytes = sizeof(eduimenu_s) + 1;
        if (title)
            bytes += NuStrLen(title);
        eduimenu_s *menu = static_cast<eduimenu_s *>(NU_ALLOC(bytes, 4, 1, "", 0));
        if (menu) {
            memset(menu, 0, bytes);
            menu->field_24 = -1;
            menu->x = x;
            menu->y = y;
            menu->width = width;
            menu->height = height;
            menu->field_28 = -1;
            menu->font = font;
            menu->callback = callback;
            menu->flags = 0;
            if (title) {
                menu->title = reinterpret_cast<char *>(menu + 1);
                NuStrCpy(menu->title, title);
            }
            active_menu = menu;
        }
        edui_last_item = NULL;
        return menu;
    }
    void eduiMenuDestroy(eduimenu_s *menu) {
        if (menu) {
            if (eduiInteractLocked && eduiInteractLocked->menu == menu)
                eduiInteractLocked = NULL;
            if (processing_menu == menu)
                processing_menu = NULL;
            if (menu->parent) {
                if (active_menu == menu)
                    active_menu = menu->parent;
                menu->parent->child = NULL;
            }
            if (default_active_menu == menu)
                default_active_menu = NULL;
            if (active_menu == menu)
                active_menu = NULL;
            if (menu->child)
                menu->child->parent = NULL;
            eduiMenuDestroyItems(menu);
            NU_FREE(menu);
        }
    }
    i32 eduiMenuDetach(eduimenu_s *menu) {
        if (menu->parent) {
            if (menu->child)
                eduiMenuDetach(menu->child);
            if (eduiGetActiveMenu() == menu)
                active_menu = menu->parent;
            menu->parent->child = NULL;
            menu->parent = NULL;
            return 1;
        }
        eduiSetActiveMenu(NULL);
        return 0;
    }
    void eduiMenuEnsureSelection(eduimenu_s *menu) {
        if (!menu->selected)
            menu->selected = menu->first;
    }
    void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding) {
        if (menu->field_24 < 0) {
            i32 saved = edui_donotdraw;
            edui_donotdraw = 1;
            eduiMenuRender(menu);
            edui_donotdraw = saved;
        }
        if (menu->x + menu->field_24 > 640 - padding)
            menu->x -= menu->field_24;
        if (menu->x < padding)
            menu->x = padding;
        if (menu->y + menu->field_28 > 448 - padding)
            menu->y -= menu->field_28;
        if (menu->y < padding)
            menu->y = padding;
    }
    void eduiMenuFitWidth(eduimenu_s *menu, i32 padding) {
        eduicbMenuOpenAllexpanders(menu);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntSetScale(edui_font, edui_font_scale_x, edui_font_scale_y);

        i32 width = menu->title ? static_cast<i32>(NuQFntPrintLenU(edui_font, menu->title)) >> 4 : 0;
        i32 property_label_width = 0;
        i32 property_value_width = 0;

        for (eduiitem_s *item = menu->first; item; item = item->next) {
            i32 item_text_width = 0;
            switch (item->type) {
                case 1:
                    item_text_width = (static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3) * 10;
                    break;
                case 2:
                case 3:
                    if (item->text) {
                        item_text_width = (static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3) +
                                          (static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4);
                    }
                    break;
                case 17: {
                    edui_prop_s *property = static_cast<edui_prop_s *>(item);
                    i32 label_width = item->text ? static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4 : 30;
                    i32 value_width = property->property_text
                                          ? static_cast<i32>(NuQFntPrintLenU(edui_font, property->property_text)) >> 4
                                          : 30;
                    if (label_width > property_label_width)
                        property_label_width = label_width;
                    if (value_width > property_value_width)
                        property_value_width = value_width;
                    item_text_width = value_width;
                    break;
                }
                case 6: {
                    edui_slider_s *slider = static_cast<edui_slider_s *>(item);
                    char format[512];
                    char text[520];
                    NuStrCpy(format, item->text);
                    NuStrCat(format, slider->format);
                    sprintf(text, format, 0, 0x80000000);
                    item_text_width = static_cast<i32>(NuQFntPrintLenU(edui_font, text)) >> 4;
                    break;
                }
                default:
                    if (item->text)
                        item_text_width = static_cast<i32>(NuQFntPrintLenU(edui_font, item->text)) >> 4;
                    break;
            }
            if (item_text_width > width)
                width = item_text_width;
        }

        i32 property_width =
            property_label_width + property_value_width + (static_cast<i32>(NuQFntHeight(edui_font)) >> 3);
        if (property_width > width)
            width = property_width;

        for (eduiitem_s *item = menu->first; item; item = item->next) {
            if (item->type == 17)
                static_cast<edui_prop_s *>(item)->label_width = static_cast<f32>(property_label_width + 1);
        }
        menu->width = width + padding;

        NuQFntPopCoordinateSystem();
        eduicbMenuCloseAllexpanders(menu);
    }
    void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item) {
        if (item->selection_group) {
            for (eduiitem_s *entry = menu->first; entry; entry = entry->next) {
                if (entry == item) {
                    if (entry->type == EDUI_ITEM_TOGGLE)
                        entry->highlighted = 1 - entry->highlighted;
                    else
                        entry->highlighted = 1;
                } else if (entry->selection_group == item->selection_group) {
                    entry->highlighted = 0;
                }
            }
        }
    }
    i32 eduiMenuIsActive(eduimenu_s *menu) {
        if (!eduiGetUsingMenuFocus())
            return 1;
        if (eduiGetUsingMenuFocus() && active_menu == menu)
            return 1;
        return 0;
    }
    i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->last == item)
            return 0;
        eduiitem_s *next = item->next;
        eduiitem_s *previous = item->previous;
        item->previous = next;
        item->next = next->next;
        if (item->next)
            item->next->previous = item;
        else
            menu->last = item;
        if (previous)
            previous->next = next;
        else
            menu->first = next;
        next->next = item;
        next->previous = previous;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        return 1;
    }
    i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first == item)
            return 0;
        eduiitem_s *previous = item->previous;
        eduiitem_s *next = item->next;
        item->next = previous;
        item->previous = previous->previous;
        if (item->previous)
            item->previous->next = item;
        else
            menu->first = item;
        if (next)
            next->previous = previous;
        else
            menu->last = item->next;
        previous->previous = item;
        previous->next = next;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        return 1;
    }
    i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        processing_menu = menu;
        if (menu->child) {
            i32 result = eduiMenuProcess(menu->child, delta_time, pad);
            if (processing_menu) {
                if (result)
                    eduiSetActiveMenu(menu->child ? menu->child : menu);
                else
                    result = eduiProcessInteracts(menu, pad);
            }
            return result;
        }
        return eduiMenuProcessAux(menu, delta_time, pad);
    }
    i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        if (eduiMenuIsActive(menu)) {
            eduiMenuEnsureSelection(menu);
            i32 result = eduiMenuProcessSelectedItem(menu, delta_time, pad);
            if (eduiMenuIsActive(menu) && processing_menu == menu) {
                eduiMenuEnsureSelection(menu);
                if (eduiMenuIsActive(menu) && processing_menu == menu)
                    eduiMenuProcessInput(menu, delta_time, pad, result);
            }
        }
        return eduiProcessInteracts(menu, pad);
    }
    i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result) {
        static f32 up_rept;
        static f32 down_rept;
        cbInteractMenuKeySelect(menu);
        if (item_result || !pad)
            return 0;
        if (menu && menu->selected && menu->selected->input && !menu->selected->disabled && pad->digital_buttons) {
            if (menu->selected->input(menu, menu->selected, pad->digital_buttons, pad->digital_buttons_pressed))
                return 0;
        }
        u32 buttons = pad->digital_buttons;
        u32 pressed = pad->digital_buttons_pressed;
        u32 up_button = (menu->flags & 2) ? 0x8000 : 0x1000;
        u32 down_button = (menu->flags & 2) ? 0x2000 : 0x4000;
        i32 up = 0;
        if (pressed & up_button) {
            up_rept = 1.0f / 3.0f;
            up = 1;
        }
        if ((buttons & up_button) && up_rept > 0.0f) {
            up_rept -= delta_time;
            if (up_rept <= 0.0f) {
                up_rept = 1.0f / 6.0f;
                up = 1;
            }
        }
        if (!(menu->flags & 6)) {
            if ((buttons & 0x8000) && !(menu->flags & 1)) {
                i32 distance = static_cast<i32>(delta_time * 1500.0f);
                if (distance < 1)
                    distance = 1;
                menu->x -= distance;
            }
            if ((buttons & 0x2000) && !(menu->flags & 1)) {
                i32 distance = static_cast<i32>(delta_time * 1500.0f);
                if (distance < 1)
                    distance = 1;
                menu->x += distance;
            }
        }
        i32 down = 0;
        if (pressed & down_button) {
            down_rept = 1.0f / 3.0f;
            down = 1;
        }
        if ((buttons & down_button) && down_rept > 0.0f) {
            down_rept -= delta_time;
            if (down_rept <= 0.0f) {
                down_rept = 1.0f / 6.0f;
                down = 1;
            }
        }
        i32 to_first = up && (buttons & 1);
        i32 to_last = down && (buttons & 1);
        i32 page_up = up && (buttons & 2);
        i32 page_down = down && (buttons & 2);
        if (page_down && menu->field_10) {
            i32 count = 1;
            for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                ++count;
            while (menu->selected->next && count--) {
                menu->selected = menu->selected->next;
            }
            return 0;
        }
        if (page_up && menu->field_10) {
            i32 count = 1;
            for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                ++count;
            menu->selected = menu->field_0c;
            while (menu->selected->previous && count--) {
                menu->selected = menu->selected->previous;
                menu->field_0c = menu->selected;
            }
            return 0;
        }
        if (up && menu->selected && menu->selected->previous) {
            do {
                if (menu->selected == menu->field_0c)
                    menu->field_0c = menu->selected->previous;
                eduiitem_s *item = menu->selected->previous;
                while (item->type == 18)
                    item = item->previous;
                menu->selected = item;
            } while (menu->selected->previous && to_first);
            return 0;
        }
        if (down && menu->selected && menu->selected->next) {
            do {
                eduiitem_s *item = menu->selected->next;
                while (item->type == 18)
                    item = item->next;
                menu->selected = item;
            } while (menu->selected->next && to_last);
            if (to_last) {
                i32 count = 1;
                for (eduiitem_s *item = menu->field_0c; item && item != menu->field_10; item = item->next)
                    ++count;
                menu->field_0c = menu->selected;
                while (menu->field_0c->previous && count--)
                    menu->field_0c = menu->field_0c->previous;
            }
            return 0;
        }
        eduiCheckForPadMenuCancel(menu, pad);
        return 0;
    }
    i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad) {
        if (menu && menu->selected && !(menu->selected->flags & EDUI_ITEM_DISABLED) && menu->selected->process)
            return menu->selected->process(menu, menu->selected, delta_time, pad);
        return 0;
    }
    void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item) {
        if (menu->first == item)
            menu->first = item->next;
        if (menu->last == item)
            menu->last = item->previous;
        if (menu->selected == item)
            menu->selected = item->next ? item->next : item->previous;
        menu->field_0c = menu->first;
        menu->field_10 = 0;
        if (item->next)
            item->next->previous = item->previous;
        if (item->previous)
            item->previous->next = item->next;
        item->next = NULL;
        item->previous = NULL;
    }
    void eduiMenuRender(eduimenu_s *menu) {
        ++NuPrimCSPos;
        i32 x = menu->x;
        i32 y = menu->y;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
        NuQFntPushPrintMode(2);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        NuQFntSetScale(edui_font, edui_font_scale_x, edui_font_scale_y);

        const f32 font_height = NuQFntHeight(edui_font);
        const f32 title_font_height = NuQFntHeight(edui_font);
        const f32 baseline = NuQFntBaseline(edui_font);
        const i32 title_height = static_cast<i32>(font_height * 0.15625f);
        if (menu->title && menu->title[0]) {
            if (!menu->child && numInteracts < 64 && !eduiInteractLocked) {
                edui_interact_s *interact = &eduiInteracts[numInteracts++];
                interact->x = static_cast<f32>(x);
                interact->y = static_cast<f32>(y) * 0.5f;
                interact->width = static_cast<f32>(menu->width);
                interact->height = static_cast<f32>(title_height) * 0.5f;
                interact->menu = menu;
                interact->item = NULL;
                interact->callback = cbInteractMenuTitle;
                interact->buttons = 0x40;
                interact->field_20 = 0;
            }
            if (!edui_donotdraw) {
                NuRndrRect2di(x << 4, y << 3, menu->width << 4, title_height << 3, 0x80000000, uimtls[ui_bgmtl]);
            }
            if (!edui_donotdraw) {
                NuQFntSet(edui_font);
            }
            if (!edui_donotdraw) {
                NuQFntSetColour(edui_font, 0x807f7f7f);
            }
            eduiFntPrintClipEx(edui_font, (x + 1) << 4,
                               static_cast<i32>(title_font_height * 0.125f + baseline) + (y << 3), 0x10, x << 4,
                               menu->width << 4, menu->title);
            y += title_height;
        }

        eduiitem_s *item = menu->field_0c;
        i32 max_item_height = 0;
        if (item) {
            eduiitem_s *selected = menu->selected;
            bool find_selected = selected != NULL;

            if (item != menu->first) {
                const i32 scroll_height = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f);
                if (!menu->child && numInteracts < 64 && !eduiInteractLocked) {
                    edui_interact_s *interact = &eduiInteracts[numInteracts++];
                    interact->x = static_cast<f32>(x);
                    interact->y = static_cast<f32>(y) * 0.5f;
                    interact->width = static_cast<f32>(menu->width);
                    interact->height = static_cast<f32>(scroll_height) * 0.5f;
                    interact->menu = menu;
                    interact->item = NULL;
                    interact->callback = cbInteractMenuScrollUp;
                    interact->buttons = 0x40;
                    interact->field_20 = 0;
                }
                if (!edui_donotdraw) {
                    NuRndrRect2di(x << 4, y << 3, menu->width << 4, scroll_height << 3, 0x80000000, uimtls[ui_bgmtl]);
                }
                const i32 centre = (x * 2 + menu->width) << 3;
                const i32 top = (y << 3) + 8;
                const i32 bottom = ((y + scroll_height) << 3) - 16;
                if (!edui_donotdraw) {
                    NuRndrLine2di(centre, top, centre - ((scroll_height - 2) << 4), bottom, 0x80ffffff, uimtls[0]);
                }
                if (!edui_donotdraw) {
                    NuRndrLine2di(centre, top, centre + ((scroll_height - 2) << 4), bottom, 0x80ffffff, uimtls[0]);
                }
                y += scroll_height;
            }

            menu->field_10 = item;
            while (item) {
                i32 is_selected = 0;
                if (item == selected) {
                    find_selected = false;
                    if (!menu->child)
                        is_selected = 1;
                }

                const i32 item_height = eduiItemRender(item, menu, x, y, menu->width, is_selected);
                if (item_height > max_item_height)
                    max_item_height = item_height;

                if (!menu->child && numInteracts < 64 && !eduiInteractLocked) {
                    edui_interact_s *interact = &eduiInteracts[numInteracts++];
                    interact->x = static_cast<f32>(x);
                    interact->y = static_cast<f32>(y) * 0.5f;
                    interact->width = static_cast<f32>(menu->width);
                    interact->height = static_cast<f32>(item_height) * 0.5f;
                    interact->menu = menu;
                    interact->item = item;
                    interact->callback = reinterpret_cast<edui_sel_s *>(item)->interact;
                    interact->buttons = 0x40;
                    interact->field_20 = 0;
                }

                item = item->next;
                if (!(menu->flags & 2))
                    y += item_height;
                else
                    x += menu->width;

                if (!item)
                    break;
                if (menu->y + menu->height <= y) {
                    const i32 scroll_height = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f);
                    if (!menu->child && numInteracts < 64 && !eduiInteractLocked) {
                        edui_interact_s *interact = &eduiInteracts[numInteracts++];
                        interact->x = static_cast<f32>(x);
                        interact->y = static_cast<f32>(y) * 0.5f;
                        interact->width = static_cast<f32>(menu->width);
                        interact->height = static_cast<f32>(scroll_height) * 0.5f;
                        interact->menu = menu;
                        interact->item = NULL;
                        interact->callback = cbInteractMenuScrollDown;
                        interact->buttons = 0x40;
                        interact->field_20 = 0;
                    }
                    if (!edui_donotdraw) {
                        NuRndrRect2di(x << 4, y << 3, menu->width << 4, scroll_height << 3, 0x80000000,
                                      uimtls[ui_bgmtl]);
                    }
                    const i32 centre = (x * 2 + menu->width) << 3;
                    const i32 top = (y << 3) + 8;
                    const i32 bottom = ((y + scroll_height - 2) << 3);
                    if (!edui_donotdraw) {
                        NuRndrLine2di(centre, bottom, centre - ((scroll_height - 2) << 4), top, 0x80ffffff, uimtls[0]);
                    }
                    if (!edui_donotdraw) {
                        NuRndrLine2di(centre, bottom, centre + ((scroll_height - 2) << 4), top, 0x80ffffff, uimtls[0]);
                    }
                    break;
                }
                menu->field_10 = item;
            }

            if (find_selected && menu->field_0c->next)
                menu->field_0c = menu->field_0c->next;
        }

        if (!(menu->flags & 2))
            x += menu->width;
        else
            y += max_item_height;
        menu->field_24 = x - menu->x;
        menu->field_28 = y - menu->y;

        const u32 outline = eduiGetActiveMenu() == menu ? eduimenu_selected_outline_colour : eduimenu_outline_colour;
        if (!edui_donotdraw)
            NuRndrLineRect2di((menu->x - 1) << 4, (menu->y << 3) - 8, (menu->field_24 + 2) << 4,
                              (menu->field_28 << 3) + 16, outline, uimtls[ui_outmtl]);

        if (menu->child) {
            if (numInteracts < 64 && !eduiInteractLocked) {
                edui_interact_s *interact = &eduiInteracts[numInteracts++];
                interact->x = static_cast<f32>(menu->x);
                interact->y = static_cast<f32>(menu->y) * 0.5f;
                interact->width = static_cast<f32>(menu->field_24);
                interact->height = static_cast<f32>(menu->field_28) * 0.5f;
                interact->menu = menu;
                interact->item = NULL;
                interact->callback = cbInteractMenuCancelChild;
                interact->buttons = 0x40;
                interact->field_20 = 1;
            }
            eduiMenuRender(menu->child);
        }

        NuQFntPopPrintMode();
        NuQFntPopCoordinateSystem();
        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    }
    void eduiMenuSelectFirstEntry(eduimenu_s *menu) {
        menu->selected = NULL;
    }
    void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours) {
        do {
            for (eduiitem_s *item = menu->first; item; item = item->next) {
                item->colours[0] = colours[0];
                item->colours[1] = colours[1];
                item->colours[2] = colours[2];
                item->colours[3] = colours[3];
            }
            menu = menu->child;
        } while (menu);
    }
    void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled) {
        do {
            for (eduiitem_s *item = menu->first; item; item = item->next)
                item->disabled = disabled;
            menu = menu->child;
        } while (menu);
    }
    void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours) {
        for (eduiitem_s *item = menu->first; item; item = item->next) {
            item->colours[0] &= 0xffffff;
            item->colours[0] |= colours[0] & 0xff000000;
            item->colours[1] &= 0xffffff;
            item->colours[1] |= colours[1] & 0xff000000;
            item->colours[2] &= 0xffffff;
            item->colours[2] |= colours[2] & 0xff000000;
            item->colours[3] &= 0xffffff;
            item->colours[3] |= colours[3] & 0xff000000;
        }
    }
    void eduiMenuSortItemsByTxt(eduimenu_s *menu) {
        eduiitem_s *first = menu->first;
        if (first) {
            i32 sorted;
            do {
                sorted = 1;
                eduiitem_s *item = first;
                while (item->next && item->text && item->next->text) {
                    if (item->type != 20 && (item->next->type == 20 || NuStrICmp(item->text, item->next->text) > 0)) {
                        eduiMenuItemMoveDown(menu, item);
                        if (item == first)
                            first = item->previous;
                        sorted = 0;
                    } else {
                        item = item->next;
                    }
                }
            } while (!sorted);
        }
    }
    void eduiProcessCursor(f32 delta_time, nupad_s *pad) {
        eduiProcessCursorDefault(delta_time, pad);
    }
    void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad) {
        if (pad && eduiUsedAlgPad(pad) && !(pad->digital_buttons & EDUI_CURSOR_PRIMARY)) {
            eduiSetCursorCoords(0.5f, 0.5f);
        } else {
            edui_cursor_dx = -NuMouseReadXVel() * 0.1f;
            edui_cursor_dy = -NuMouseReadYVel() * 0.1f;
        }
        edui_cursor_locked = NuKeyboard(0x38) != 0;
        if (!edui_cursor_locked) {
            edui_cursor_x += edui_cursor_dx;
            edui_cursor_y += edui_cursor_dy;
        }
        if (edui_cursor_x < 0.0f)
            edui_cursor_x = 0.0f;
        if (edui_cursor_x > 640.0f)
            edui_cursor_x = 640.0f;
        if (edui_cursor_y < 0.0f)
            edui_cursor_y = 0.0f;
        if (edui_cursor_y > 224.0f)
            edui_cursor_y = 224.0f;
        edui_cursor_buttons_old = edui_cursor_buttons;
        edui_cursor_buttons = 0;
        edui_cursor_buttons |= NuMouseReadButtons() == 1 ? EDUI_CURSOR_PRIMARY : 0;
        edui_cursor_buttons |= NuMouseReadButtons() == 2 ? EDUI_CURSOR_SECONDARY : 0;
        edui_cursor_buttons_db = edui_cursor_buttons & ~edui_cursor_buttons_old;
    }
    i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad) {
        i32 result = 0;
        if (edmainGetCursorEnabled()) {
            if (eduiInteractLocked) {
                if ((!eduiInteractLocked->menu || !eduiInteractLocked->item) && eduiInteractLocked->field_20)
                    eduiInteractLocked = NULL;
            }
            if (eduiInteractLocked) {
                result = 1;
                if (menu == eduiInteractLocked->menu || menu->parent == eduiInteractLocked->menu) {
                    eduiInteractLocked = eduiInteractLocked->callback(eduiInteractLocked) ? eduiInteractLocked : NULL;
                    result = eduiInteractLocked != NULL;
                }
            } else {
                edui_interact_s *interact = eduiInteracts + numInteracts - 1;
                for (i32 i = 0; i < numInteracts; ++i, --interact) {
                    if (interact->menu == menu && (interact->buttons & edui_cursor_buttons_db) &&
                        edui_cursor_x >= interact->x && edui_cursor_y >= interact->y &&
                        edui_cursor_x < interact->x + interact->width &&
                        edui_cursor_y < interact->y + interact->height) {
                        if (menu && interact->item && interact->item->type != 0x12)
                            menu->selected = interact->item;
                        if (interact->field_20 || (interact->buttons & edui_cursor_buttons)) {
                            if (!interact->field_20 && eduiGetActiveMenu() != menu)
                                eduiSetActiveMenu(menu);
                            if (interact->callback && interact->callback(interact))
                                eduiInteractLocked = interact;
                        }
                        result = 1;
                        break;
                    }
                }
            }
        }
        return result;
    }
    void eduiRenderCursor(void) {
        if (edui_cursor_locked)
            return;
        NuPrim2DBegin(0, 7, uimtls[0]);
#define EDUI_CURSOR_VERTEX(xoff, yoff)                                                                                 \
    NuRndrPrimSetColour(edui_cursor_colour);                                                                           \
    NuRndrPrimUV(0.0f, 0.0f);                                                                                          \
    NuPrim2DAddXYZ(edui_cursor_x + (xoff), edui_cursor_y + (yoff), 0.0f)
        EDUI_CURSOR_VERTEX(0.0f, 0.0f);
        EDUI_CURSOR_VERTEX(8.0f, 2.0f);
        EDUI_CURSOR_VERTEX(4.0f, 4.0f);
        EDUI_CURSOR_VERTEX(0.0f, 0.0f);
        EDUI_CURSOR_VERTEX(12.0f, 5.4f);
        EDUI_CURSOR_VERTEX(10.8f, 6.0f);
#undef EDUI_CURSOR_VERTEX
        NuPrim2DEnd();
    }
    void eduiRenderInteracts(void) {
        if (numInteracts < 1)
            return;

        ++NuPrimCSPos;
        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_PS2);
        NuPrim2DBegin(2, 5, uimtls[0]);

        edui_interact_s *hovered = NULL;
        edui_interact_s *interact = eduiInteractLocked ? eduiInteractLocked : eduiInteracts;
        const i32 count = eduiInteractLocked ? 1 : numInteracts;
        for (i32 i = 0; i < count; ++i, ++interact) {
            const u32 colour = NuRandInt();
#define EDUI_INTERACT_VERTEX(x, y)                                                                                     \
    NuRndrPrimSetColour(colour);                                                                                       \
    NuPrim2DAddXYZ((x), (y), 0.0f)
            EDUI_INTERACT_VERTEX(interact->x, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x + interact->width, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y + interact->height);
            EDUI_INTERACT_VERTEX(interact->x, interact->y);
#undef EDUI_INTERACT_VERTEX

            if (interact->x <= edui_cursor_x && interact->y <= edui_cursor_y &&
                edui_cursor_x < interact->x + interact->width && edui_cursor_y < interact->y + interact->height)
                hovered = interact;
        }
        NuPrim2DEnd();

        if (hovered) {
            NuPrim2DBegin(4, 5, uimtls[3]);
            const u32 colour = (NuRandInt() & 0x00ffffff) | 0x40000000;
            NuRndrPrimSetColour(colour);
            NuPrim2DAddXYZ(hovered->x, hovered->y, 0.0f);
            NuRndrPrimSetColour(colour);
            NuPrim2DAddXYZ(hovered->x + hovered->width, hovered->y + hovered->height, 0.0f);
            NuPrim2DEnd();
        }

        --NuPrimCSPos;
        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
    }
    void eduiSetActiveMenu(eduimenu_s *menu) {
        if (eduiGetUsingMenuFocus()) {
            if (menu) {
                while (menu->child)
                    menu = menu->child;
                active_menu = menu;
            } else {
                menu = default_active_menu;
                if (menu)
                    while (menu->child)
                        menu = menu->child;
                active_menu = menu;
            }
        }
    }
    void eduiSetCameraEnabled(i32 enabled) {
        bCameraEnabled = enabled;
    }
    void eduiSetCursorColour(u32 colour) {
        edui_cursor_colour = colour;
    }
    void eduiSetCursorCoords(f32 x, f32 y) {
        edui_cursor_x = x * 640.0f;
        edui_cursor_y = y * 224.0f;
    }
    void eduiSetDefaultActiveMenu(eduimenu_s *menu) {
        default_active_menu = menu;
    }
    void eduiSetFont(NUQFNT *font) {
        edui_font = font;
    }
    void eduiSetFontScale(f32 x, f32 y) {
        edui_font_scale_x = x;
        edui_font_scale_y = y;
    }
    void eduiSetGlobalSliderAccel(f32 acceleration) {
        if (acceleration < 0.0f)
            acceleration = 0.0f;
        else if (acceleration > 1.0f)
            acceleration = 1.0f;
        slider_acceleration = acceleration;
    }
    void eduiSetRenderPlane(i8 plane) {
        NuMtlSetRenderPlane(uimtls[0], plane);
        NuMtlSetRenderPlane(uimtls[1], plane);
        NuMtlSetRenderPlane(uimtls[2], plane);
        NuMtlSetRenderPlane(uimtls[3], plane);
    }
    void eduiSetUsingMenuFocus(i32 enabled) {
        bUsingMenuFocus = enabled;
    }
    void eduiShowCursor(i32 show) {
        bShowCursor = show;
    }
    i32 eduiUsedAlgPad(nupad_s *pad) {
        if (pad) {
            if (pad->analog_left_x < 64 || pad->analog_left_x > 192 || pad->analog_left_y < 64 ||
                pad->analog_left_y > 192)
                return EDUI_ANALOG_PAD_LEFT;
            if (pad->analog_right_x < 64 || pad->analog_right_x > 192 || pad->analog_right_y < 64 ||
                pad->analog_right_y > 192)
                return EDUI_ANALOG_PAD_RIGHT;
        }
        return EDUI_ANALOG_PAD_NONE;
    }
    void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *) {
        eduiMenuDestroy(edui_messagemenu);
        edui_messagemenu = NULL;
    }
    i32 eduicbInteractSlider(edui_interact_s *interact) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(interact->item);
        f32 value = (edui_cursor_x - interact->x) / interact->width;
        value = CLAMP(value, 0.0f, 1.0f);
        eduiItemSliderSetVal(slider, value * slider->range);
        return (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
    }
    void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_interact_s *interact = eduiInteracts;
        for (i32 i = 0; i < numInteracts; ++i, ++interact) {
            if (menu == interact->menu && item == interact->item) {
                interact->menu = NULL;
                interact->item = NULL;
            }
        }
        if (item->text) {
            NU_FREE(item->text);
            item->text = NULL;
        }
        NU_FREE(item);
        menu->field_0c = menu->first;
    }
    void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item) {
        edui_prop_s *prop = static_cast<edui_prop_s *>(item);
        if (prop->text) {
            NU_FREE(prop->text);
            prop->text = NULL;
        }
        if (prop->property_text) {
            NU_FREE(prop->property_text);
            prop->property_text = NULL;
        }
        NU_FREE(prop);
        menu->field_0c = menu->first;
    }

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

    static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *) {
        edmainSetReturn(1);
    }

    static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        if (menu && menu->child && menu->child->callback)
            menu->child->callback(menu->child, menu);
        return 0;
    }

    static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32) {
        edmainSetReturn(1);
    }

    static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32) {
        f32 divisor = 512.0f / static_cast<edui_slider_s *>(item)->value;
        f32 speed = 12.0f / divisor;
        edcamSetSpeed(speed, speed, speed, 1.0f / divisor);
    }

    static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
    }

    static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32) {
        ed_curr = static_cast<ed_module_s *>(selected->data_ptr);
        if (!ed_curr->reserved) {
            ed_module_active = 1;
            if (ed_curr->activate)
                ed_curr->activate();
        }
    }
    static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32) {
        ed_cfg_menu->x = menu->x + 10;
        ed_cfg_menu->y = menu->y + 10;
        eduiMenuAttach(menu, ed_cfg_menu);
    }

    static __used__ void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour) {
        i32 previous_x = 0;
        i32 previous_y = height - static_cast<i32>(height * graph->y[0]);
        f32 fraction = 0.0f;
        for (i32 sample = 0; sample < 101; ++sample) {
            i32 current_x = static_cast<i32>(width * fraction);
            i32 current_y =
                height -
                static_cast<i32>(height * nugraphGetYatX(graph, fraction * graph->x_extent * graph->x_scale, 2));
            if (!edui_donotdraw)
                NuRndrLine2di((x + previous_x) << 4, (y + previous_y) << 3, (x + current_x) << 4, (y + current_y) << 3,
                              colour, uimtls[0]);
            previous_x = current_x;
            previous_y = current_y;
            fraction += 0.01f;
        }
    }

    static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }

    static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker) {
        if (!dir_list)
            return;

        picker->reopen_directory = 0;
        filepick_menu = menu;
        filepick_item = picker;
        eduimenu_s *directory_menu = eduiMenuCreate(menu->x + 20, menu->y + 20, 250, 300, menu->font,
                                                    eduicbDestroyDirectoryList, const_cast<char *>("Directory"));
        if (!directory_menu)
            return;

        char text[1032];
        sprintf(text, "%s%s", picker->directory, picker->filename);
        NUFILE directory = NuFileOpenDir(text);
        if (directory) {
            dir_size = 0;
            auto *entries = static_cast<FilePickDirectoryEntry *>(dir_list);
            while (dir_size < edui_filepick_listsize && NuFileReadDir(directory, &entries[dir_size]) > 0)
                ++dir_size;
            if (picker->compare_entries)
                qsort(entries, dir_size, sizeof(FilePickDirectoryEntry), picker->compare_entries);

            if (picker->filename[0]) {
                eduiitem_s *parent = eduiItemSelCreate(9999, picker->colours, 0, 1, eduicbSelectDirectoryEntry,
                                                       const_cast<char *>(".."));
                parent->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, parent);
            }

            for (i32 index = 0; index < dir_size; ++index) {
                FilePickDirectoryEntry &entry = entries[index];
                if (!(entry.flags & 8))
                    continue;
                sprintf(text, "<%s>", entry.name);
                eduiitem_s *item = eduiItemSelCreate(index, picker->colours, 0, 1, eduicbSelectDirectoryEntry, text);
                item->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, item);
            }

            for (i32 index = 0; index < dir_size; ++index) {
                FilePickDirectoryEntry &entry = entries[index];
                if (entry.flags & 8 || (picker->filename[0x100] && !NuStrIStr(entry.name, picker->filename + 0x100)))
                    continue;
                sprintf(text, "%s  ( size=%d  date=%d/%d/%d %d:%d )", entry.name, entry.size, entry.month, entry.day,
                        entry.year, entry.hour, entry.minute);
                eduiitem_s *item = eduiItemSelCreate(index, picker->colours, 0, 1, eduicbSelectDirectoryEntry, text);
                item->text_alignment = 0x10;
                eduiMenuAddItem(directory_menu, item);
            }

            if (!directory_menu->first)
                eduiMenuAddItem(directory_menu,
                                eduiItemSelCreate(dir_size, picker->colours, 0, 1, eduicbSelectDirectoryExit,
                                                  const_cast<char *>("No Files")));
            NuFileCloseDir(directory);
        }
        eduiMenuFitWidth(directory_menu, 8);
        eduiMenuAttach(menu, directory_menu);
    }

    static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(interact->item);
        if (((edui_cursor_x >= interact->x && edui_cursor_y >= interact->y &&
              edui_cursor_x < interact->x + interact->width &&
              edui_cursor_y < interact->y + interact->height * 0.75f) ||
             pick->dragging_colour) &&
            !pick->dragging_saturation) {
            pick->cursor_x = (edui_cursor_x - interact->x) / interact->width;
            pick->cursor_y = (edui_cursor_y - interact->y) / (interact->height * 0.75f);
            pick->dragging_colour = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        }
        if (((edui_cursor_x >= interact->x && edui_cursor_y >= interact->y + interact->height * 0.75f &&
              edui_cursor_x < interact->x + interact->width &&
              edui_cursor_y < interact->y + interact->height * 0.75f + interact->height * 0.125f) ||
             pick->dragging_saturation) &&
            !pick->dragging_colour) {
            pick->saturation = (edui_cursor_x - interact->x) / interact->width;
            pick->dragging_saturation = (edui_cursor_buttons & EDUI_CURSOR_PRIMARY) != 0;
        }
        if (pick->cursor_x > 1.0f)
            pick->cursor_x = 1.0f;
        else if (pick->cursor_x < 0.0f)
            pick->cursor_x = 0.0f;
        if (pick->cursor_y > 1.0f)
            pick->cursor_y = 1.0f;
        else if (pick->cursor_y < 0.0f)
            pick->cursor_y = 0.0f;
        if (pick->saturation > 1.0f)
            pick->saturation = 1.0f;
        else if (pick->saturation < 0.0f)
            pick->saturation = 0.0f;
        pick->hue = pick->cursor_x * 360.0f;
        pick->value = pick->cursor_y;
        eduiHSVToRGB(pick->hue, pick->saturation, pick->value, pick->red, pick->green, pick->blue);
        if (edui_cursor_x >= interact->x &&
            edui_cursor_y >= interact->y + interact->height * 0.75f + interact->height * 0.125f &&
            edui_cursor_x < interact->x + interact->width && edui_cursor_y < interact->y + interact->height &&
            (edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY)) {
            pick->dragging_colour = 0;
            pick->dragging_saturation = 0;
            pick->confirm = 1;
        }
        return pick->dragging_colour || pick->dragging_saturation;
    }
    static void eduicbItemExpanderClose(edui_expander_s *expander) {
        for (eduiitem_s *child = expander->first_child; child; child = child->next) {
            if (child->type == EDUI_ITEM_EXPANDER)
                eduicbItemExpanderClose(static_cast<edui_expander_s *>(child));
            if (child == expander->last_child)
                break;
        }
        if (expander->first_child && expander->open) {
            expander->next = expander->last_child->next;
            if (expander->last_child->next)
                expander->last_child->next->previous = expander;
            expander->first_child->previous = NULL;
            expander->open = 0;
        }
    }
    static __used__ i32 eduicbInteractExpander(edui_interact_s *interact) {
        if (edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY) {
            edui_expander_s *expander = static_cast<edui_expander_s *>(interact->item);
            if (expander->changed)
                expander->changed(interact->menu, expander, 0);
            if (edui_cursor_x >= expander->button_x && edui_cursor_y >= expander->button_y * 0.5f &&
                edui_cursor_x < expander->button_x + expander->button_size &&
                edui_cursor_y < (expander->button_y + expander->button_size) * 0.5f && expander->first_child) {
                if (!expander->open) {
                    expander->last_child->next = expander->next;
                    if (expander->next)
                        expander->next->previous = expander->last_child;
                    expander->first_child->previous = expander;
                    expander->next = expander->first_child;
                    expander->open = 1;
                } else {
                    eduicbItemExpanderClose(expander);
                }
            }
        }
        return 0;
    }
    static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact) {
        edui_prop_s *property = static_cast<edui_prop_s *>(interact->item);
        if (!(edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY) || (property->unknown_property_flags & 0x0a))
            return (property->unknown_property_flags & 0x0b) != 0;
        f32 label_end = interact->x + property->label_width;
        f32 cursor_bottom = interact->y + interact->height;
        bool in_row = edui_cursor_y >= interact->y && edui_cursor_y < cursor_bottom;
        if (property->unknown_property_flags & 1) {
            if (in_row && edui_cursor_x >= label_end + 1.0f && edui_cursor_x < property->button_x) {
                property->unknown_property_flags |= 1;
                eduiPropTextPos = -1;
                return 1;
            }
            return (property->unknown_property_flags & 0x0b) != 0;
        }
        edui_prop_s *bounds_property = property;
        if (in_row && edui_cursor_x >= interact->x && edui_cursor_x < label_end - 1.0f && property->selected) {
            property->selected(interact->menu, property, 0);
            bounds_property = static_cast<edui_prop_s *>(interact->item);
            label_end = interact->x + bounds_property->label_width;
        }
        if (in_row && edui_cursor_x >= label_end - 1.0f && edui_cursor_x < label_end + 1.0f) {
            property->unknown_property_flags |= 0x06;
            bounds_property = static_cast<edui_prop_s *>(interact->item);
            label_end = interact->x + bounds_property->label_width;
        }
        if (in_row && edui_cursor_x >= label_end + 1.0f && edui_cursor_x < bounds_property->button_x &&
            !(property->unknown_property_flags & 1)) {
            if (property->selected)
                property->selected(interact->menu, property, 0);
            NuKeyFlush();
            property->unknown_property_flags |= 1;
            NuStrCpy(eduiPropTextStore, property->property_text);
            NuStrCpy(eduiPropTextEdit, property->property_text);
            eduiPropTextPos = NuStrLen(property->property_text);
        }
        if ((eduiInteractLocked ||
             (edui_cursor_x >= property->button_x && edui_cursor_x < property->button_x + property->button_size &&
              edui_cursor_y >= property->button_y * 0.5f &&
              edui_cursor_y < (property->button_y + property->button_size) * 0.5f)) &&
            property->button) {
            property->button(interact->menu, property, 0);
            if (property->unknown_property_flags & 0x20)
                return 1;
        }
        return (property->unknown_property_flags & 0x0b) != 0;
    }
    static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact) {
        return eduicbInteractProp(interact);
    }
    static __used__ i32 eduicbInteractSel(edui_interact_s *interact) {
        eduimenu_s *menu = interact->menu;
        edui_sel_s *item = static_cast<edui_sel_s *>(interact->item);
        if (interact->callback) {
            if (edui_cursor_buttons_db & EDUI_CURSOR_PRIMARY)
                item->selected(menu, item, edui_cursor_buttons);
            else if ((edui_cursor_buttons & EDUI_CURSOR_PRIMARY) && item->held)
                item->held(menu, item, edui_cursor_buttons);
        }
        return 0;
    }
    static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item) {
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        if (expander->last_child)
            expander->last_child->next = NULL;
        while (expander->first_child) {
            eduiitem_s *next = expander->first_child->next;
            expander->first_child->destroy(menu, expander->first_child);
            expander->first_child = next;
        }
    }
    static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item) {
        edui_filter_s *filter = static_cast<edui_filter_s *>(item);
        while (filter->first_child) {
            eduiitem_s *next = filter->first_child->next;
            filter->first_child->destroy(menu, filter->first_child);
            filter->first_child = next;
        }
        eduicbItemDestroyProp(menu, item);
    }
    static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_file_pick_s *file_pick = static_cast<edui_file_pick_s *>(item);
        if (file_pick->format) {
            NU_FREE(file_pick->format);
            file_pick->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }
    static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item) {
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        while (gradient->first_stage)
            eduiGradStageDelete(gradient, gradient->first_stage);
        NU_FREE(gradient);
    }
    static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if (slider->format) {
            NU_FREE(slider->format);
            slider->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }
    static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item) {
        edui_textpicker_s *text_pick = static_cast<edui_textpicker_s *>(item);
        if (text_pick->format) {
            NU_FREE(text_pick->format);
            text_pick->format = NULL;
        }
        eduicbItemDestroy(menu, item);
    }
    static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *) {
        eduiMenuDestroy(menu);
    }

    static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        if (!edui_cursor_buttons) {
            pick->dragging_colour = 0;
            pick->dragging_saturation = 0;
        }
        f32 dx = NuPs2ApplyDeadZone(pad->analog_left_x, 32) * 0.001f;
        f32 dy = NuPs2ApplyDeadZone(pad->analog_left_y, 32) * 0.001f;
        pick->cursor_x += dx;
        pick->cursor_y += dy;
        if (pick->cursor_x < 0.0f)
            pick->cursor_x = 0.0f;
        else if (pick->cursor_x > 1.0f)
            pick->cursor_x = 1.0f;
        if (pick->cursor_y < 0.0f)
            pick->cursor_y = 0.0f;
        else if (pick->cursor_y > 1.0f)
            pick->cursor_y = 1.0f;
        if (pad->analog_l2) {
            pick->saturation -= static_cast<u32>(pad->analog_l2) * delta_time * 0.005f;
            if (pick->saturation < 0.0f)
                pick->saturation = 0.0f;
            return 1;
        }
        if (pad->analog_r2) {
            pick->saturation += static_cast<u32>(pad->analog_r2) * delta_time * 0.005f;
            if (pick->saturation > 1.0f)
                pick->saturation = 1.0f;
            return 1;
        }
        pick->hue = pick->cursor_x * 360.0f;
        pick->value = pick->cursor_y;
        eduiHSVToRGB(pick->hue, pick->saturation, pick->value, pick->red, pick->green, pick->blue);
        if (((pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) || pick->confirm) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }
    static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        static i32 rep_timer = 30;
        static f32 dynascale;
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if ((pad->digital_buttons & 0x0c) == 0x0c) {
            if (slider && slider->minimum <= 0.0f && slider->minimum + slider->range >= 0.0f) {
                slider->value = 0.0f;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
            rep_timer = 30;
        } else if ((pad->digital_buttons & 0x03) == 0x03) {
            if (slider && slider->minimum <= 0.0f && slider->minimum + slider->range >= 255.0f) {
                slider->value = 255.0f;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
            rep_timer = 30;
        } else if (pad->digital_buttons & 0x04) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x04)) {
                f32 value = slider->value - slider->granularity;
                if (value < slider->minimum)
                    value = slider->minimum;
                f32 rounded = value > 0.0f ? value + slider->granularity * 0.5f : value - slider->granularity * 0.5f;
                slider->value = static_cast<i32>(rounded / slider->granularity) * slider->granularity;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
        } else if (pad->digital_buttons & 0x08) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x08)) {
                f32 value = slider->value + slider->granularity;
                if (value > slider->minimum + slider->range)
                    value = slider->minimum + slider->range;
                f32 rounded = value > 0.0f ? value + slider->granularity * 0.5f : value - slider->granularity * 0.5f;
                slider->value = static_cast<i32>(rounded / slider->granularity) * slider->granularity;
                slider->normalized_value = (slider->value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            }
        } else {
            rep_timer = 30;
            if (pad->analog_l2) {
                f32 scale;
                if (pad->unknown_a9) {
                    dynascale *= 1.05f;
                    scale = dynascale;
                } else {
                    dynascale = 1.627604291343232e-7f;
                    scale = 1.627604251552839e-5f;
                }
                f32 value = slider->normalized_value - static_cast<u32>(pad->analog_l2) * scale;
                if (value < 0.0f)
                    value = 0.0f;
                slider->normalized_value = value;
                slider->value = value * slider->range + slider->minimum;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            } else if (pad->analog_r2) {
                f32 scale;
                if (pad->unknown_a9) {
                    dynascale *= 1.05f;
                    scale = dynascale;
                } else {
                    dynascale = 1.627604291343232e-7f;
                    scale = 1.627604251552839e-5f;
                }
                f32 value = slider->normalized_value + static_cast<u32>(pad->analog_r2) * scale;
                if (value > 1.0f)
                    value = 1.0f;
                slider->normalized_value = value;
                slider->value = value * slider->range + slider->minimum;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
            } else {
                dynascale = 1.627604291343232e-7f;
            }
        }
        return 0;
    }
    static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        if (pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) {
            if (expander->changed)
                expander->changed(menu, item, 0);
            if (expander->first_child) {
                if (!expander->open) {
                    expander->last_child->next = item->next;
                    if (item->next)
                        item->next->previous = expander->last_child;
                    expander->first_child->previous = item;
                    item->next = expander->first_child;
                    expander->open = 1;
                } else {
                    eduicbItemExpanderClose(expander);
                }
            }
        }
        return 0;
    }
    static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        auto *picker = static_cast<edui_file_pick_s *>(item);
        if ((pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) || picker->reopen_directory) {
            eduiMenuHighlight(menu, item);
            eduicbAttachDirectoryList(menu, picker);
        }
        return 0;
    }
    static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        auto *filter = static_cast<edui_filter_s *>(item);
        (void)delta_time;
        if (filter->unknown_property_flags & 2) {
            filter->label_width = edui_cursor_x - static_cast<f32>(item->x);
            if (filter->label_width < 1.0f)
                filter->label_width = 1.0f;
            for (eduiitem_s *other = menu->first; other; other = other->next) {
                if (other->type == 17 && other != item)
                    static_cast<edui_prop_s *>(other)->label_width = filter->label_width;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                filter->unknown_property_flags &= ~2;
        }
        eduicbProcessPropKeyboard(menu, filter);
        eduiSetCameraEnabled(1);
        if ((filter->unknown_property_flags & 8) ||
            (pad && item->type == 17 && (pad->digital_buttons & EDUI_CURSOR_PRIMARY))) {
            if (filter->button) {
                eduiSetCameraEnabled(0);
                filter->button(menu, item, edui_cursor_buttons);
                filter->unknown_property_flags &= ~8;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                filter->unknown_property_flags &= ~8;
        }
        char *query = (filter->unknown_property_flags & 1) ? eduiPropTextEdit : filter->property_text;
        bool changed = false;
        if (!*query) {
            for (eduiitem_s *child = filter->first_child; child;) {
                eduiitem_s *next = child->next;
                eduiItemFilterRemoveItem(filter, child);
                eduiMenuAddItem(menu, child);
                child = next;
                changed = true;
            }
        } else {
            for (eduiitem_s *candidate = menu->first; candidate;) {
                eduiitem_s *next = candidate->next;
                if (candidate->type != 18 && candidate->type != 20 && !NuStrIStr(candidate->text, query)) {
                    eduiMenuRemoveItem(menu, candidate);
                    eduiItemFilterAddItem(filter, candidate);
                    changed = true;
                }
                candidate = next;
            }
            for (eduiitem_s *candidate = filter->first_child; candidate;) {
                eduiitem_s *next = candidate->next;
                if (NuStrIStr(candidate->text, query)) {
                    eduiItemFilterRemoveItem(filter, candidate);
                    eduiMenuAddItem(menu, candidate);
                    changed = true;
                }
                candidate = next;
            }
        }
        if (changed || !(filter->unknown_84[4] & 1)) {
            eduiMenuSortItemsByTxt(menu);
            filter->unknown_84[4] |= 1;
        }
        return 0;
    }
    static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        if (gradient->changed)
            gradient->changed(menu, item, pad->digital_buttons);
        if (pad->digital_buttons_pressed & 0x40) {
            static eduimenu_s *gpcfg;
            gpcfg = eduiMenuCreate(menu->x + 10, menu->y + 40, 180, 250, menu->font, eduicbPickCFGCancel,
                                   "GradPick Adjust");
            if (!gpcfg)
                return 0;
            eduiitem_s *pick;
            if (item->type == 8)
                pick = eduiItemGreyPickCreate(reinterpret_cast<usize>(item), &ed_attr, cbgpcfgCPPress, "Colourpick");
            else if (item->type == 9)
                pick = eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgCPPress, "Edit");
            else
                pick = eduiItemColourPickCreate(reinterpret_cast<usize>(item), &ed_attr, cbgpcfgCPPress, "Colourpick");
            if (item->type != 9 && gradient->selected_stage)
                eduiItemColourPickSetHSV(static_cast<edui_colour_pick_s *>(pick), gradient->selected_stage->hue,
                                         gradient->selected_stage->saturation, gradient->selected_stage->value);
            eduiMenuAddItem(gpcfg, pick);
            eduiMenuAddItem(gpcfg, eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgAdd, "Add"));
            eduiMenuAddItem(gpcfg, eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgDel, "Del"));
            eduiMenuAddItem(gpcfg,
                            eduiItemSelCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0, cbgpcfgCopy, "Copy"));
            edui_last_item->type = item->type;
            eduiMenuAddItem(gpcfg, eduiItemSelWithClipColourCreate(reinterpret_cast<usize>(item), &ed_attr, 0, 0,
                                                                   cbgpcfgPaste, "Paste"));
            edui_last_item->type = item->type;
            eduiMenuAttach(menu, gpcfg);
            return 0;
        }
        if (!gradient->selected_stage)
            gradient->selected_stage = gradient->first_stage;
        edui_gradient_node_s *stage = gradient->selected_stage;
        if (!stage)
            return 0;
        if (pad->digital_buttons_pressed & 0x8000) {
            if (stage->previous)
                gradient->selected_stage = stage->previous;
            return 1;
        }
        if (pad->digital_buttons_pressed & 0x2000) {
            if (stage->next)
                gradient->selected_stage = stage->next;
            return 1;
        }
        if (pad->digital_buttons & 0xa000)
            return 1;
        f32 lower = stage->previous ? stage->previous->time : 0.0f;
        f32 upper = stage->next ? stage->next->time : 1.0f;
        u8 right2 = pad->analog_r2;
        u8 left2 = pad->analog_l2;
        u8 decrease = pad->analog_l1;
        u8 increase = pad->analog_r1;
        if (decrease)
            goto decrease_time;
        if (increase)
            goto increase_time;
        decrease = left2;
        if (decrease)
            goto decrease_time;
        increase = right2;
        if (!increase)
            return 0;
    increase_time:
        stage->time += static_cast<f32>(static_cast<i32>(increase)) / 15360.0f;
        if (stage->time > upper)
            stage->time = upper;
        gradient->change_timer = 60;
        return 0;
    decrease_time:
        stage->time -= static_cast<f32>(static_cast<i32>(decrease)) / 15360.0f;
        if (stage->time < lower)
            stage->time = lower;
        gradient->change_timer = 60;
        return 0;
    }
    static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_graph_s *graph_item = static_cast<edui_graph_s *>(item);
        show_y_scale_help = show_y_scale_help > 0 ? show_y_scale_help - 1 : 0;
        show_x_scale_help = show_x_scale_help > 0 ? show_x_scale_help - 1 : 0;
        f32 x = NuPs2ApplyDeadZone(pad->analog_left_x, 32) * (1.0f / 8192.0f);
        f32 y = -NuPs2ApplyDeadZone(pad->analog_left_y, 32) * (1.0f / 8192.0f);
        x += graph_item->cursor_x;
        y += graph_item->cursor_y;
        if (x < 0.0f)
            x = 0.0f;
        else if (x > 1.0f)
            x = 1.0f;
        if (y < 0.0f)
            y = 0.0f;
        else if (y > 1.0f)
            y = 1.0f;
        graph_item->cursor_x = x;
        graph_item->cursor_y = y;

        u32 left = pad->analog_l2;
        u32 right = pad->analog_r2;
        if (left && right) {
            graph_item->graph->x_scale = 1.0f;
            show_x_scale_help = 60;
            return 1;
        }
        if (left > 55) {
            graph_item->graph->x_scale -= left * 0.02f / 255.0f;
            if (graph_item->graph->x_scale < 0.01f)
                graph_item->graph->x_scale = 0.01f;
            show_x_scale_help = 60;
            return 1;
        }
        if (right > 55) {
            graph_item->graph->x_scale += right * 0.02f / 255.0f;
            show_x_scale_help = 60;
            return 1;
        }

        left = pad->analog_l1;
        right = pad->analog_r1;
        if (left && right) {
            graph_item->graph->y_scale = 1.0f;
            show_y_scale_help = 60;
            return 1;
        }
        if (left) {
            graph_item->graph->y_scale -= left * 0.02f / 255.0f;
            if (graph_item->graph->y_scale < 0.01f)
                graph_item->graph->y_scale = 0.01f;
            show_y_scale_help = 60;
            return 1;
        }
        if (right) {
            graph_item->graph->y_scale += right * 0.02f / 255.0f;
            show_y_scale_help = 60;
            return 1;
        }

        u32 pressed = pad->digital_buttons_pressed;
        if ((pressed & 0x80) && graph_item->selected_point >= 0) {
            nugraphDeletePoint(graph_item->graph, graph_item->selected_point);
            nugraphCalcCurve(graph_item->graph, 100);
            return 1;
        }
        if (pressed & 0x20) {
            nugraphAddPoint(graph_item->graph, graph_item->cursor_x, graph_item->cursor_y);
            nugraphCalcCurve(graph_item->graph, 100);
            return 1;
        }
        if ((pressed & 0x40) && graph_item->selected_point >= 0) {
            graph_item->cursor_x = graph_item->graph->x[graph_item->selected_point];
            graph_item->cursor_y = graph_item->graph->y[graph_item->selected_point];
        }
        if ((pad->digital_buttons & 0x40) && graph_item->selected_point >= 0) {
            i32 index = graph_item->selected_point;
            nugraph_s *graph = graph_item->graph;
            graph->x[index] = graph_item->cursor_x;
            graph->y[index] = graph_item->cursor_y;
            if (index == 0) {
                graph->x[index] = 0.0f;
            } else if (index == graph->point_count - 1) {
                graph->x[index] = 1.0f;
            } else if (graph->x[index] <= graph->x[index - 1]) {
                graph->x[index] = graph->x[index - 1] + 0.01f;
            } else if (graph->x[index] >= graph->x[index + 1]) {
                graph->x[index] = graph->x[index + 1] - 0.01f;
            }
            nugraphCalcCurve(graph, 100);
            return 1;
        }
        if ((pressed & 0x10) && graph_item->changed)
            graph_item->changed(menu, item, pressed);
        return 0;
    }
    static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        i32 left = pad->analog_l1 ? pad->analog_l1 : pad->analog_l2;
        i32 right = pad->analog_r1 ? pad->analog_r1 : pad->analog_r2;
        if (left) {
            pick->value -= left / 4192.0f;
            if (pick->value < 0.0f)
                pick->value = 0.0f;
            return 1;
        }
        if (right) {
            pick->value += right / 4192.0f;
            if (pick->value > 1.0f)
                pick->value = 1.0f;
            return 1;
        }
        pick->saturation = 0.0f;
        pick->hue = 0.0f;
        if ((pad->digital_buttons_pressed & 0x40) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }
    static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property) {
        if (!(property->unknown_property_flags & 1))
            return 0;
        i32 length = NuStrLen(eduiPropTextEdit);
        if (eduiPropTextPos < 0)
            eduiPropTextPos = 0;
        if (eduiPropTextPos > length)
            eduiPropTextPos = length;
        u32 modifiers = 0;
        i32 key = NuKeyGet(&modifiers);
        if (key == -1)
            return 0;
        if (key == 0x1c) {
            eduiItemPropSetText(property, eduiPropTextEdit);
            if (property->changed)
                property->changed(menu, property, 0);
            property->unknown_property_flags &= ~1;
            eduiPropTextPos = -1;
            return 0;
        }
        if (key < 0x1d) {
            if (key == 1) {
                property->unknown_property_flags &= ~1;
                eduiPropTextPos = -1;
                return 0;
            }
            if (key == 0xe) {
                if (eduiPropTextPos < 1)
                    return 0;
                char *cursor = eduiPropTextEdit + eduiPropTextPos - 1;
                --eduiPropTextPos;
                char character = *cursor;
                while (character) {
                    character = cursor[1];
                    *cursor++ = character;
                }
                return 0;
            }
        } else {
            if (key == 0xcd) {
                if (eduiPropTextPos < length)
                    ++eduiPropTextPos;
                return 0;
            }
            if (key == 0xd3) {
                if (eduiPropTextPos >= length)
                    return 0;
                char *cursor = eduiPropTextEdit + eduiPropTextPos;
                char character = *cursor;
                while (character) {
                    character = cursor[1];
                    *cursor++ = character;
                }
                return 0;
            }
            if (key == 0xcb) {
                if (eduiPropTextPos > 0)
                    --eduiPropTextPos;
                return 0;
            }
        }
        char letter = NuKeyToAscii(key, modifiers & 1);
        if (letter) {
            eduiPropTextEdit[length + 1] = '\0';
            char *cursor = eduiPropTextEdit + length;
            char *insertion = eduiPropTextEdit + eduiPropTextPos;
            if (insertion < cursor) {
                do {
                    *cursor = cursor[-1];
                    --cursor;
                } while (cursor != insertion);
            }
            *cursor = letter;
            ++eduiPropTextPos;
        }
        return 0;
    }
    static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad) {
        (void)delta_time;
        edui_prop_s *property = static_cast<edui_prop_s *>(item);
        if (property->unknown_property_flags & 2) {
            property->label_width = edui_cursor_x - static_cast<f32>(item->x);
            if (property->label_width < 1.0f)
                property->label_width = 1.0f;
            for (eduiitem_s *other = menu->first; other; other = other->next) {
                if (other->type == 17 && other != item)
                    static_cast<edui_prop_s *>(other)->label_width = property->label_width;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                property->unknown_property_flags &= ~2;
        }
        eduicbProcessPropKeyboard(menu, property);
        eduiSetCameraEnabled(1);
        if ((property->unknown_property_flags & 8) ||
            (pad && item->type == 17 && (pad->digital_buttons & EDUI_CURSOR_PRIMARY))) {
            if (property->button) {
                eduiSetCameraEnabled(0);
                property->button(menu, item, edui_cursor_buttons);
                property->unknown_property_flags &= ~8;
            }
            if (!(edui_cursor_buttons & EDUI_CURSOR_PRIMARY))
                property->unknown_property_flags &= ~8;
        }
        return 0;
    }
    static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_sel_s *selection = static_cast<edui_sel_s *>(item);
        if (pad->digital_buttons_pressed & EDUI_CURSOR_PRIMARY) {
            eduiMenuHighlight(menu, item);
            if (selection->selected)
                selection->selected(menu, item, pad->digital_buttons);
        } else if ((pad->digital_buttons & EDUI_CURSOR_PRIMARY) && selection->held) {
            selection->held(menu, item, pad->digital_buttons);
        }
        return 0;
    }
    static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *) {
        return 0;
    }
    static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        static i32 rep_timer = 30;
        static f32 accel = 1.0f;
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        if ((pad->digital_buttons & 0x0c) == 0x0c) {
            f32 value;
            if (slider->minimum <= 0.0f && slider->minimum + slider->range >= 0.0f)
                value = 0.0f;
            else if (slider->minimum <= 1.0f && slider->minimum + slider->range >= 1.0f)
                value = 1.0f;
            else {
                rep_timer = 30;
                return 0;
            }
            slider->value = value;
            slider->normalized_value = (value - slider->minimum) / slider->range;
            slider->change_timer = 60;
            if (slider->changed)
                slider->changed(menu, item, pad->digital_buttons);
            rep_timer = 30;
        } else if (pad->digital_buttons & 0x04) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x04)) {
                f32 step = slider->granularity * accel;
                f32 value = slider->value - step;
                if (value < slider->minimum)
                    value = slider->minimum;
                value = static_cast<i32>((value > 0.0f ? value + step * 0.5f : value - step * 0.5f) / step) * step;
                slider->value = value;
                slider->normalized_value = (value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
                accel += slider_acceleration;
            }
        } else if (pad->digital_buttons & 0x08) {
            if (rep_timer > 0)
                --rep_timer;
            else
                rep_timer = 0;
            if (!rep_timer || (pad->digital_buttons_pressed & 0x08)) {
                f32 step = slider->granularity * accel;
                f32 value = slider->value + step;
                if (value > slider->minimum + slider->range)
                    value = slider->minimum + slider->range;
                value = static_cast<i32>((value > 0.0f ? value + step * 0.5f : value - step * 0.5f) / step) * step;
                slider->value = value;
                slider->normalized_value = (value - slider->minimum) / slider->range;
                slider->change_timer = 60;
                if (slider->changed)
                    slider->changed(menu, item, pad->digital_buttons);
                accel += slider_acceleration;
            }
        } else {
            rep_timer = 30;
            accel = 1.0f;
        }
        return 0;
    }
    static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        auto *picker = static_cast<edui_textpicker_s *>(item);
        u32 modifiers;
        i32 key = NuKeyGet(&modifiers);
        NuStrLen(picker->value);

        auto delete_previous = [&]() {
            if (picker->cursor <= 0)
                return;
            --picker->cursor;
            if (picker->value[picker->cursor + 1])
                memmove(picker->value + picker->cursor, picker->value + picker->cursor + 1,
                        sizeof(picker->value) - picker->cursor - 1);
            else
                picker->value[picker->cursor] = '\0';
        };
        auto insert_character = [&](char character) {
            if (NuStrLen(picker->value) >= picker->max_length)
                return;
            if (picker->cursor < static_cast<i32>(sizeof(picker->value)))
                memmove(picker->value + picker->cursor + 1, picker->value + picker->cursor,
                        sizeof(picker->value) - picker->cursor - 1);
            picker->value[picker->cursor++] = character;
        };
        auto next_row = [&]() {
            if (++picker->keyboard_row > ((picker->keyboard_flags & 2) ? 5 : 3))
                picker->keyboard_row = 0;
        };
        auto previous_row = [&]() {
            if (--picker->keyboard_row < 0)
                picker->keyboard_row = (picker->keyboard_flags & 2) ? 5 : 3;
        };
        auto next_column = [&]() {
            if (picker->keyboard_row >= 4)
                return;
            if (++picker->keyboard_column >= NuStrLen(textrow[picker->keyboard_row]))
                picker->keyboard_column = 0;
        };
        auto previous_column = [&]() {
            if (picker->keyboard_row >= 4)
                return;
            if (--picker->keyboard_column < 0)
                picker->keyboard_column = NuStrLen(textrow[picker->keyboard_row]) - 1;
        };

        switch (key) {
            case 0xc8:
                previous_row();
                break;
            case 0xd0:
                next_row();
                break;
            case 0xcb:
                previous_column();
                break;
            case 0xcd:
                next_column();
                break;
            case 0x0e:
            case 0xd3:
                delete_previous();
                break;
            case 0x1c:
            case 1:
                if (picker->callback)
                    picker->callback(menu, item, pad->digital_buttons);
                break;
            default:
                if (key != -1) {
                    char character = NuKeyToAscii(key, modifiers & 1);
                    if (character)
                        insert_character(character);
                }
                break;
        }

        if (!picker->editing) {
            if (pad->digital_buttons_pressed & 0x40)
                eduiAddTextPickEnt(menu, item);
            return 0;
        }

        if (pad->digital_buttons_pressed & 0x4000)
            next_row();
        if (pad->digital_buttons_pressed & 0x1000)
            previous_row();
        if (pad->digital_buttons_pressed & 0x8000)
            previous_column();
        if (pad->digital_buttons_pressed & 0x2000)
            next_column();
        if ((pad->digital_buttons_pressed & 0x10) && picker->callback)
            picker->callback(menu, item, pad->digital_buttons);
        if ((pad->digital_buttons & 0xc) == 0xc) {
            picker->cursor = 0;
            picker->value[0] = '\0';
            if ((picker->keyboard_flags & 2) && TextPickCopyBuffer[0]) {
                NuStrNCpy(picker->value, TextPickCopyBuffer, picker->max_length - 1);
                picker->value[picker->max_length - 1] = '\0';
                picker->cursor = NuStrLen(picker->value);
            }
        }
        if ((pad->digital_buttons & 3) == 3 && (picker->keyboard_flags & 2)) {
            NuStrNCpy(TextPickCopyBuffer, picker->value, 0x40);
            picker->value[0x3f] = '\0';
        }
        if (pad->digital_buttons_pressed & 4)
            picker->cursor = picker->cursor > 0 ? picker->cursor - 1 : 0;
        if (pad->digital_buttons_pressed & 8) {
            if (picker->cursor < NuStrLen(picker->value))
                ++picker->cursor;
            else
                picker->cursor = NuStrLen(picker->value);
        }
        if (pad->digital_buttons_pressed & 0x40)
            insert_character(textrow[picker->keyboard_row][picker->keyboard_column]);
        if (pad->digital_buttons_pressed & 0x80)
            delete_previous();
        if (pad->digital_buttons_pressed & 0x20) {
            bool uppercase = textrow[1] == const_cast<char *>("QWERTYUIOP");
            textrow[0] = const_cast<char *>("1234567890");
            textrow[1] = uppercase ? const_cast<char *>("qwertyuiop") : const_cast<char *>("QWERTYUIOP");
            textrow[2] = uppercase ? const_cast<char *>("asdfghjkl_") : const_cast<char *>("ASDFGHJKL_");
            textrow[3] = uppercase ? const_cast<char *>("zxcvbnm<>?") : const_cast<char *>("ZXCVBNM<>?");
        }
        return 0;
    }
    static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad) {
        edui_texture_pick_s *pick = static_cast<edui_texture_pick_s *>(item);
        f32 x = NuPs2ApplyDeadZone(pad->analog_left_x, 32);
        f32 y = NuPs2ApplyDeadZone(pad->analog_left_y, 32);
        i32 corner = pick->selected_corner;
        x = pick->uv_x[corner] + x / (8192.0f * pick->zoom);
        y = pick->uv_y[corner] + y / (8192.0f * pick->zoom);
        if (x < 0.0f)
            x = 0.0f;
        else if (x > 1.0f)
            x = 1.0f;
        if (y < 0.0f)
            y = 0.0f;
        else if (y > 1.0f)
            y = 1.0f;
        pick->uv_x[corner] = x;
        pick->uv_y[corner] = y;
        i32 left = pad->analog_l1 ? pad->analog_l1 : pad->analog_l2;
        i32 right = pad->analog_r1 ? pad->analog_r1 : pad->analog_r2;
        if (left) {
            pick->zoom -= left / 4192.0f;
            if (pick->zoom < 1.0f)
                pick->zoom = 1.0f;
            return 1;
        }
        if (right) {
            pick->zoom += right / 4192.0f;
            if (pick->zoom > 16.0f)
                pick->zoom = 16.0f;
            return 1;
        }
        if (pad->digital_buttons_pressed & 0x100) {
            pick->uv_x[corner] = static_cast<i32>(pick->uv_x[corner] * 256.0f + 0.5f) * (1.0f / 256.0f);
            pick->uv_y[corner] = static_cast<i32>(pick->uv_y[corner] * 256.0f + 0.5f) * (1.0f / 256.0f);
        }
        if (pad->digital_buttons_pressed & 0x80)
            pick->selected_corner = corner != 1;
        if ((pad->digital_buttons_pressed & 0x40) && pick->changed)
            pick->changed(menu, item, pad->digital_buttons);
        return 0;
    }
    static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2], uimtls[ui_bgmtl]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, item->text);

        i32 left = ((x - height + width - 2) << 4) + 16;
        i32 top = (y << 3) + 16;
        i32 right = left + ((height - 2) << 4) - 16;
        i32 bottom = top + ((height - 2) << 3) - 8;
        i32 middle_y = (top + bottom) >> 1;
        i32 quarter_x = (left + ((left + right) >> 1)) >> 1;
        i32 lower_y = (bottom + middle_y) >> 1;
        if (item->highlighted) {
            i32 points[8] = {left, middle_y, quarter_x, lower_y, quarter_x, bottom, right, top};
            if (!edui_donotdraw)
                NuRndrTriStrip2di(points, NULL, 4, item->colours[0], uimtls[0]);
        } else {
            i32 points[10] = {left, middle_y, quarter_x, lower_y, right, top, quarter_x, bottom, left, middle_y};
            if (!edui_donotdraw)
                NuRndrLineStrip2di(points, NULL, 5, item->colours[0], uimtls[0]);
        }
        return height;
    }
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, u32 first, u32 second, f32 start, f32 end) {
        i32 left = (x << 4) + static_cast<i32>(start * static_cast<f32>(width << 4));
        i32 band_width = static_cast<i32>(end * static_cast<f32>(width << 4)) -
                         static_cast<i32>(start * static_cast<f32>(width << 4));
        i32 strip_height = height;
        for (i32 strip = 0; strip < 8; ++strip) {
            i32 scale_top = strip > 1 ? strip - 1 : 0;
            i32 scale_bottom = strip;
            u32 top_left = 0x80000000u;
            u32 top_right = 0x80000000u;
            u32 bottom_left = 0x80000000u;
            u32 bottom_right = 0x80000000u;
            for (i32 channel = 0; channel < 3; ++channel) {
                i32 shift = channel * 8;
                top_left |= (((first >> shift) & 0xff) * scale_top / 8) << shift;
                top_right |= (((second >> shift) & 0xff) * scale_top / 8) << shift;
                bottom_left |= (((first >> shift) & 0xff) * scale_bottom / 8) << shift;
                bottom_right |= (((second >> shift) & 0xff) * scale_bottom / 8) << shift;
            }
            i32 colours[4] = {static_cast<i32>(top_left), static_cast<i32>(top_right), static_cast<i32>(bottom_left),
                              static_cast<i32>(bottom_right)};
            if (!edui_donotdraw)
                NuRndrGradRect2di(left, (y << 3) + strip * strip_height, band_width, strip_height, colours,
                                  uimtls[ui_bgmtl]);
        }
    }
    static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width) {
        (void)menu;
        edui_colour_pick_s *picker = static_cast<edui_colour_pick_s *>(item);
        item->x = x;
        item->y = y;
        i32 main_height = (width * 3) >> 2;
        static const u32 hues[7] = {0x800000ffu, 0x8000ffffu, 0x8000ff00u, 0x80ffff00u,
                                    0x80ff0000u, 0x80ff00ffu, 0x800000ffu};
        for (i32 band = 0; band < 6; ++band)
            eduiDrawHueValueBand(x, y, width, main_height, hues[band], hues[band + 1], static_cast<f32>(band) / 6.0f,
                                 static_cast<f32>(band + 1) / 6.0f);
        if (!edui_donotdraw) {
            i32 value_y = static_cast<i32>(y + main_height * picker->cursor_y) << 3;
            i32 hue_x = static_cast<i32>(x + width * picker->cursor_x) << 4;
            NuRndrLine2di(x << 4, value_y, (x + width - 1) << 4, value_y, 0x80ffffff, uimtls[0]);
            NuRndrLine2di(hue_x, y << 3, hue_x, ((y + main_height) << 3) - 8, 0x80ffffff, uimtls[0]);
        }
        i32 bar_height = (width + 7) >> 3;
        f32 full_red, full_green, full_blue;
        eduiHSVToRGB(picker->hue, 1.0f, picker->value, full_red, full_green, full_blue);
        i32 grey_byte = static_cast<i32>(picker->value * 255.0f);
        i32 grey = static_cast<i32>(0x80000000u | (grey_byte * 0x10101));
        i32 full = static_cast<i32>(0x80000000u | (static_cast<i32>(full_blue * 255.0f) << 16) |
                                    (static_cast<i32>(full_green * 255.0f) << 8) | static_cast<i32>(full_red * 255.0f));
        i32 saturation_colours[4] = {grey, full, grey, full};
        i32 saturation_y = y + main_height;
        if (!edui_donotdraw)
            NuRndrGradRect2di(x << 4, saturation_y << 3, width << 4, bar_height << 3, saturation_colours,
                              uimtls[ui_bgmtl]);
        i32 marker_x = static_cast<i32>(x + picker->saturation * (width - 2));
        if (!edui_donotdraw) {
            i32 marker_mid = saturation_y + (bar_height >> 1);
            for (i32 offset = -1; offset <= 1; ++offset) {
                NuRndrLine2di((marker_x + offset) << 4, saturation_y << 3, (marker_x + offset) << 4,
                              (marker_mid << 3) - 8, 0x80ffffff, uimtls[0]);
                NuRndrLine2di((marker_x + offset) << 4, marker_mid << 3, (marker_x + offset) << 4,
                              ((saturation_y + bar_height) << 3) - 8, 0x80000000, uimtls[0]);
            }
        }
        f32 red, green, blue;
        eduiHSVToRGB(picker->hue, picker->saturation, picker->value, red, green, blue);
        i32 selected = static_cast<i32>(0x80000000u | (static_cast<i32>(blue * 255.0f) << 16) |
                                        (static_cast<i32>(green * 255.0f) << 8) | static_cast<i32>(red * 255.0f));
        i32 selected_colours[4] = {selected, selected, selected, selected};
        if (!edui_donotdraw)
            NuRndrGradRect2di(x << 4, (saturation_y + bar_height) << 3, width << 4, bar_height << 3, selected_colours,
                              uimtls[ui_bgmtl]);
        return width;
    }
    static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_colour_slider_s *slider = static_cast<edui_colour_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[0]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        eduiFntPrintEx(edui_font, (x + 64) << 4, (y << 3) + baseline, 64, item->text);
        eduiFntPrintEx(edui_font, (x + 116) << 4, (y << 3) + baseline, 64, slider->format,
                       static_cast<i32>(slider->value));

        i32 colour = slider->red + (slider->green << 8) + (slider->blue << 16);
        i32 colours[4] = {static_cast<i32>(0xff000000), colour, static_cast<i32>(0xff000000), colour};
        i32 top = (y + height) << 3;
        if (!edui_donotdraw) {
            i32 left = static_cast<i32>(0.0f * (width << 4));
            i32 right = static_cast<i32>(1.0f * (width << 4));
            NuRndrGradRect2di((x << 4) + left, top, right - left, height << 3, colours, uimtls[ui_bgmtl]);
        }
        i32 bottom = (y + height * 2 - 1) << 3;
        if (!edui_donotdraw) {
            i32 marker = static_cast<i32>((x + 1) + slider->normalized_value * width) << 4;
            NuRndrLine2di(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        if (!edui_donotdraw) {
            i32 marker = static_cast<i32>(x + slider->normalized_value * width) << 4;
            NuRndrLine2di(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        if (!edui_donotdraw) {
            i32 marker = static_cast<i32>((x - 1) + slider->normalized_value * width) << 4;
            NuRndrLine2di(marker, top, marker, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        return height * 2;
    }
    static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_expander_s *expander = static_cast<edui_expander_s *>(item);
        const u32 button_colours[2] = {0xff000000, 0xffffffff};
        item->x = x;
        item->y = y;
        const bool over_button = edui_cursor_x >= expander->button_x && edui_cursor_y >= expander->button_y * 0.5f &&
                                 edui_cursor_x < expander->button_x + expander->button_size &&
                                 edui_cursor_y < (expander->button_y + expander->button_size) * 0.5f;
        expander->unknown_flags = (expander->unknown_flags & ~1u) | static_cast<u32>(over_button);

        if (!edui_donotdraw) {
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        const f32 row_height = NuQFntHeight(edui_font) * 1.25f * 0.125f;
        const i32 height = static_cast<i32>(row_height);
        const f32 baseline = (NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font)) * 0.125f;
        for (eduimenu_s *ancestor = menu; ancestor; ancestor = ancestor->parent) {
            if (ancestor == eduiGetActiveMenu()) {
                if (!edui_donotdraw)
                    NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                                  uimtls[ui_bgmtl]);
                break;
            }
        }
        const f32 button_size = row_height - 2.0f;
        for (i32 depth = 0; depth < expander->depth; ++depth) {
            if (!edui_donotdraw) {
                const i32 guide_x =
                    static_cast<i32>((static_cast<f32>(x) + (static_cast<f32>(depth) + 0.5f) * button_size) * 16.0f);
                NuRndrLine2di(guide_x, y << 3, guide_x, static_cast<i32>((static_cast<f32>(y) + row_height) * 8.0f),
                              0xff000000, uimtls[0]);
            }
        }
        const f32 button_x = static_cast<f32>(x) + static_cast<f32>(expander->depth) * button_size + 2.0f;
        const f32 button_y = static_cast<f32>(y) + 2.0f;
        expander->button_size = button_size;
        expander->button_x = button_x;
        expander->button_y = button_y;
        if (expander->first_child && !edui_donotdraw) {
            const u32 button_line_colour = button_colours[expander->unknown_flags & 1];
            NuRndrLineRect2di(static_cast<i32>(button_x * 16.0f), static_cast<i32>(button_y * 8.0f),
                              static_cast<i32>(button_size * 16.0f), static_cast<i32>(button_size * 8.0f), 0xff000000,
                              uimtls[ui_outmtl]);
            const f32 mid_x = button_x + button_size * 0.5f;
            const f32 mid_y = button_y + button_size * 0.5f;
            NuRndrLine2di(static_cast<i32>((button_x + 2.0f) * 16.0f), static_cast<i32>(mid_y * 8.0f),
                          static_cast<i32>((button_x + button_size - 2.0f) * 16.0f), static_cast<i32>(mid_y * 8.0f),
                          button_line_colour, uimtls[0]);
            if (!expander->open)
                NuRndrLine2di(static_cast<i32>(mid_x * 16.0f), static_cast<i32>((button_y + 2.0f) * 8.0f),
                              static_cast<i32>(mid_x * 16.0f), static_cast<i32>((button_y + button_size - 2.0f) * 8.0f),
                              button_line_colour, uimtls[0]);
        }
        const f32 text_x = button_x + button_size + 2.0f;
        eduiFntPrintClipEx(edui_font, text_x, static_cast<f32>(y) + baseline, 16, text_x,
                           static_cast<f32>(width) - button_size + 2.0f, item->text);
        return height;
    }
    static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width) {
        (void)menu;
        edui_file_pick_s *picker = static_cast<edui_file_pick_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        if (!picker->reserved_54) {
            if (!edui_donotdraw)
                NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                              uimtls[ui_bgmtl]);
            char formatted[264];
            sprintf(formatted, picker->format, picker->name);
            if (!edui_donotdraw) {
                NuQFntSet(edui_font);
                NuQFntSetColour(edui_font, item->colours[item->highlighted]);
            }
            eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, "%s %s", item->text, formatted);
            return height;
        }
        if (!edui_donotdraw) {
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        i32 centre = (x * 2 + width) << 3;
        eduiFntPrintEx(edui_font, centre, (y << 3) + baseline, 64, item->text);
        eduiFntPrintEx(edui_font, centre, ((y + height) << 3) + baseline, 64, picker->format, picker->name);
        return height * 2;
    }
    static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_gradient_pick_s *gradient = static_cast<edui_gradient_pick_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 2;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        if (gradient->change_timer && gradient->selected_stage) {
            eduiFntPrintEx(edui_font, (width + x * 2) << 3, (y << 3) + baseline, 64, "%1.03f",
                           gradient->selected_stage->time);
            --gradient->change_timer;
        } else
            eduiFntPrintEx(edui_font, (width + x * 2) << 3, (y << 3) + baseline, 64, item->text);

        i32 half_height = height >> 1;
        for (edui_gradient_node_s *stage = gradient->first_stage; stage && stage->next; stage = stage->next) {
            i32 colours[4] = {static_cast<i32>(stage->colour), static_cast<i32>(stage->next->colour),
                              static_cast<i32>(stage->colour), static_cast<i32>(stage->next->colour)};
            if (!edui_donotdraw) {
                i32 left = static_cast<i32>((width << 4) * stage->time);
                i32 right = static_cast<i32>((width << 4) * stage->next->time);
                NuRndrGradRect2di((x << 4) + left, (y + half_height) << 3, right - left, half_height << 3, colours,
                                  uimtls[ui_bgmtl]);
            }
        }
        for (edui_gradient_node_s *stage = gradient->first_stage; stage; stage = stage->next) {
            i32 marker_x = (x + static_cast<i32>(width * stage->time) - 2) << 4;
            i32 marker_y = ((y + half_height) << 3) - 16;
            i32 marker_height = (half_height << 3) + 16;
            if (!edui_donotdraw)
                NuRndrRect2di(marker_x, marker_y, 80, marker_height, stage->colour, uimtls[0]);
            u32 outline = gradient->selected_stage == stage ? 0x80ffffff : 0x80000000;
            if (!edui_donotdraw)
                NuRndrLineRect2di(marker_x, marker_y, 80, marker_height, outline, uimtls[ui_outmtl]);
        }
        return height;
    }
    static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width) {
        (void)menu;
        edui_graph_s *picker = static_cast<edui_graph_s *>(item);
        item->x = x;
        item->y = y;
        i32 graph_width = picker->width - 60;
        i32 graph_height = picker->height - 60;
        i32 plot_x = x + 30;
        i32 plot_y = y + 30;
        i32 plot_bottom = plot_y + graph_height;
        if (!edui_donotdraw) {
            NuRndrRect2di(x << 4, y << 3, picker->width << 4, picker->height << 3, 0x80000000, uimtls[0]);
            NuRndrRect2di(plot_x << 4, plot_y << 3, graph_width << 4, graph_height << 3, 0x80100000, uimtls[0]);
        }
        nugraph_s *graph = picker->graph;
        if (!graph) {
            if (!edui_donotdraw) {
                NuQFntSet(edui_font);
                NuQFntSetColour(edui_font, 0x80000080);
            }
            eduiFntPrintEx(edui_font, (x + picker->width) << 3, (y + picker->height) << 2, 64, "Invalid Graph Pointer");
            return picker->height;
        }
        if (!edui_donotdraw) {
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, 0x80808080);
        }
        if (picker->x_scale > 0.0f) {
            f32 tick_count = graph->x_extent * graph->x_scale / picker->x_scale;
            i32 label_interval = static_cast<i32>(tick_count / 10.0f) + 1;
            for (i32 tick = 1; tick <= static_cast<i32>(tick_count); ++tick) {
                i32 line_x = static_cast<i32>(plot_x + static_cast<f32>(graph_width * tick) / tick_count) << 4;
                u32 colour = 0x80260000;
                if (tick % label_interval == 0) {
                    eduiFntPrintEx(edui_font, line_x,
                                   (plot_bottom << 3) - 48 + static_cast<i32>(NuQFntHeight(edui_font)), 64, "%1.01f",
                                   static_cast<f32>(tick) * picker->x_scale);
                    colour = 0x80340000;
                }
                if (!edui_donotdraw)
                    NuRndrLine2di(line_x, plot_y << 3, line_x, (plot_bottom << 3) - 8, colour, uimtls[0]);
            }
        }
        if (picker->y_scale > 0.0f) {
            f32 tick_count = graph->y_extent * graph->y_scale / picker->y_scale;
            i32 label_interval = static_cast<i32>(tick_count / 10.0f) + 1;
            for (i32 tick = 1; tick <= static_cast<i32>(tick_count); ++tick) {
                i32 line_y = static_cast<i32>(plot_bottom - static_cast<f32>(graph_height * tick) / tick_count) << 3;
                u32 colour = 0x80260000;
                if (tick % label_interval == 0) {
                    eduiFntPrintEx(edui_font, (x + 28) << 4, line_y + (static_cast<i32>(NuQFntHeight(edui_font)) >> 1),
                                   32, "%1.01f", static_cast<f32>(tick) * picker->y_scale);
                    colour = 0x80340000;
                }
                if (!edui_donotdraw)
                    NuRndrLine2di(plot_x << 4, line_y, (plot_x + graph_width - 1) << 4, line_y, colour, uimtls[0]);
            }
        }
        i32 font_height = static_cast<i32>(NuQFntHeight(edui_font));
        eduiFntPrintEx(edui_font, (plot_x + graph_width) << 4, (y << 3) + font_height, 32, picker->x_label);
        eduiFntPrintEx(edui_font, (x + picker->width - 3) << 4, (y + picker->height) << 3, 32, picker->y_label);
        eduiFntPrintEx(edui_font, plot_x << 4, (y << 3) + font_height, 16, picker->title);
        eduiFntPrintEx(edui_font, (x + 28) << 4, (plot_bottom << 3) - 48 + font_height, 32, "0.0");
        u32 cursor_colour = width ? 0x80707070 : 0x80303030;
        if (!edui_donotdraw) {
            i32 cursor_y = static_cast<i32>(plot_y + (1.0f - picker->cursor_y) * graph_height) << 3;
            i32 cursor_x = static_cast<i32>(plot_x + picker->cursor_x * graph_width) << 4;
            NuRndrLine2di(plot_x << 4, cursor_y, (plot_x + graph_width - 1) << 4, cursor_y, cursor_colour, uimtls[0]);
            NuRndrLine2di(cursor_x, plot_y << 3, cursor_x, (plot_bottom << 3) - 8, cursor_colour, uimtls[0]);
        }
        picker->selected_point = -1;
        f32 closest = 1.0f;
        for (i32 point = 0; point < graph->point_count; ++point) {
            i32 px = plot_x + static_cast<i32>(graph->x[point] * graph_width);
            i32 py = plot_bottom - static_cast<i32>(graph->y[point] * graph_height);
            if (!edui_donotdraw) {
                NuRndrLine2di((px - 10) << 4, py << 3, (px + 10) << 4, py << 3, 0x80505050, uimtls[0]);
                NuRndrLine2di(px << 4, (py << 3) - 80, px << 4, (py << 3) + 80, 0x80505050, uimtls[0]);
            }
            f32 distance = fabsf(picker->cursor_x - graph->x[point]) + fabsf(picker->cursor_y - graph->y[point]);
            if (distance < closest && distance < 0.05f) {
                picker->selected_point = point;
                closest = distance;
            }
        }
        for (i32 onion = 0; onion < 8; ++onion)
            if (picker->onion_skins[onion])
                eduiRenderGraphLine(picker->onion_skins[onion], plot_x, plot_y, graph_width, graph_height, 0x80402000);
        eduiRenderGraphLine(graph, plot_x, plot_y, graph_width, graph_height, 0x80800000);
        if (picker->selected_point >= 0 && !edui_donotdraw) {
            i32 point = picker->selected_point;
            NuRndrLineRect2di((plot_x + static_cast<i32>(graph->x[point] * graph_width) - 6) << 4,
                              ((plot_bottom - static_cast<i32>(graph->y[point] * graph_height)) << 3) - 48, 192, 96,
                              0x80ffffff, uimtls[ui_outmtl]);
        }
        if (show_x_scale_help || show_y_scale_help) {
            if (!edui_donotdraw) {
                NuQFntSet(edui_font);
                NuQFntSetColour(edui_font, 0x80808080);
            }
            i32 help_x = (plot_x + graph_width / 2) << 4;
            i32 help_y = (plot_y << 3) + static_cast<i32>(NuQFntHeight(edui_font));
            eduiFntPrintEx(edui_font, help_x, help_y, 64, "Scaling %c Axis", show_x_scale_help ? 'X' : 'Y');
            eduiFntPrintEx(edui_font, help_x, help_y + static_cast<i32>(NuQFntHeight(edui_font)), 64, "%1.03f",
                           show_x_scale_help ? graph->x_scale : graph->y_scale);
        }
        i32 label_height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 label_y = (y + picker->height) << 3;
        i32 label_baseline = static_cast<i32>(NuQFntBaseline(edui_font)) +
                             ((label_height << 3) - static_cast<i32>(NuQFntHeight(edui_font))) / 2;
        if (!edui_donotdraw) {
            NuRndrRect2di(x << 4, label_y, picker->width << 4, label_height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        if (picker->selected_point >= 0) {
            i32 point = picker->selected_point;
            eduiFntPrintEx(edui_font, (x + 2) << 4, label_y + label_baseline, 16, "Point %d = %1.02f, %1.02f", point,
                           graph->x[point] * graph->x_scale * graph->x_extent,
                           graph->y[point] * graph->y_scale * graph->y_extent);
        }
        eduiFntPrintEx(edui_font, (x + picker->width - 4) << 4, label_y + label_baseline, 32,
                       "Cursor pos = %1.02f, %1.02f", picker->cursor_x * graph->x_scale * graph->x_extent,
                       picker->cursor_y * graph->y_scale * graph->y_extent);
        return picker->height + label_height;
    }
    static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_colour_pick_s *pick = static_cast<edui_colour_pick_s *>(item);
        i32 colours[4] = {static_cast<i32>(0x80000000), static_cast<i32>(0x80ffffff), static_cast<i32>(0x80000000),
                          static_cast<i32>(0x80ffffff)};
        i32 height = width / 4;
        item->x = x;
        item->y = y;
        if (!edui_donotdraw) {
            i32 left = static_cast<i32>(0.0f * (width << 4));
            i32 right = static_cast<i32>(1.0f * (width << 4));
            NuRndrGradRect2di((x << 4) + left, y << 3, right - left, height << 3, colours, uimtls[ui_bgmtl]);
        }
        i32 cursor_x = static_cast<i32>(x + 1 + (width - 2) * pick->value) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x + (width - 2) * pick->value) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x - 1 + (width - 2) * pick->value) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(cursor_x, y << 3, cursor_x, (y + (height >> 1) - 1) << 3, 0x80ffffff, uimtls[0]);
        cursor_x = static_cast<i32>(x + 1 + (width - 2) * pick->value) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        cursor_x = static_cast<i32>(x + (width - 2) * pick->value) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        cursor_x = static_cast<i32>(x - 1 + (width - 2) * pick->value) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(cursor_x, (y + (height >> 1)) << 3, cursor_x, (y + height - 1) << 3, 0x80000000, uimtls[0]);
        return height;
    }
    static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_slider_s *number = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, number->format);
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, format, number->value);
        return height;
    }
    static __used__ i32 eduicbRenderProp(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale) {
        edui_prop_s *property = static_cast<edui_prop_s *>(item);
        item->x = x;
        item->y = y;
        const bool over_button = edui_cursor_x >= property->button_x && edui_cursor_y >= property->button_y * 0.5f &&
                                 edui_cursor_x < property->button_x + property->button_size &&
                                 edui_cursor_y < (property->button_y + property->button_size) * 0.5f &&
                                 !eduiInteractLocked;
        const bool button_highlighted = (property->unknown_property_flags & 8) || over_button;
        property->unknown_property_flags = (property->unknown_property_flags & ~16u) | (button_highlighted ? 16u : 0u);
        if (!edui_donotdraw) {
            NuQFntSet(edui_font);
            const u32 text_colour = item->unknown_10 == 1   ? 0xff000060
                                    : item->unknown_10 == 2 ? 0xff006000
                                    : item->unknown_10 == 3 ? 0xff600000
                                                            : item->colours[item->highlighted];
            NuQFntSetColour(edui_font, text_colour);
        }
        const f32 row_height = NuQFntHeight(edui_font) * 1.25f * 0.125f;
        const i32 height = static_cast<i32>(row_height);
        const f32 baseline = (NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font)) * 0.125f;
        const f32 indentation = static_cast<f32>(property->depth) * (row_height - 2.0f);
        if (property->unknown_property_flags & 1) {
            const f32 text_width = NuQFntPrintLenU(edui_font, eduiPropTextEdit) * 0.0625f;
            f32 edit_width = static_cast<f32>(scale) - property->label_width - 4.0f;
            if (edit_width < text_width)
                edit_width = text_width;
            if (!edui_donotdraw) {
                NuRndrRect2di(x << 4, y << 3, scale << 4, static_cast<i32>(row_height * 8.0f),
                              item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
                NuRndrRect2di(static_cast<i32>((static_cast<f32>(x) + property->label_width + 2.0f) * 16.0f),
                              static_cast<i32>((static_cast<f32>(y) + 2.0f) * 8.0f),
                              static_cast<i32>(edit_width * 16.0f), static_cast<i32>((row_height - 4.0f) * 8.0f),
                              0xffffffff, uimtls[0]);
                for (i32 depth = 0; depth < property->depth; ++depth) {
                    const i32 guide_x = static_cast<i32>(
                        (static_cast<f32>(x) + (static_cast<f32>(depth) + 0.5f) * (row_height - 2.0f)) * 16.0f);
                    NuRndrLine2di(guide_x, y << 3, guide_x, static_cast<i32>((static_cast<f32>(y) + row_height) * 8.0f),
                                  0xff000000, uimtls[0]);
                }
                const i32 divider_x = static_cast<i32>((static_cast<f32>(x) + property->label_width) * 16.0f);
                NuRndrLine2di(divider_x, y << 3, divider_x, static_cast<i32>((static_cast<f32>(y) + row_height) * 8.0f),
                              (property->unknown_property_flags & 4) ? 0xffffffff : 0x0ff00000, uimtls[0]);
            }
            eduiFntPrintClipEx(edui_font, static_cast<f32>(x) + indentation, static_cast<f32>(y) + baseline, 16,
                               static_cast<f32>(x) - indentation, property->label_width, item->text);
            const f32 text_x = static_cast<f32>(x) + property->label_width + 2.0f;
            eduiFntPrintEx(edui_font, static_cast<i32>(text_x * 16.0f),
                           static_cast<i32>((static_cast<f32>(y) + baseline) * 8.0f), 16, eduiPropTextEdit);
            char prefix[256];
            NuStrNCpy(prefix, eduiPropTextEdit, eduiPropTextPos + 1);
            prefix[eduiPropTextPos + 1] = '\0';
            if (!edui_donotdraw) {
                const i32 caret_x = static_cast<i32>((text_x + NuQFntPrintLenU(edui_font, prefix) * 0.0625f) * 16.0f);
                NuRndrLine2di(caret_x, y << 3, caret_x, static_cast<i32>((static_cast<f32>(y) + row_height) * 8.0f),
                              0xff000000, uimtls[0]);
            }
            return height;
        }
        property->button_size = row_height - 2.0f;
        property->button_x = static_cast<f32>(x + scale) - (2.0f + property->button_size);
        property->button_y = static_cast<f32>(y) + 2.0f;
        if (!edui_donotdraw) {
            for (eduimenu_s *ancestor = menu; ancestor; ancestor = ancestor->parent) {
                if (ancestor == eduiGetActiveMenu()) {
                    NuRndrRect2di(x << 4, y << 3, scale << 4, static_cast<i32>(row_height * 8.0f),
                                  item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
                    break;
                }
            }
            const i32 divider_x = static_cast<i32>((static_cast<f32>(x) + property->label_width) * 16.0f);
            NuRndrLine2di(divider_x, y << 3, divider_x, static_cast<i32>((static_cast<f32>(y) + row_height) * 8.0f),
                          (property->unknown_property_flags & 4) ? 0xffffffff : 0x0ff00000, uimtls[0]);
            for (i32 depth = 0; depth < property->depth; ++depth) {
                const i32 guide_x = static_cast<i32>(
                    (static_cast<f32>(x) + (static_cast<f32>(depth) + 0.5f) * property->button_size) * 16.0f);
                NuRndrLine2di(guide_x, y << 3, guide_x, static_cast<i32>((static_cast<f32>(y) + row_height) * 8.0f),
                              0xff000000, uimtls[0]);
            }
        }
        eduiFntPrintClipEx(edui_font, static_cast<f32>(x) + indentation, static_cast<f32>(y) + baseline, 16,
                           static_cast<f32>(x) - indentation, property->label_width, item->text);
        const f32 value_x = property->button_x - 2.0f;
        const f32 value_clip_x = static_cast<f32>(x) + property->label_width;
        eduiFntPrintClipEx(edui_font, value_x, static_cast<f32>(y) + baseline, 32, value_clip_x, value_x - value_clip_x,
                           property->property_text);
        if (!edui_donotdraw) {
            const f32 button_x = property->button_x;
            const f32 button_y = property->button_y;
            const f32 button_size = property->button_size;
            if (property->button_type == 3) {
                u32 fill_colour;
                memcpy(&fill_colour, property->unknown_4e + 2, sizeof(fill_colour));
                NuRndrRect2di(static_cast<i32>(button_x * 16.0f), static_cast<i32>(button_y * 8.0f),
                              static_cast<i32>(button_size * 16.0f), static_cast<i32>(button_size * 8.0f), fill_colour,
                              uimtls[0]);
            }
            NuRndrLineRect2di(static_cast<i32>(button_x * 16.0f), static_cast<i32>(button_y * 8.0f),
                              static_cast<i32>(button_size * 16.0f), static_cast<i32>(button_size * 8.0f), 0xff000000,
                              uimtls[ui_outmtl]);
            const auto px = [&](f32 fraction) { return static_cast<i32>((button_x + button_size * fraction) * 16.0f); };
            const auto py = [&](f32 fraction) { return static_cast<i32>((button_y + button_size * fraction) * 8.0f); };
            const u32 icon_colour = button_highlighted ? 0xffffffff : 0xff000000;
            if (property->button_type == 1) {
                i32 points[6] = {px(0.1f), py(0.1f), px(0.9f), py(0.5f), px(0.1f), py(0.9f)};
                NuRndrTriStrip2di(points, NULL, 3, icon_colour, uimtls[0]);
            } else if (property->button_type == 2) {
                i32 up[6] = {px(0.1f), py(0.45f), px(0.5f), py(0.1f), px(0.9f), py(0.45f)};
                i32 down[6] = {px(0.1f), py(0.55f), px(0.5f), py(0.9f), px(0.9f), py(0.55f)};
                NuRndrTriStrip2di(up, NULL, 3, icon_colour, uimtls[0]);
                NuRndrTriStrip2di(down, NULL, 3, icon_colour, uimtls[0]);
            }
        }
        return height;
    }
    static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale) {
        return eduicbRenderProp(menu, item, x, y, scale);
    }
    static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 0.15625f);
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        if (item->text_alignment == 16)
            eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, item->text);
        else if (item->text_alignment == 32)
            eduiFntPrintEx(edui_font, (x + width) << 4, (y << 3) + baseline, 32, item->text);
        else
            eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, item->text_alignment, item->text);
        return height;
    }
    static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width) {
        (void)menu;
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw) {
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, item->text);
        f32 red = clipg;
        f32 green = clipg;
        f32 blue = clipg;
        if (item->type == 7)
            eduiHSVToRGB(cliph, clips, clipv, red, green, blue);
        u32 colour = 0x80000000u | (static_cast<i32>(blue * 255.0f) << 16) | (static_cast<i32>(green * 255.0f) << 8) |
                     static_cast<i32>(red * 255.0f);
        i32 colours[4] = {static_cast<i32>(colour), static_cast<i32>(colour), static_cast<i32>(colour),
                          static_cast<i32>(colour)};
        if (!edui_donotdraw)
            NuRndrGradRect2di((x + width - 45) << 4, y << 3, 40 << 4, height << 3, colours, uimtls[ui_bgmtl]);
        return height;
    }
    static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        NuQFntHeight(edui_font);
        NuQFntBaseline(edui_font);
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, 64, item->colours[2 + item->highlighted], uimtls[ui_bgmtl]);
        if (!edui_donotdraw)
            NuRndrLine2di((x + 4) << 4, (y << 3) + 32, ((x + width) << 4) - 64, (y << 3) + 32, 0xff000000,
                          uimtls[ui_bgmtl]);
        return 8;
    }
    static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, slider->format);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, format, slider->value);
        if (slider->normalized_value >= 0.0f && slider->normalized_value <= 1.0f) {
            i32 top = (y + height) << 3;
            i32 bottom = (y + height * 2 - 1) << 3;
            f32 span = width - 2;
            i32 marker_x = static_cast<i32>(x + 1 + slider->normalized_value * span) << 4;
            if (!edui_donotdraw)
                NuRndrLine2di(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
            marker_x = static_cast<i32>(x + slider->normalized_value * span) << 4;
            if (!edui_donotdraw)
                NuRndrLine2di(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
            marker_x = static_cast<i32>(x - 1 + slider->normalized_value * span) << 4;
            if (!edui_donotdraw)
                NuRndrLine2di(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        }
        return height * 2;
    }
    static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_slider_s *slider = static_cast<edui_slider_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted], uimtls[0]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, slider->format);
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (y << 3) + baseline, 64, format,
                       static_cast<i32>(slider->value));
        i32 top = (y + height) << 3;
        i32 bottom = (y + height * 2 - 1) << 3;
        f32 span = width;
        i32 marker_x = static_cast<i32>(x + 1 + slider->normalized_value * span) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        marker_x = static_cast<i32>(x + slider->normalized_value * span) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        marker_x = static_cast<i32>(x - 1 + slider->normalized_value * span) << 4;
        if (!edui_donotdraw)
            NuRndrLine2di(marker_x, top, marker_x, bottom, item->colours[item->highlighted], uimtls[0]);
        return height * 2;
    }
    static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        auto *picker = static_cast<edui_textpicker_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;

        if (!picker->editing) {
            if (!(picker->keyboard_flags & 1)) {
                if (!edui_donotdraw)
                    NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                                  uimtls[ui_bgmtl]);
                char formatted[256];
                sprintf(formatted, picker->format, picker->value);
                if (!edui_donotdraw) {
                    NuQFntSet(edui_font);
                    NuQFntSetColour(edui_font, item->colours[item->highlighted]);
                }
                eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, const_cast<char *>("%s %s"), item->text,
                               formatted);
                return height;
            }
            if (!edui_donotdraw) {
                NuRndrRect2di(x << 4, y << 3, width << 4, height << 4, item->colours[2 + item->highlighted],
                              uimtls[ui_bgmtl]);
                NuQFntSet(edui_font);
                NuQFntSetColour(edui_font, item->colours[item->highlighted]);
            }
            i32 centre = (x * 2 + width) << 3;
            eduiFntPrintEx(edui_font, centre, (y << 3) + baseline, 64, item->text);
            eduiFntPrintEx(edui_font, centre, ((y + height) << 3) + baseline, 64, picker->format, picker->value);
            return height * 2;
        }

        item_width = width > height * 10 ? width : height * 10;
        i32 total_height = (picker->keyboard_flags & 2) ? height * 6 : height * 5;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, item_width << 4, total_height << 3, item->colours[2], uimtls[ui_bgmtl]);
        for (i32 row = 0; row < 4; ++row) {
            for (i32 column = 0; column < 10; ++column) {
                if (!edui_donotdraw) {
                    NuQFntSet(edui_font);
                    NuQFntSetColour(edui_font,
                                    item->colours[row == picker->keyboard_row && column == picker->keyboard_column]);
                }
                char letter[2] = {textrow[row][column], '\0'};
                eduiFntPrintEx(edui_font, (x + height * column) << 4, ((y + row * height) << 3) + baseline, 16, letter);
            }
        }
        if (!edui_donotdraw) {
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[1]);
        }
        i32 value_y = ((y + height * 4) << 3) + baseline;
        eduiFntPrintEx(edui_font, x << 4, value_y, 16, picker->value);
        if (--cursor_flash < 1)
            cursor_flash = 32;
        if (cursor_flash >= 16 && !edui_donotdraw) {
            char prefix[256];
            i32 position = picker->cursor;
            if (position < 0)
                position = 0;
            if (position > 255)
                position = 255;
            memcpy(prefix, picker->value, position);
            prefix[position] = '\0';
            i32 offset = static_cast<i32>(NuQFntPrintLenU(edui_font, prefix));
            char at_cursor[2] = {position < NuStrLen(picker->value) ? picker->value[position] : 'A', '\0'};
            NuRndrRect2di((x << 4) + offset, value_y - static_cast<i32>(NuQFntBaseline(edui_font)),
                          static_cast<i32>(NuQFntPrintLenU(edui_font, at_cursor)),
                          static_cast<i32>(NuQFntHeight(edui_font)), item->colours[1] ^ 0xffffff, NULL);
        }
        if (picker->keyboard_flags & 2) {
            if (!edui_donotdraw) {
                NuQFntSet(edui_font);
                NuQFntSetColour(edui_font, item->colours[0]);
            }
            eduiFntPrintEx(edui_font, x << 4, ((y + height * 5) << 3) + baseline, 16,
                           TextPickCopyBuffer[0] ? TextPickCopyBuffer : const_cast<char *>("Copy L2+R2 Paste L1+R1"));
        }
        return total_height;
    }
    static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width) {
        edui_text_selector_s *selector = static_cast<edui_text_selector_s *>(item);
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        item->x = x;
        item->y = y;
        if (!edui_donotdraw)
            NuRndrRect2di(x << 4, y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
        if (!edui_donotdraw)
            NuQFntSet(edui_font);
        if (!edui_donotdraw)
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        char format[512];
        NuStrCpy(format, item->text);
        NuStrCat(format, ": %s");
        eduiFntPrintEx(edui_font, x << 4, (y << 3) + baseline, 16, format,
                       selector->options[static_cast<i32>(selector->value)]);
        return height;
    }
    static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width) {
        (void)menu;
        edui_texture_pick_s *picker = static_cast<edui_texture_pick_s *>(item);
        item->x = x;
        item->y = y;
        f32 size = static_cast<f32>(width);
        f32 uv_span = 1.0f / picker->zoom;
        f32 origin_u = picker->uv_x[picker->selected_corner] - 0.5f / picker->zoom;
        f32 origin_v = picker->uv_y[picker->selected_corner] - 0.5f / picker->zoom;
        f32 maximum = 1.0f - uv_span;
        if (origin_u < 0.0f)
            origin_u = maximum < 0.0f ? maximum : 0.0f;
        else if (maximum < origin_u)
            origin_u = maximum;
        if (origin_v < 0.0f)
            origin_v = maximum < 0.0f ? maximum : 0.0f;
        else if (maximum < origin_v)
            origin_v = maximum;
        f32 point_u[2] = {
            (picker->uv_x[0] - origin_u) / uv_span,
            (picker->uv_x[1] - origin_u) / uv_span,
        };
        f32 point_v[2] = {
            (picker->uv_y[0] - origin_v) / uv_span,
            (picker->uv_y[1] - origin_v) / uv_span,
        };
        for (i32 corner = 0; corner < 2; ++corner) {
            if (point_u[corner] < 0.0f)
                point_u[corner] = 0.0f;
            else if (point_u[corner] > 1.0f)
                point_u[corner] = 1.0f;
            if (point_v[corner] < 0.0f)
                point_v[corner] = 0.0f;
            else if (point_v[corner] > 1.0f)
                point_v[corner] = 1.0f;
        }
        u32 colours[2] = {0x807f7f7f, 0x807f7f7f};
        colours[picker->selected_corner] = 0x80ffffff;
        if (!edui_donotdraw) {
            NuRndrRectUV2di(x << 4, y << 3, width << 4, width << 3, origin_u, origin_v, origin_u + uv_span,
                            origin_v + uv_span, 0x80808080, picker->material);
            for (i32 corner = 0; corner < 2; ++corner) {
                i32 horizontal = static_cast<i32>(y + size * point_v[corner]) << 3;
                i32 vertical = static_cast<i32>(x + size * point_u[corner]) << 4;
                NuRndrLine2di(x << 4, horizontal, (x + width - 1) << 4, horizontal, colours[corner], uimtls[0]);
                NuRndrLine2di(vertical, y << 3, vertical, (y + width) * 8 - 8, colours[corner], uimtls[0]);
            }
        }
        i32 label_y = y + width;
        i32 height = static_cast<i32>(NuQFntHeight(edui_font) * 1.25f) >> 3;
        i32 baseline = static_cast<i32>(NuQFntHeight(edui_font) * 0.125f + NuQFntBaseline(edui_font));
        if (!edui_donotdraw) {
            NuRndrRect2di(x << 4, label_y << 3, width << 4, height << 3, item->colours[2 + item->highlighted],
                          uimtls[ui_bgmtl]);
            NuQFntSet(edui_font);
            NuQFntSetColour(edui_font, item->colours[item->highlighted]);
        }
        eduiFntPrintEx(edui_font, (x * 2 + width) << 3, (label_y << 3) + baseline, 64, "%1.03f,%1.03f",
                       picker->uv_x[picker->selected_corner] * 256.0f, picker->uv_y[picker->selected_corner] * 256.0f);
        return width + height;
    }
    static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value) {
        if (!dir_list || !filepick_item)
            return;
        if (item->data == 9999) {
            char *separator = NuStrRChr(filepick_item->filename, '/');
            if (separator)
                *separator = '\0';
            else
                filepick_item->filename[0] = '\0';
            filepick_item->reopen_directory = 1;
        } else if (item->data >= 0 && item->data < dir_size) {
            auto *entries = static_cast<FilePickDirectoryEntry *>(dir_list);
            FilePickDirectoryEntry &entry = entries[item->data];
            if (entry.flags & 8) {
                if (filepick_item->filename[0])
                    NuStrCat(filepick_item->filename, const_cast<char *>("/"));
                NuStrCat(filepick_item->filename, entry.name);
                filepick_item->reopen_directory = 1;
            } else {
                NuStrCpy(filepick_item->name, entry.name);
                if (filepick_item->changed)
                    filepick_item->changed(filepick_menu, filepick_item, value);
            }
        }
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }
    static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32) {
        eduiMenuDetach(menu);
        eduiMenuDestroy(menu);
    }
}
