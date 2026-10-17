#include "decomp.h"
#include "legoapi/world/area.h"
#include "legoapi/render/core/screen.h"

#include <stdlib.h>

#include "MechInputTouch/MechInputTouch_types.h"
#include "globals.h"

i32 Area_CharIDInCurrentList(i32 character_id) {
    i32 result = -1;
    if (CurrentCList != NULL) {
        for (i32 index = 0; CurrentStoryCList[index].model_id != -1 && index != 64; ++index) {
            if (CurrentStoryCList[index].model_id != character_id)
                continue;
            result = index;
            break;
        }
    }
    return result;
}
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/customiser.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/render.h"
#include "legoapi/world/level.h"
#include "legoapi/world/mission.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/nusound/nusound.h"
#include "legoapi/world/world.h"

struct APICHARACTERMODELLIST_s;
struct CUSTOMISER;
struct MISSIONSYS_s;

extern "C" volatile i32 abort_load;
extern NUGSCN *area_scene;
extern NUGSCN *vehicle_scene;
extern NUGSCN *big_icon_scene;
extern i16 id_ANAKINJEDISCARRED;
extern i16 id_HANINCARBONITE;
extern i16 id_DEFAULTCHARACTER[2];
extern i16 Area_PlayerModelList[18];
extern APICHARACTERMODELLIST_s Area_MissionModelList[49];
extern APICHARACTERMODELLIST_s Area_StoryModelList[52];
extern APICHARACTERMODELLIST_s FreePlayModelList[52];
extern APICHARACTERMODELLIST_s Hub_ModelList[6];
extern i32 Area_PlayerModelCount;
extern CUSTOMISER *CharacterCustomiser;
extern "C" i32 makefreeplaymodellist;
extern i32 BonusArea;
extern AREADATA *ANEWHOPE_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern AREADATA *PODRACE_ADATA;
extern AREADATA *BONUS_GUNSHIP_ADATA;
extern AREADATA *NEGOTIATIONS_ADATA;
extern AREADATA *GUNGAN_ADATA;
extern AREADATA *RESCUE_ADATA;
extern AREADATA *RETAKE_ADATA;
extern AREADATA *MAUL_ADATA;
extern AREADATA *BOUNTYHUNTERPURSUIT_ADATA;
extern AREADATA *FACTORY_ADATA;
extern AREADATA *JEDI_ADATA;
extern AREADATA *GUNSHIP_ADATA;
extern AREADATA *DOOKU_ADATA;
extern AREADATA *DOGFIGHT_ADATA;
extern AREADATA *BATTLEOVERCORUSCANT_ADATA;
extern AREADATA *CRUISER_ADATA;
extern AREADATA *GRIEVOUS_ADATA;
extern AREADATA *KASHYYYK_ADATA;
extern AREADATA *TEMPLE_ADATA;
extern AREADATA *VADER_ADATA;
extern AREADATA *BLOCKADERUNNER_ADATA;
extern AREADATA *TATOOINE_ADATA;
extern AREADATA *MOSEISLEY_ADATA;
extern AREADATA *DEATHSTARRESCUE_ADATA;
extern AREADATA *DEATHSTARESCAPE_ADATA;
extern AREADATA *DEATHSTARBATTLE_ADATA;
extern AREADATA *HOTHESCAPE_ADATA;
extern AREADATA *ASTEROIDCHASE_ADATA;
extern AREADATA *DAGOBAH_ADATA;
extern AREADATA *CLOUDCITYTRAP_ADATA;
extern AREADATA *CLOUDCITYESCAPE_ADATA;
extern AREADATA *JABBASPALACE_ADATA;
extern AREADATA *SARLACCPIT_ADATA;
extern AREADATA *SPEEDERCHASE_ADATA;
extern AREADATA *ENDORBATTLE_ADATA;
extern AREADATA *DEATHSTARBATTLE2_ADATA;
extern AREADATA *EMPERORFIGHT_ADATA;
extern AREADATA *E1CHARACTER_ADATA;
extern AREADATA *E2CHARACTER_ADATA;
extern AREADATA *E3CHARACTER_ADATA;
extern AREADATA *E4CHARACTER_ADATA;
extern AREADATA *E5CHARACTER_ADATA;
extern AREADATA *E6CHARACTER_ADATA;
extern AREADATA *E1VEHICLE_ADATA;
extern AREADATA *E2VEHICLE_ADATA;
extern AREADATA *E3VEHICLE_ADATA;
extern AREADATA *E4VEHICLE_ADATA;
extern AREADATA *E5VEHICLE_ADATA;
extern AREADATA *E6VEHICLE_ADATA;
extern AREADATA *ANAKINSFLIGHT_ADATA;
extern AREADATA *LEGOCITY_ADATA;
extern AREADATA *NEWTOWN_ADATA;
extern AREADATA *JIMTEST_ADATA;
extern i32 CHARPAK;
extern i32 CharacterDataLoad;
extern i32 loadareadata_loadlevel;
extern i32 AreaDataLoaded;
extern i32 Level;
extern "C" VARIPTR characterbuffer_ptr;
extern "C" VARIPTR characterbuffer_end;
extern i32 VehicleArea;
extern i32 last_area;
extern i32 BGLOAD;
extern i32 LOADEROFF;
extern i32 new_level_from_menu;
extern "C" i32 SuperStory;
extern i32 GAMEDEMO;
extern i32 textcrawlactive;
extern i32 readpads_always;
extern "C" i32 loadareacharacters_loadedlevel;
extern "C" i32 waiting_for_character;
extern "C" GAMEPAD_s GamePad[64];
extern "C" f32 MainRenderTime;
extern f32 backdrop_top_r;
extern f32 backdrop_top_g;
extern f32 backdrop_top_b;
extern f32 backdrop_bot_r;
extern f32 backdrop_bot_g;
extern f32 backdrop_bot_b;
extern "C" OPTIONSSAVE *Game_OptionsSave;
extern FadeSystem FadeSys;
extern void *theGameThings;

