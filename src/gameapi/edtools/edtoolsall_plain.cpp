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

// Retail keeps an unreferenced partial-inlining clone of the old HSV2RGB
// helper (the non-grey path) in this unit.
#if defined(__i386__)
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone, regparm(2), sseregparm))
#else
#define HSV2RGB_BODY_ATTR __attribute__((used, noinline, noclone))
#endif
static HSV2RGB_BODY_ATTR void
HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue) __asm__("_ZL7HSV2RGBfffPfS_S_.part.18");
static void HSV2RGBBody(f32 hue, f32 saturation, f32 value, f32 *red, f32 *green, f32 *blue);

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



#include "gameapi/edtools/edptlall.cpp"


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
static __used__ __attribute__((noclone)) void eduiFntPrintExGeneric(void *font, int x, int y,
                                                                                int alignment, char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz");
static __used__ void eduiFntPrintExGeneric(void *font, int x, int y, int alignment, char *format, ...);

// Left-aligned (16) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExLeft(void *font, int x, int y, int alignment,
                                                                  char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.67");

// Centred (64) specialisation of the integer print helper.
static __attribute__((noclone)) void eduiFntPrintExCentre(void *font, int x, int y, int alignment,
                                                                    char *format, ...)
    __asm__("_ZL14eduiFntPrintExPviiiPcz.constprop.68");

#if defined(__clang__)
// Clang has no __builtin_va_arg_pack; format here and print the result. The
// generic helper already owns this overload's mangled name, so rename it.
static void eduiFntPrintEx(void *font, int x, int y, int alignment, char *format, ...)
    __asm__("eduiFntPrintExFormatted");
#else
static inline __attribute__((always_inline)) void eduiFntPrintEx(void *font, int x, int y, int alignment,
                                                                 char *format, ...) {
    if (__builtin_constant_p(alignment) && alignment == 16)
        eduiFntPrintExLeft(font, x, y, alignment, format, __builtin_va_arg_pack());
    else if (__builtin_constant_p(alignment) && alignment == 64)
        eduiFntPrintExCentre(font, x, y, alignment, format, __builtin_va_arg_pack());
    else
        eduiFntPrintExGeneric(font, x, y, alignment, format, __builtin_va_arg_pack());
}
#endif


// Only caller prints the property edit buffer left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintExEditBuffer(void *font, float x, float y, int alignment,
                                                                        char *format, ...)
    __asm__("_ZL14eduiFntPrintExPvffiPcz.constprop.66");

static __used__ void eduiFntPrintClipEx(void *font, float x, float y, int alignment, float clip_x, float clip_width,
                                        char *format, ...);
// Only eduiMenuRender calls the integer clip helper, always left-aligned.
static __attribute__((noinline, noclone)) void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32 alignment, i32 clip_x,
                                                                  i32 clip_width, char *format, ...)
    __asm__("_ZL18eduiFntPrintClipExPviiiiiPcz.constprop.70");
static void eduiFntPrintClipEx(void *font, i32 x, i32 y, i32, i32 clip_x, i32 clip_width, char *format, ...);

extern "C" i32 nugraphAddPoint(nugraph_s *graph, f32 x, f32 y);
extern "C" i32 nugraphDeletePoint(nugraph_s *graph, i32 index);
extern "C" i32 nugraphCalcCurve(nugraph_s *graph, i32 point_count);
extern "C" f32 nugraphGetYatX(nugraph_s *graph, f32 x, i32 mode);

