#include "decomp.h"
#include "gameapi_edtools_types.h"
#include "gameapi/edtools/edpp_internal.h"
#include "gameapi/edtools/edstubs.h"
#include "gameapi/edtools/edui.h"
#include "legoapi/legoapi_types.h"
#include "legoapi/render/fx.h"
#include "nu2api/numusic/sfx.h"
#include <stdio.h>

extern "C" {
    extern debkeydatatype_s *debkeydata;
    extern debinftype **debtab;
    extern debinftype *effecttypes;
    extern i32 debris_render_group;
    extern void *ed_fnt;
    extern u32 edblack[4];
    extern eduimenu_s *edptl_repeatbox_menu;
    extern i32 edpp_usememcard;
    extern i32 edbits_override_backups;
    extern i32 edbits_particle_general_page;
    extern i32 edbits_particle_level_page;
extern i32 edbits_particle_char_page;
    extern NUGSCN *edbits_base_scene;
    extern char edbits_datapath[256];
    extern char edbits_general_save_directory[256];
    extern char edbits_general_save_name[256];
    extern char edbits_general_save_extension[256];
    extern char edbits_level_save_directory[256];
    extern char edbits_level_save_name[256];
    extern char edbits_level_save_extension[256];
    void DebFreeInstantly(i32 *handle);
    void DebReAlloc(debkeydatatype_s *key, i32 particle_count);
    void DebrisSetDetailLevels(i32 handle, i32 detail_levels);
    void edppDeleteEffect(i32 index);
    i32 edppLoadPage(char *path, i32 flag, usize scene);
    void eduiCreateMessageMenu(eduimenu_s *parent, char *message, i32 highlighted);
    void DebrisReScale(i32 effect_index, f32 scale);
    void edppRestartAllEffectsInLevel(void);
}

void edppStartSingleEffect(i32 index);
void edppPtlDestroy(i32 index);
void edppDestroyAllPages();
i32 edppSaveEffects(char *filename, char page);
void cbFileSaveEffects(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClipboardMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbGhostMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStarMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSoundIDMenu(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlRepeatBoxMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCutClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbEmptyClipboard(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *, u32);
static void UpdateTotalPtls(debinftype *effect);

// The original particle editor's private callbacks and menu state share this TU.
#include "gameapi/edtools/edptlcallbacks.cpp"

void cbFileSaveEffects(eduimenu_s *parent, eduiitem_s *, u32);

static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *, u32);

static void cbPtlCancelRepeatBoxMenu(eduimenu_s *, eduimenu_s *);
static void cbPtlRepeatBoxMenu(eduimenu_s *parent, eduiitem_s *, u32);

static void UpdateTotalPtls(debinftype *effect);

// Particle list editor subsystem stubs (static, internal linkage).

static void edptlcbPageMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbStarMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbStopPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClearPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbGhostMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbGroupMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDetail(eduimenu_s *, eduiitem_s *item, u32);
void edppStartPage(i32 page);
static void edptlcbStartPage(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbBounceMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetMaxThin(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSoundID(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbSoundXMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbSoundsMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSwitchMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetDpadMode(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetDrawflag(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetSwitchId(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSoundIDMenu(eduimenu_s *parent, eduiitem_s *item, u32);
static void edptlcbCutClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbDpadModeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDrawflagMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbSetSwitchVar(eduimenu_s *, eduiitem_s *item, u32);
static void edptlChangeRepeatBox(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbClipboardMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbDeleteOrphans(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbSetSwitchType(eduimenu_s *menu, eduiitem_s *item, u32);
static void edptlcbApplyGhostTime(eduimenu_s *, eduiitem_s *item, u32);

static void edptlcbApplyNumGhosts(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOffRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOff(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOnRan(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeIvalOn(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeETime(eduimenu_s *, eduiitem_s *item, u32);
static void cbPtlChangeGenRate(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbApplyStarRatio(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbCancelPageMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbCancelStarMenu(eduimenu_s *, eduimenu_s *);
static void edptlcbChangeDistortX(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeDistortY(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbChangeRampTime(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbEmptyClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbOrphanListMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbPasteClipboard(eduimenu_s *menu, eduiitem_s *, u32);
static void edptlcbResetParticles(eduimenu_s *, eduiitem_s *, u32);
static void edptlcbSetMasterGroup(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSetScaleFactor(eduimenu_s *, eduiitem_s *item, u32);
static void edptlcbSwitchTypeMenu(eduimenu_s *parent, eduiitem_s *, u32);
static void edptlcbTestDetailMenu(eduimenu_s *parent, eduiitem_s *, u32);