extern void MakeFreePlayModelList(i32, i32, i32, i32, i32);
extern void Customiser_LoadAccessories(CUSTOMISER *, APICHARACTERMODELLIST_s *);
extern void Customiser_SetAnimsToLoad(CUSTOMISER *, i32);
extern void GameLoadCharacterModels(APICHARACTERMODELLIST_s *, i32, variptr_u *, variptr_u *, i32, i32);
extern void CharScenes_AreaLoad(APICHARACTERMODELLIST_s *, variptr_u *, variptr_u);
extern void Area_Configure(i32, i32, EXTRAMODEL *, i16 *);
extern void SetBackgroundMusic(i32);
extern f32 GameSetMusicVolume(OPTIONSSAVE_s *);
extern void legoSetMusicVolume(f32);
extern void MusicClearAll(void);
extern void BackDrop_ResetColours(void);
extern void BackDrop_Update(f32);
extern void BackDrop_UpdateColours(i32);
extern void BackDrop_Draw(f32, i32);
extern i16 tTOUCHTOSTART;
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" f32 NuFrameEnd(void);
extern "C" void edGraEnableTerrainSwap(void);
extern "C" void edGraDisableTerrainSwap(void);
extern "C" i32 NuKeyboard_db(i32);
extern "C" i32 NuSound3LoadingSfx(void);
extern void Particles_LoadAreaPage(char *);
extern "C" {
    extern void SoundKillAll(void);
}

AREADATA *ADataList = NULL;
extern AREADATA * HUB_ADATA;
extern i32 loadareacharacters_no_backdrop_reset;



extern i32 Area;

i32 AREA_DEFAULTBONUSTIMETRIALTIME = 300;
i32 AREA_DEFAULTCHALLENGETIME = 600;

void FixUpAreas(void);

i32 AreaFromMiniKitID(i32 minikitId) {
    i32 i;

    for (i = 0; i < AREACOUNT; i++) {
        if (ADataList[i].minikit_id == minikitId) {
            return i;
        }
    }

    return -1;
}

struct bgprocinfo_s;
static __used__ void LoadAreaData(bgprocinfo_s *);

void LoadAreaCharacters();
