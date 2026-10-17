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

extern NUGSCN *area_scene;
extern NUGSCN *vehicle_scene;
extern NUGSCN *big_icon_scene;
extern i16 Area_PlayerModelList[18];
extern APICHARACTERMODELLIST_s Area_MissionModelList[49];
extern APICHARACTERMODELLIST_s Area_StoryModelList[52];
extern APICHARACTERMODELLIST_s FreePlayModelList[52];
extern APICHARACTERMODELLIST_s Hub_ModelList[6];
extern i32 Area_PlayerModelCount;
extern CUSTOMISER *CharacterCustomiser;
extern "C" i32 makefreeplaymodellist;
extern i32 BonusArea;
extern AREADATA *PODSPRINT_ADATA;
extern AREADATA *BONUS_GUNSHIP_ADATA;
extern AREADATA *BOUNTYHUNTERPURSUIT_ADATA;
extern AREADATA *GUNSHIP_ADATA;
extern AREADATA *DOGFIGHT_ADATA;
extern AREADATA *VADER_ADATA;
extern i32 Level;
extern i32 VehicleArea;
extern i32 last_area;
extern i32 BGLOAD;
extern i32 LOADEROFF;
extern i32 new_level_from_menu;
extern "C" i32 SuperStory;
extern i32 GAMEDEMO;
extern "C" GAMEPAD_s GamePad[64];
extern "C" OPTIONSSAVE *Game_OptionsSave;
extern FadeSystem FadeSys;
extern void *theGameThings;

extern void Customiser_LoadAccessories(CUSTOMISER *, APICHARACTERMODELLIST_s *);
extern void legoSetMusicVolume(f32);
extern void BackDrop_ResetColours(void);
extern "C" i32 NuRndrBeginScene(i32);
extern "C" void NuRndrEndScene(void);
extern "C" i32 NuSound3LoadingSfx(void);

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