#include "gameapi/edtools/edtoolsall_plain_types.h"

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
    static i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
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
    // Retail only has IPA-SRA clones of these two helpers.
    static __attribute__((noinline, noclone)) void
    eduicbAttachDirectoryList(eduimenu_s *, edui_file_pick_s *) __asm__("eduicbAttachDirectoryList.isra.32");
    static void eduicbSelectDirectoryEntry(eduimenu_s *, eduiitem_s *, u32);
    static void eduicbSelectDirectoryExit(eduimenu_s *, eduiitem_s *, u32);
    static i32 eduicbProcessTextPick(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static i32 eduicbProcessProp(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
    static __attribute__((noinline, noclone)) i32
    eduicbProcessPropKeyboard(eduimenu_s *, edui_prop_s *) __asm__("eduicbProcessPropKeyboard.isra.27");
    static i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSelWithClipColour(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGraph(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTexturePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderExpander(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilter(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderFilePick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
    static i32 eduicbRenderProp(eduimenu_s *, eduiitem_s *, i32, i32, i32, i32);
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
extern i32 bCameraEnabled;
extern i32 edui_filepick_listsize;
extern u32 eduiitem_context_outline_colour;
extern u32 eduiitem_selected_outline_colour;
extern u32 eduimenu_selected_outline_colour;
extern u32 eduimenu_outline_colour;
extern f32 edmain_default_camera_speed;
extern i32 ed_main_menu_h;
extern i32 ed_main_menu_w;
extern i32 ed_main_menu_y;
extern i32 ed_main_menu_x;
extern eduiiattr_s ed_attr;
extern i32 eduiPropTextPos;
extern i32 edmain_render_interacts;
extern ed_module_s edbridesc;
extern ed_module_s edanimdesc;
extern ed_module_s edptldesc;
extern ed_module_s edgradesc;
extern i32 edpp_create_type;
    f32 edpp_copy_size = 0.2f;
extern f32 edpp_scale_factor;
extern i32 edptl_repeatboxxzlock;
extern i32 edptl_clipboard_entry;

extern eduimenu_s * edui_messagemenu;
extern u32 edblack[4];
extern u32 edgrey[4];
extern u32 eddarkred[4];
extern NUMTL * uimtls[5];
extern i32 ui_bgmtl;
extern i32 ui_outmtl;

extern eduimenu_s * edgra_active_menu;
extern eduimenu_s * edgra_options_menu;
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
extern struct numtl_s * edgra_mtl;
extern NUMTL * edgra_mtl_zoff;
    NUVEC edgra_cam_pos;
    f32 edgra_cam_dist;
    i32 edgra_cam_ax;
    i32 edgra_cam_ay;
extern f32 edgra_size;
extern i32 edgra_nearest;
extern i32 edgra_nearest_instance;
extern i32 edgra_instance_type;
    i32 edgra_fadeunits_used;
extern i32 edgra_clumps_used;
    i32 edgra_fadeclumps_used;
extern i32 edgra_clump_size;
    i32 edgra_pageid;
    i32 edgra_rotz;
    i32 edgra_roty;
    i32 edgra_dpadmode;
extern i32 edgra_editormode;
extern void * edgra_free_vecbuffer;
extern NUMTX * edgra_mtxbuffer;
extern NUVEC * edgra_vecbuffer;
extern NUGSCN * edgra_page_scene[8];
extern void * edgra_page_terrain[8];
extern NUMTX * edgra_page_matrix_stack[8];
extern i32 EDGRA_MAX_CLUMPS;
extern i32 EDGRA_MAX_INDIVIDUAL_CLUMPS;
extern i32 EDGRA_MAX_UNITS_PER_INDIVIDUAL_CLUMP;
extern edgra_clump_s * GrassClumps;
extern edgra_individual_s * IndGrassClumps;
extern i32 * IndGrassClumpsUsed;
extern i32 edgra_ind_clumps_used;

extern i32 edpp_page_used[8];
extern i32 edpp_page_on[8];
extern usize edpp_page_scene[8];
extern i32 edpp_types_used;
    i32 edpp_copy_source[8];
extern i32 edpp_usememcard;
extern edpp_particle_s edpp_ptls[512];
    NUVEC edpp_cam_pos;
    f32 edpp_cam_dist;
    i32 edpp_cam_ax;
    i32 edpp_cam_ay;
extern i32 edpp_first_time_this_level;
    i32 edpp_numparticles;
extern NUMTL * edpp_mtl;
extern NUMTL * edpp_boxmtl;
extern i32 edpp_nearest;
extern i32 edpp_curr;
extern i32 edpp_snap_enabled;
extern u8 edpp_effect_list;
extern i32 edpp_instances_used;
    i32 edpp_copy_mode;
    i32 edpp_copy_enclosed;
    i32 edpp_copy_source_count;
    NUVEC edpp_copy_source_vec;
extern i32 edpp_num_orphans;
extern i32 edpp_showAllPlaced;
extern i32 edpp_dpad_mode;
extern i32 edpp_readout;
extern eduimenu_s * edpp_active_menu;
extern eduimenu_s * ptltypemenu;
extern eduimenu_s * ptloptmenu;
extern eduimenu_s * ptlgsortmenu;
extern eduimenu_s * ptlgcodemenu;
extern eduimenu_s * ptlemitmenu;
extern eduimenu_s * ptlsizemenu;
extern eduimenu_s * ptlrotmenu;
extern eduimenu_s * ptljibmenu;
extern eduimenu_s * ptlcolmenu;
extern eduimenu_s * ptlemitvelmenu;
extern eduimenu_s * ptlgravmenu;
extern eduimenu_s * ptlvaremitmenu;
extern eduimenu_s * ptlvarstartmenu;
extern eduimenu_s * ptlstartvelmenu;
extern eduimenu_s * ptlcutoffmenu;
extern eduimenu_s * ptldatamenu;
extern eduimenu_s * ptlreadoutmenu;
    eduimenu_s *memcardmenu;
extern eduimenu_s * messagemenu;
    eduimenu_s *loadeffectsmenu;
    eduimenu_s *mergeeffectsmenu;
    eduimenu_s *deleteeffectsmenu;
    eduimenu_s *filenamemenu;
extern eduimenu_s * namemenu;
    eduimenu_s *confirmmenu;
extern eduimenu_s * changegenratemenu;
    eduimenu_s *totalptlsmenu;
extern eduimenu_s * etimemenu;
extern eduimenu_s * emittimemenu;
extern eduimenu_s * sscalemenu;
extern eduimenu_s * texturemenu;
extern eduimenu_s * textureselectmenu;
extern eduimenu_s * collmenu;
extern eduimenu_s * effectlistmenu;
extern eduimenu_s * ptlclipmenu;
extern eduimenu_s * dpadmodemenu;
extern eduimenu_s * edptl_switch_menu;
extern eduimenu_s * edptl_switchtype_menu;
extern eduimenu_s * edptl_sounds_menu;
extern eduimenu_s * edptl_soundx_menu;
extern eduimenu_s * edptl_soundid_menu;
extern eduimenu_s * edptl_soundcontrol_menu;
extern eduimenu_s * edptl_bounce_menu;
extern eduimenu_s * edptl_group_menu;
extern eduimenu_s * edptl_ghost_menu;
extern eduimenu_s * edptl_star_menu;
extern eduimenu_s * edptl_page_menu;
extern eduimenu_s * edptl_drawflag_menu;
extern eduimenu_s * edptl_scaleeffect_menu;
extern eduimenu_s * edptl_orphanlist_menu;
extern eduimenu_s * edptl_instancesettings_menu;
extern eduimenu_s * edptl_torus_menu;
extern eduimenu_s * edptl_damage_menu;
extern eduimenu_s * edptl_damageflag_menu;
extern eduimenu_s * edptl_repeatbox_menu;
extern eduimenu_s * edptl_quickdel_menu;
extern eduimenu_s * edptl_detail_menu;
extern eduimenu_s * edptl_testdetail_menu;
extern i32 edpp_emitrotz;
extern i32 edpp_emitroty;
extern i32 edpp_emitrotx;
    i32 edpp_rotz;
    i32 edpp_roty;
    f32 edpp_offset;
    i32 edpp_refrotz;
    i32 edpp_refroty;
    i32 edpp_copyrotz;
    i32 edpp_copyroty;
extern i32 edpp_facrotx;
extern i32 edpp_facroty;

    void edgraInitAllClumps(void);
}

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

void edbriDoInput(nupad_s *);
void edbriDetermineNearest(f32);
void edbriDrawCursor();
extern "C" {
    extern NUMTL *edbri_mtl, *edbri_mtl_zoff;
    extern eduimenu_s *edbri_options_menu, *edbri_active_menu;
    extern i32 edbri_nearest, edbri_plank_instance_type, edbri_post_instance_type;
    void edbriBridgesReset(void);
}

static void edbriInit();

static void edbriClose();

static i32 edbriProc(f32 delta_time, nupad_s *pad);

static void edbriRender();

static void edanimInit();

static void edanimClose();

static i32 edanimProc(f32 delta_time, nupad_s *pad);

static void edanimRender();

void edgraClumpReseed(i32 index);

edgra_individual_s *GetIndGrassClump(i32 clump, i32 element);

void edgraClumpDestroy(i32 index);
void edbitsDoSingleDump(i32 face);
void edbriBridgeUpdate(i32 bridge, NUGSCN *scene);


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
    part_emit_s part_emits[40];
    i32 part_page_on[8];
    i32 part_page_used[8];
    i32 edpart_instances_used;
extern edanim_param_s AnimParams[64];
extern eduimenu_s * edanim_active_menu;
extern eduimenu_s * edanim_options_menu;
extern eduimenu_s * edanim_particle_menu;
extern eduimenu_s * edanim_particletype_menu;
extern eduimenu_s * edanim_localparticletype_menu;
extern eduimenu_s * edanim_localparticle_menu;
extern eduimenu_s * edanim_switch_menu;
extern eduimenu_s * edanim_switchtype_menu;
extern eduimenu_s * edanim_sound_menu;
extern eduimenu_s * edanim_soundtype_menu;
extern eduimenu_s * edanim_localsoundtype_menu;
extern eduimenu_s * edanim_localsound_menu;
extern eduimenu_s * edanim_bouncy_menu;
extern eduimenu_s * edanim_mctb_menu;
extern NUGSCN * edanim_page_scene[8];
extern i32 edanim_page_used[8];
extern i32 edanim_page_on[8];
extern NUMTL * edanim_mtl;
extern NUMTL * edanim_mtl_zoff;
    NUVEC edanim_cam_pos;
    f32 edanim_cam_dist;
    i32 edanim_cam_ax;
    i32 edanim_cam_ay;
extern i32 edanim_nearest;
extern i32 edanim_nearest_param_id;
extern i32 edanim_nearest_particle;
extern i32 edanim_nearest_sound;
extern i32 edanim_next_param;
extern i32 edanim_particle_mode;
extern i32 edanim_particle_type;
extern i32 edanim_sound_mode;
extern i32 edanim_sound_type;
extern i32 edanim_params_used;
    i32 edanim_emitrotz;
    i32 edanim_emitroty;
}

extern u8 object_switches[128];

extern "C" {
extern u8 edbits_what_game;
extern char edbits_level_filename[256];
extern char edbits_general_save_directory[256];
extern char edbits_general_save_name[256];
extern char edbits_general_save_extension[256];
extern char edbits_level_save_directory[256];
extern char edbits_level_save_name[256];
extern char edbits_level_save_extension[256];
extern char edbits_datapath[256];
extern i32 edSfxAllCount;
extern NUGSCN * edbits_base_scene;
extern NUGSCN * edbits_things_scene;
extern void * edbits_base_terrain;
extern i32 edbits_particle_general_page;
extern i32 edbits_anim_page;
extern i32 edbits_editmode;
    i32 edbits_part_general_page;
extern NUCAMERA * cubemapcam;
extern i32 edbits_types_error;
    i32 edbits_duplicates_error;
extern EDBITS_GAME_SOUND edbitsGameSound[320];
extern i32 edbits_numsounds;
extern EDBITSPLAYSOUNDCALLBACK edbitsPlaySound;
extern EDBITSREQUESTSOUNDCALLBACK edbitsRequestSound;
extern i32 edbits_override_backups;
extern eduimenu_s * edbri_active_menu;
extern eduimenu_s * edbri_options_menu;
    eduimenu_s *edbri_dpadmode_menu;
    eduimenu_s *edbri_plankinstance_menu;
    eduimenu_s *edbri_postinstance_menu;
    eduimenu_s *edbri_plankcount_menu;
    eduimenu_s *edbri_bridgeproperties_menu;
    eduimenu_s *edbri_ropecolour_menu;
extern NUMTL * edbri_mtl;
extern NUMTL * edbri_mtl_zoff;
    NUVEC edbri_cam_pos;
    f32 edbri_cam_dist;
    i32 edbri_cam_ax;
    i32 edbri_cam_ay;
extern i32 edbri_nearest;
extern i32 edbri_plank_instance_type;
extern i32 edbri_post_instance_type;
extern i32 edbri_bridges_used;
extern i32 edbri_page_used[8];
extern i32 edbri_page_on[8];
extern i32 edbri_pageid;
extern i32 edbri_rotz;
extern i32 edbri_roty;
    i32 edbri_mode;
extern NUGSCN * edbri_page_scene[8];
extern edbridge_s edBridges[64];
extern f32 edbri_length;
extern f32 edbri_width;
extern i32 edbri_planks;
extern i32 edbri_post_interval;
}

static void edbriEnter();

static void edanimEnter();

void FileLoadSingleEffectType(debinftype *, i32, char);
extern "C" void NuBridgeInit(void);
extern "C" void NuBridgeRemove(i32 bridge);
extern "C" i32 NuBridgeCreate(NUGSCN *, nuhspecial_s *, nuhspecial_s *, NUVEC *, NUVEC *, f32, i16, f32, f32, f32, f32,
                              i32, f32, f32, i32, u32);

void edbriBridgeUpdate(i32 index, NUGSCN *scene);

void edbriBridgePlace(i32 index, NUVEC *position);

i32 edbriBridgeCreate(NUVEC *position);

void edbriBridgeDestroy(i32 index);

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


void edcamSetContoller(i32 invert_pitch);



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
extern i32 PadFlyMode;
extern NUMTX * ed_remap_mtx;
extern i32 edmain_cursor_enabled;

eduimenu_s *edGetMainMenu(void);
void edGraDisableTerrainSwap(void);
void edGraEnableTerrainSwap(void);
void edGraInitTerrainSwapProtection(void);
i32 edanimLoadPage(char *path, NUGSCN *scene);

void edanimClearPage(i32 page);
i32 edanimLookupSpecial(char *name, NUGSCN *scene);
void edanimParamReset(void);
void edanimParticleDestroy(i32 parameter_index, i32 particle_index);
void edanimRegisterCubeDumpInfo(void);
void edanimStartPage(i32 page);
void edanimStopPage(i32 page);
void edanimUpdateObjects(float elapsed);
void edbitsDrawBBox(NUVEC *minimum, NUVEC *maximum, i32 colour, numtl_s *material);
    void edbitsDrawOvalXY(NUVEC *, f32, f32, i32, NUMTL *);
void edbitsDrawCircleTilted(NUVEC *centre, f32 radius, i32 colour, NUMTL *material, i32 rotation_z,
                                i32 rotation_y);
void edbitsDrawCircleXY(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
void edbitsDrawOvalXY(NUVEC *centre, f32 radius_x, f32 radius_z, i32 colour, NUMTL *material);
void edbitsDrawSphere(NUVEC *centre, f32 radius, i32 colour, NUMTL *material);
char *edbitsGetSoundName(i32);
i32 edbitsLookupInstance(char *name, NUGSCN *scene);
i32 edbitsLookupSound(char *name);
i32 edbitsLookupSoundFX(char *);
i32 edbitsProcessCubemapDump(void);
void edbitsRegisterDataPath(char *path);
void edbitsRegisterBaseScene(NUGSCN *scene);
extern i32 * edbits_editor_enabled;

void edbitsRegisterEditMode(i32 mode);
void edbitsRegisterEditorEnabledFlag(i32 *enabled);
void edbitsRegisterLevel(char *filename, i32 game);
void edbitsRegisterPlaySound(EDBITSPLAYSOUNDCALLBACK callback);
void edbitsRegisterRequestSound(EDBITSREQUESTSOUNDCALLBACK callback);
void edbitsRegisterSaveFormat(char *general_save_directory, char *general_save_name, char *general_save_extension,
                                  char *level_save_directory, char *level_save_name, char *level_save_extension);
void edbitsRegisterSoundEffect(char *name, i32 id);
void edbitsRegisterThingsScene(NUGSCN *scene);
void edbitsRegisterBaseTerrain(void *terrain);
extern i32 edbitsSfxVol;
void edbitsSetSoundFxVolume(i32 volume);
i32 edbitsStartCubemapDump(void);
void edbitsVector2YZRot(i16 *rotation_y, i16 *rotation_z, f32 x, f32 y, f32 z);
void edbriBridgesReset(void);
    void edbriStopPage(i8 page);
    void NuBridgeRemove(i32 bridge);
void edbriClearPage(i8 page);
void edbriStartPage(i32 page_number);
void edbriStopPage(i8 page);
f32 edcamGetDist(void);
edcam_s *edcamGetEdCam(void);
void edcamGetOffset(NUVEC *offset);
void edcamGetPosAng(NUVEC *position, i32 *pitch, i32 *yaw);
void edcamGetPosAngSnap(NUVEC *position, i32 *pitch, i32 *yaw);
NUVEC *edcamGetPosPointer(void);
void edcamMove(nupad_s *pad);
void edcamMoveEx(nupad_s *pad, f32 delta_time);
void edcamMtx(NUMTX *matrix);
void edcamSet(void);
void edcamSetAdjustFreedom(bool position_x, bool position_y, bool position_z, bool pitch, bool yaw, bool distance);
void edcamSetAng(i32 pitch, i32 yaw);
void edcamSetAutoSpeed(f32 move_base, f32 move_distance_scale, f32 zoom_base, f32 zoom_distance_scale);
void edcamSetDist(f32 distance);
void edcamSetMouseSensitivity(f32 pitch, f32 yaw, f32 movement);
void edcamSetOffset(NUVEC *offset);
void edcamSetPos(NUVEC *position);
void edcamSetPosAng(NUVEC *position, i32 pitch, i32 yaw);
void edcamSetSpeed(f32 position_x, f32 position_y, f32 position_z, f32 distance);
void edcamSetSpeedPos(f32 position_x, f32 position_y, f32 position_z);
i32 edgraBufferUsage(void);
i32 edgraLoadPage(char *path, void *gscn, i32 terrain, void *buf, void *buf_end);
void edgraClearPage(i8 page);
void edgraClumpsReset(void);
void edgraSetMemoryBuffer(VARIPTR start, VARIPTR end);
void edgraSetThinning(i32 clumps, i32 elements);
    void NuWindSetup(VARIPTR *, VARIPTR, i32, i32);
void edgraSetup(VARIPTR *buffer, VARIPTR end, i32 clumps, i32 individual_clumps, i32 units_per_clump);
i32 edmainActivate(ed_module_s *module, i32 notify);
void edmainClose(void);
ed_module_s *edmainCurrent(void);
void edmainExtCamera(NUCAMERA *camera);
NUCAMERA *edmainGetCamera(void);
i32 edmainGetCursorEnabled(void);
    // Shared editor font; edmainInit receives and stores the font handle.
extern void * ed_fnt;

void edmainInit(void *font, char *configuration_file);
void edmainInitEx(void *font, char *configuration_file, eduiiattr_s *colours, i32 x, i32 y, i32 width, i32 height);
i32 edmainProcess(f32 delta_time, nupad_s *pad);
NUVEC *edmainQueryLocVec(void);
i32 edmainRegister(ed_module_s *module);
void edmainRegisterLocVec(NUVEC *position);
void edmainRender(void);
void edmainSetCamera(NUMTX *matrix);
void edmainSetCursorEnabled(i32 enabled);
f32 edmainSetMainMenuScale(f32 scale);
void edmainSetReturn(i32 result);
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
void edppClearPage(i8 page);
void edppDeleteEffect(i32 index);
void edppDestroyAllEffects(void);
void edppDestroyAllParticles(void);
    extern f32 globaltime;

void edppDrawSpheres(debinftype *effect, i32 key_index);
void edppDrawTorus(debinftype *effect, i32 key_index);
i32 edppFindAllSounds(i32 page, NUVEC *positions, i32 (*sounds)[4], i32 capacity, i32 skip);
i32 edppLoadPage(char *path, i32 flag, usize scene);
void edppRegisterPointerToGameCharLocation(NUVEC *position);
void edppRestartAllEffectsInLevel(void);
void edppSetSaveName(u32 page, char *name);
void edppStopPage(i32 page_id);
extern i32 edqseed;
void edqrand(void);
edui_textpicker_s *eduiAddTextPickEntEx(eduimenu_s *menu, eduiitem_s *source, EdUiItemCallback callback);
void eduiAddPropTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
void eduiAddTextPickEnt(eduimenu_s *menu, eduiitem_s *source);
i32 eduiClearActiveMenu(void);
i32 eduiCheckForPadMenuCancel(eduimenu_s *menu, nupad_s *pad);
void eduiCreate3LineMessageMenu(eduimenu_s *parent, char *first, char *second, char *third, i32 first_highlight,
                                    i32 second_highlight, i32 third_highlight);
void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
i32 eduiCursorOverMenu(eduimenu_s *menu);
i32 cbInteractMenuTitle(edui_interact_s *interact);
i32 cbInteractMenuScrollUp(edui_interact_s *interact);
i32 cbInteractMenuScrollDown(edui_interact_s *interact);
void cbInteractMenuScrollTo(eduimenu_s *menu, char *text);
void cbInteractMenuKeySelect(eduimenu_s *menu);
void eduiFlushInteracts(void);
eduimenu_s *eduiGetActiveMenu(void);
eduimenu_s *eduiGetActiveMenuParent(void);
f32 eduiGetAnalougePadValue(nupad_s *pad);
i32 eduiGetCameraEnabled(void);
void eduiGetCursorCoords(f32 *x, f32 *y);
void eduiGetCursorDelta(f32 *x, f32 *y);
eduimenu_s *eduiGetTopLevelParent(eduimenu_s *menu);
extern i32 bUsingMenuFocus;
i32 eduiGetUsingMenuFocus(void);
i32 eduiGradPickRead(eduiitem_s *item, edui_gradient_stage_s *stages, i32 capacity);
edui_gradient_node_s *eduiGradStageAdd(edui_gradient_pick_s *item, f32 time, f32 hue, f32 saturation, f32 value);
edui_gradient_node_s *eduiGradStageAddRGB(edui_gradient_pick_s *item, f32 time, f32 red, f32 green, f32 blue);
void eduiGradStageDelete(edui_gradient_pick_s *item, edui_gradient_node_s *stage);
void eduiGradStageSetHSV(edui_gradient_node_s *stage, f32 hue, f32 saturation, f32 value);
void eduiGradStageSetRGB(edui_gradient_node_s *stage, f32 red, f32 green, f32 blue);
void eduiIitemExpanderSetDepth(edui_expander_s *item, i32 depth);
void eduiInit(void);
void eduiInitMaterials(void);
eduiitem_s *eduiItemCheckCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                    char *text);
eduiitem_s *eduiItemColourPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemColourPickSetHSV(edui_colour_pick_s *item, f32 hue, f32 saturation, f32 value);
void eduiItemColourPickSetRGB(edui_colour_pick_s *item, f32 red, f32 green, f32 blue);
eduiitem_s *eduiItemColourSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           f32 minimum, f32 range, f32 value, u8 red, u8 green, u8 blue, char *text);
eduiitem_s *eduiItemDataGradPickCreate(usize data, const void *colours, EdUiItemCallback callback,
                                           EdUiItemCallback press, EdUiItemCallback add, EdUiItemCallback remove,
                                           EdUiItemCallback copy, EdUiItemCallback paste, char *text);
void eduiItemExpanderAddChild(edui_expander_s *item, eduiitem_s *child);
eduiitem_s *eduiItemExpanderCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemFilePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemFilePickSetFmt(edui_file_pick_s *item, char *format);
void eduiItemFilterAddItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemFilterCreate(usize data, const void *colours, char *text, char *value);
void eduiItemFilterRemoveItem(edui_filter_s *item, eduiitem_s *child);
eduiitem_s *eduiItemGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemGraphAddOnionSkin(edui_graph_s *item, nugraph_s *graph);
eduiitem_s *eduiItemGraphCreate(usize data, const void *colours, EdUiItemCallback callback, nugraph_s *graph,
                                    i32 width, i32 height);
void eduiItemGraphSetCursor(edui_graph_s *item, f32 x, f32 y);
void eduiItemGraphSetLabels(edui_graph_s *item, char *x, char *y, char *title);
eduiitem_s *eduiItemGreyGradPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemGreyPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemNumberCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemPropCreate(usize data, const void *colours, EdUiItemCallback selected, EdUiItemCallback changed,
                                   EdUiItemCallback button, i32 button_type, char *text, char *value);
eduiitem_s *eduiItemPropCreateEx(usize data, const void *colours, EdUiItemCallback selected,
                                     EdUiItemCallback changed, EdUiItemCallback button, i32 button_type, char *text,
                                     char *value, i32 extra_data);
i32 eduiItemPropSetText(edui_prop_s *item, char *text);
i32 eduiItemRender(eduiitem_s *item, eduimenu_s *menu, i32 x, i32 y, i32 width, i32 selected);
eduiitem_s *eduiItemSelCreate(usize data, const void *colours, i32 selected, i32 group, EdUiItemCallback callback,
                                  char *text);
eduiitem_s *eduiItemSelWithClipColourCreate(usize data, const void *colours, i32 selected, i32 group,
                                                EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemSeparatorCreate(usize data, const void *colours);
i32 eduiItemSetText(eduiitem_s *item, char *text);
eduiitem_s *eduiItemSliderCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback, f32 minimum,
                                     f32 range, f32 value, char *text);
eduiitem_s *eduiItemSliderCreateInt(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                        i32 minimum, i32 range, i32 value, char *text);
void eduiItemSliderSetFmt(edui_slider_s *item, char *format);
void eduiItemSliderSetGranularity(edui_slider_s *item, f32 granularity);
void eduiItemSliderSetVal(edui_slider_s *item, f32 value);
void eduiItemSliderSetValEx(edui_slider_s *item, f32 value, i32 reset_timer, i32 notify);
eduiitem_s *eduiItemTextPickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
void eduiItemTextPickSetFmt(edui_textpicker_s *item, char *format);
eduiitem_s *eduiItemTextSelectorCreate(usize data, const void *colours, i32 group, EdUiItemCallback callback,
                                           i32 count, i32 selected, char *text, char **options);
eduiitem_s *eduiItemTexturePickCreate(usize data, const void *colours, EdUiItemCallback callback, char *text);
eduiitem_s *eduiItemToggleCreate(usize data, const void *colours, i32 selected, i32 group,
                                     EdUiItemCallback callback, char *text);
extern eduiitem_s * edui_last_item;
eduiitem_s *eduiMenuAddItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemAfter(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *after);
void eduiMenuAddItemBefore(eduimenu_s *menu, eduiitem_s *item, eduiitem_s *before);
void eduiMenuAddItemFirst(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuAddItemLast(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuAttach(eduimenu_s *menu, eduimenu_s *child);
eduimenu_s *eduiMenuCreate(i32 x, i32 y, i32 width, i32 height, void *font, EdUiMenuCallback callback,
                               char *title);
void eduiMenuDestroy(eduimenu_s *menu);
i32 eduiMenuDetach(eduimenu_s *menu);
void eduiMenuEnsureSelection(eduimenu_s *menu);
void eduiMenuFitOnScreen(eduimenu_s *menu, i32 padding);
void eduiMenuFitWidth(eduimenu_s *menu, i32 padding);
void eduiMenuHighlight(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuIsActive(eduimenu_s *menu);
i32 eduiMenuItemMoveDown(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuItemMoveUp(eduimenu_s *menu, eduiitem_s *item);
i32 eduiMenuProcess(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessAux(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
i32 eduiMenuProcessInput(eduimenu_s *menu, f32 delta_time, nupad_s *pad, i32 item_result);
i32 eduiMenuProcessSelectedItem(eduimenu_s *menu, f32 delta_time, nupad_s *pad);
void eduiMenuRemoveItem(eduimenu_s *menu, eduiitem_s *item);
void eduiMenuRender(eduimenu_s *menu);
void eduiMenuSelectFirstEntry(eduimenu_s *menu);
void eduiMenuSetAttr(eduimenu_s *menu, const u32 *colours);
void eduiMenuSetDisabled(eduimenu_s *menu, i32 disabled);
void eduiMenuSetTransparency(eduimenu_s *menu, const u32 *colours);
void eduiMenuSortItemsByTxt(eduimenu_s *menu);
void eduiProcessCursor(f32 delta_time, nupad_s *pad);
void eduiProcessCursorDefault(f32 delta_time, nupad_s *pad);
i32 eduiProcessInteracts(eduimenu_s *menu, nupad_s *pad);
void eduiRenderCursor(void);
void eduiRenderInteracts(void);
void eduiSetActiveMenu(eduimenu_s *menu);
void eduiSetCameraEnabled(i32 enabled);
void eduiSetCursorColour(u32 colour);
void eduiSetCursorCoords(f32 x, f32 y);
void eduiSetDefaultActiveMenu(eduimenu_s *menu);
void eduiSetFont(NUQFNT *font);
void eduiSetFontScale(f32 x, f32 y);
void CreateTestMenu(void *font);
void TestMenu(nupad_s *pad);
void eduiSetGlobalSliderAccel(f32 acceleration);
void eduiSetRenderPlane(i8 plane);
void eduiSetUsingMenuFocus(i32 enabled);
void eduiShowCursor(i32 show);
i32 eduiUsedAlgPad(nupad_s *pad);
void eduicbCancelMessageMenu(eduimenu_s *, eduimenu_s *);
i32 eduicbInteractSlider(edui_interact_s *interact);
void eduicbItemDestroy(eduimenu_s *menu, eduiitem_s *item);
void eduicbItemDestroyProp(eduimenu_s *menu, eduiitem_s *item);

    // The editor UI callbacks share this translation unit with their cursor,
    // menu, and interaction state in the original binary.

static __used__ void cbMMCancel(eduimenu_s *, eduimenu_s *);

static __used__ i32 cbInteractMenuCancelChild(edui_interact_s *interact);

static __used__ void cbMMReturnToApp(eduimenu_s *, eduiitem_s *, u32);

static __used__ void cbmcfgCameraSpeed(eduimenu_s *, eduiitem_s *item, u32);

static __used__ void cbmcfgCancel(eduimenu_s *menu, eduimenu_s *);

static __used__ void cbMMRegSel(eduimenu_s *, eduiitem_s *selected, u32);
static __used__ void cbMMEditorConfig(eduimenu_s *menu, eduiitem_s *, u32);

static void eduiRenderGraphLine(nugraph_s *graph, i32 x, i32 y, i32 width, i32 height, u32 colour);

static __used__ void eduicbDestroyDirectoryList(eduimenu_s *menu, eduimenu_s *);

static void eduicbAttachDirectoryList(eduimenu_s *menu, edui_file_pick_s *picker);

static __used__ i32 eduicbInteractColourPick(edui_interact_s *interact);
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
static __used__ i32 eduicbInteractExpander(edui_interact_s *interact);
static __used__ i32 eduicbInteractProp(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractFilter(struct edui_interact_s *interact);
static __used__ i32 eduicbInteractSel(edui_interact_s *interact);
static __used__ void eduicbItemDestroyExpander(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemDestroyFilter(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemFilePickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemGradPickDestroy(eduimenu_s *, eduiitem_s *item);
static __used__ void eduicbItemSliderDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbItemTextPickDestroy(eduimenu_s *menu, eduiitem_s *item);
static __used__ void eduicbPickCFGCancel(eduimenu_s *menu, eduimenu_s *);


static __used__ i32 eduicbProcessColourPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessColourSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessExpander(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilePick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessFilter(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGradPick(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessGraph(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessGreyPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static i32 eduicbProcessPropKeyboard(eduimenu_s *menu, edui_prop_s *property);
static __used__ i32 eduicbProcessProp(eduimenu_s *menu, eduiitem_s *item, f32 delta_time, nupad_s *pad);
static __used__ i32 eduicbProcessSel(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessSeparator(eduimenu_s *, eduiitem_s *, f32, nupad_s *);
static __used__ i32 eduicbProcessSlider(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTextPick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
static __used__ i32 eduicbProcessTexturePick(eduimenu_s *menu, eduiitem_s *item, f32, nupad_s *pad);
__attribute__((force_align_arg_pointer)) static __used__ i32 eduicbRenderCheck(eduimenu_s *, eduiitem_s *item,
                                                                                   i32 x, i32 y, i32 width, i32);
    // Retail keeps this as the constant-propagated clone of the generic four-step
    // gradient helper; only the hue/value colour picker band reaches it.
    static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second)
        __asm__("_ZL19eduiDrawGrad4StepsViiiiffiiiii.constprop.59");
static void eduiDrawHueValueBand(i32 x, i32 y, i32 width, i32 height, f32 start, f32 end, u32 first, u32 second);
static __used__ i32 eduicbRenderColourPick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderColourSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderExpander(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGradPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderGraph(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32 selected);
static __used__ i32 eduicbRenderGreyPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderNumber(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderProp(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderFilter(struct eduimenu_s *menu, struct eduiitem_s *item, i32 x, i32 y, i32 scale,
                                           i32 selected);
static __used__ i32 eduicbRenderSel(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSelWithClipColour(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width,
                                                      i32);
static __used__ i32 eduicbRenderSeparator(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSlider(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderSliderInt(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextPick(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTextSelector(eduimenu_s *, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ i32 eduicbRenderTexturePick(eduimenu_s *menu, eduiitem_s *item, i32 x, i32 y, i32 width, i32);
static __used__ void eduicbSelectDirectoryEntry(eduimenu_s *menu, eduiitem_s *item, u32 value);
static __used__ void eduicbSelectDirectoryExit(eduimenu_s *menu, eduiitem_s *, u32);
}

static void eduiFntPrintExLeft(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExCentre(void *font, int x, int y, int, char *format, ...);

static void eduiFntPrintExEditBuffer(void *font, float x, float y, int, char *format, ...);

// Text-picker commit callbacks. Retail keeps them unmangled and local.
extern "C" {
__attribute__((visibility("hidden"), optimize("O3"))) void tpentOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                            unsigned int buttons);

__attribute__((visibility("hidden"), optimize("O3"))) void tpentPropOnEnter(eduimenu_s *menu, eduiitem_s *item,
                                                                                unsigned int buttons);
}
