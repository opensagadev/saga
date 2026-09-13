#include "decomp.h"
#include "globals.h"
#include "legoapi/characters/core/character.h"

i32 CanPullLevers(i32 character_id) {
    u32 flags = CDataList[character_id].model_flags;
    if ((flags & 0x01000010) == 0x01000010) {
        return 0;
    }
    return (flags & 0x00040088) != 0;
}

#include "legoapi/characters/core/CharacterObjectInterface.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/area.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/numusic/sfx.h"

#include <string.h>
struct numtx_s;
struct APICHARACTERMODELLIST_s;
struct EXTRAMODEL;

extern "C" void NuHGobjRestrictEvaluation(nuhgobj_s *object);
extern "C" void NuHGobjRestoreEvaluation(void);
extern "C" void AddVariableShotDebrisEffect(i32 effect_id, NUVEC *position, i32 count, i16 angle_z, i16 angle_y);

// Forward declarations for local (static) character/gameplay helper stubs.
struct nuvec_s;
struct EDCREATURE_s;
struct APIOBJECT_s;

extern "C" {
    NUMTL *APITrans_Mtl[2];
    i32 notransparentchardraw;

    i16 id_WEIRDO1 = -1;
    i16 id_WEIRDO2 = -1;
    i16 id_BATMAN = -1;
    i16 id_ROBIN = -1;
    i16 id_HENCHMAN = -1;
    i16 id_BUGGY = -1;
    i16 id_GLIDEPACK = -1;
    i16 id_RADIOCAR = -1;
    i16 id_POLICECAR = -1;
    i16 id_TWOFACE = -1;
    i16 id_HARLEYQUINN = -1;
    i16 id_CATWOMAN = -1;
    i16 id_WHIP = -1;
    i16 id_PENGUIN_BOMB = -1;
    i16 id_PENGUIN_GOON = -1;
    i16 id_PENGUIN_GOON_GUN = -1;
    i16 id_JOKER_GOON = -1;
    i16 id_JOKER_GOON_GUN = -1;
    i16 id_RIDDLER_GOON = -1;
    i16 id_RIDDLER_GOON_GUN = -1;
    i16 id_JACKINABOX = -1;
    i16 id_MOWER = -1;
    i16 id_DODGEM = -1;
    i16 id_GYROCOPTER = -1;
    i16 id_CLAYFACE = -1;
    i16 id_PENGUIN = -1;
    i16 id_UMBRELLA = -1;
    i16 id_POISONIVY = -1;
    i16 id_QUIGONJINN = -1;
    i16 id_MACEWINDU = -1;
    i16 id_OBIWANKENOBI = -1;
    i16 id_PRINCESSLEIA = -1;
    i16 id_PRINCESSLEIASLAVE = -1;
    i16 id_PRINCESSLEIABOUSHH = -1;
    i16 id_CAPTAINANTILLES = -1;
    i16 id_CAPTAINTARPALS = -1;
    i16 id_IMPERIALGUARD = -1;
    i16 id_BODYGUARD = -1;
    i16 id_DARTHMAUL = -1;
    i16 id_GAMORREANGUARD = -1;
    i16 id_BATTLEDROID = -1;
    i16 id_BATTLEDROIDCOMMANDER = -1;
    i16 id_BATTLEDROIDGEONOSIAN = -1;
    i16 id_BATTLEDROIDSECURITY = -1;
    i16 id_CLONEEP3 = -1;
    i16 id_CLONEEP3SAND = -1;
    i16 id_NAFFDROID1 = -1;
    i16 id_NAFFDROID2 = -1;
    i16 id_NAFFDROID3 = -1;
    i16 id_NAFFDROID4 = -1;
    i16 id_MOUSEDROID = -1;
    i16 id_PROBEDROID = -1;
    i16 id_PKDROID = -1;
    i16 id_SNAKE = -1;
    i16 id_BAT = -1;
    i16 id_WOMPRAT = -1;
    i16 id_WAMPA = -1;
    i16 id_HANINCARBONITE = -1;
    i16 id_YODA = -1;
    i16 id_C3PO = -1;
    i16 id_TC14 = -1;
    i16 id_YODAGHOST = -1;
    i16 id_MOSEISLEYCITIZEN = -1;
    i16 id_CANTINAALIEN = -1;
    i16 id_CLOUDCITYCITIZEN = -1;
    i16 id_GEONOSIAN = -1;
    i16 id_BOB = -1;
    i16 id_WATTO = -1;
    i16 id_CHEWBACCA = -1;
    i16 id_WOOKIEE = -1;
    i16 id_ATST = -1;
    i16 id_ATST_LOWRES = -1;
    i16 id_BARMAN = -1;
    i16 id_DROIDEKA = -1;
    i16 id_SUPERBATTLEDROID = -1;
    i16 id_BOBAFETT = -1;
    i16 id_TRAININGREMOTE = -1;
    i16 id_ATAT = -1;
    i16 id_SERVICECAR = -1;
    i16 id_DRAGBOMB = -1;
    i16 id_CLONEWALKER = -1;
    i16 id_WICKET = -1;
    i16 id_EWOK = -1;
    i16 id_CATAPULT = -1;
    i16 id_BASKETCANNON = -1;
    i16 id_R2Q5 = -1;
    i16 id_BANTHA = -1;
    i16 id_BOMARRMONK = -1;
    i16 id_DEWBACK = -1;
    i16 id_LANDSPEEDER = -1;
    i16 id_TAUNTAUN = -1;
    i16 id_SPEEDERBIKE = -1;
    i16 id_HEAVYREPEATINGCANNON = -1;
    i16 id_BIGGUN = -1;
    i16 id_4LOM = -1;
    i16 id_TROOPERCANNON = -1;
    i16 id_MOSCANNON = -1;
    i16 id_CANNON = -1;
    i16 id_JABBA = -1;
    i16 id_BOSSNASS = -1;
    i16 id_SLAVE1 = -1;
    i16 id_YWING = -1;
    i16 id_TIEBOMBER = -1;
    i16 id_GRIEVOUS = -1;
    i16 id_TIEFIGHTERDARTH = -1;
    i16 id_TIEFIGHTER = -1;
    i16 id_GRABCONTROL = -1;
    i16 id_GRABR2CONTROL = -1;
    i16 id_GRABMACHINE = -1;
    i16 id_GRABMAGNET = -1;
    i16 id_ROBOTBASE = -1;
    i16 id_LUKESKYWALKERDAGOBAH = -1;
    i16 id_SNOWMOB = -1;
    i16 id_SNOWTROOPER = -1;
    i16 id_DEATHSTARTROOPER = -1;
    i16 id_DARTHVADER = -1;
    i16 id_THEEMPEROR = -1;
    i16 id_MILLENNIUMFALCON = -1;
    i16 id_SPEEDERBIKESNOW = -1;
    i16 id_KAADU = -1;
    i16 id_GUNGAN = -1;
    i16 id_FALUMPASET = -1;
    i16 id_STAP2 = -1;
    i16 id_JUMBOHOMINGDROID = -1;
    i16 id_JARJAR = -1;
    i16 id_PADMECLAWED = -1;
    i16 id_ANAKINPADAWAN = -1;
    i16 id_OBIWANKENOBIJEDIMASTER = -1;
    i16 id_SHAAKTI = -1;
    i16 id_LUMINARA = -1;
    i16 id_JANGOFETT = -1;
    i16 id_JEDISTARFIGHTERYELLOWEP3 = -1;
    i16 id_JEDISTARFIGHTERREDEP3 = -1;
    i16 id_RANCOR = -1;
    i16 id_OBIWANKENOBIEP3 = -1;
    i16 id_ANAKINJEDI = -1;
    i16 id_ANAKINJEDISCARRED = -1;
    i16 id_REPUBLICGUNSHIP = -1;
    i16 id_REPUBLICGUNSHIP_GREEN = -1;
    i16 id_NEW_REPUBLIC_GUNSHIP = -1;
    i16 id_NEW_REPUBLIC_GUNSHIP_GREEN = -1;
    i16 id_COUNTDOOKU = -1;
    i16 id_PALPATINE = -1;
    i16 id_KITFISTO = -1;
    i16 id_XWING = -1;
    i16 id_ROYALGUARD = -1;
    i16 id_SNOWSPEEDER = -1;
    i16 id_TIEINTERCEPTOR = -1;
    i16 id_IMPERIALSHUTTLE = -1;
    i16 id_UGNAUGHT = -1;
    i16 id_SENTRYDROID = -1;
    i16 id_KAMINOANDROID = -1;
    i16 id_STORMTROOPER = -1;
    i16 id_BEACHTROOPER = -1;
    i16 id_IMPERIALSHUTTLEPILOT = -1;
    i16 id_IMPERIALOFFICER = -1;
    i16 id_GRANDMOFFTARKIN = -1;
    i16 id_GONKDROID = -1;
    i16 id_JAWA = -1;
    i16 id_MOONCAR = -1;
    i16 id_MAPCAR = -1;
    i16 id_WOOKIEFLYER = -1;
    i16 id_IMPERIALSPY = -1;
    i16 id_GREEDO = -1;
    i16 id_BOSSK = -1;
    i16 id_CANTINABAND = -1;
    i16 id_PITDROID = -1;
    i16 id_TOWNCAR = -1;
    i16 id_TRACTOR = -1;
    i16 id_FIRETRUCK = -1;
    i16 id_LIFEBOAT = -1;
    i16 id_LAMASU = -1;
    i16 id_TAUNWE = -1;
    i16 id_DEXTER = -1;
    i16 id_BIBFORTUNA = -1;
    i16 id_ADMIRALACKBAR = -1;
    i16 id_LOBOT = -1;
    i16 id_BESPINGUARD = -1;
    i16 id_TUSKENRAIDER = -1;
    i16 id_BUZZDROID = -1;
    i16 id_CLONEARC = -1;
    i16 id_NABOOSTARFIGHTER = -1;
    i16 id_NABOOSTARFIGHTERLIME = -1;
    i16 id_FLASHSPEEDER = -1;
    i16 id_SKELETON = -1;
    i16 id_MINIXWING = -1;
    i16 id_MINIYWING = -1;
    i16 id_MINITIEINTERCEPTOR = -1;
    i16 id_MINITIEBOMBER = -1;
    i16 id_MINIATAT = -1;
    i16 id_MINISTARDESTROYER = -1;
    i16 id_MINIROYALSTARSHIP = -1;
    i16 id_MINIIMPERIALSHUTTLE = -1;
    i16 id_MINIMILLENNIUMFALCON = -1;
    i16 id_MINIATST = -1;
    i16 id_MINIATTE = -1;
    i16 id_MINISLAVE1 = -1;
    i16 id_MINIDROIDEKA = -1;
    i16 id_MINITIEFIGHTER = -1;
    i16 id_MINITIEADVANCED = -1;
    i16 id_MINISITHINFILTRATOR = -1;
    i16 id_MINISOLARSAILOR = -1;
    i16 id_MINISANDCRAWLER = -1;
    i16 id_ANAKINSPOD = -1;
    i16 id_ANAKINSPODGREEN = -1;
    i16 id_SEBULBASPOD = -1;
    i16 id_GASGANOSPOD = -1;
    i16 id_ANOTHERMISCPOD = -1;
    i16 id_ANOTHERMISCPOD2 = -1;
    i16 id_ANAKINSNEWPOD = -1;
    i16 id_ANAKINSNEWPODGREEN = -1;
    i16 id_ANAKINSSPEEDER = -1;
    i16 id_ANAKINSSPEEDER_GREEN = -1;
    i16 id_ZAMSSPEEDER = -1;
    i16 id_VULTUREDROID = -1;
    i16 id_DROIDTRIFIGHTER = -1;
    i16 id_DROIDSTARFIGHTER = -1;

    CHARFIXUP CharFixUp[222] = {
        {"weirdo1", &id_WEIRDO1},
        {"weirdo2", &id_WEIRDO2},
        {"batman", &id_BATMAN},
        {"robin", &id_ROBIN},
        {"henchman", &id_HENCHMAN},
        {"buggy", &id_BUGGY},
        {"glidepack", &id_GLIDEPACK},
        {"radiocar", &id_RADIOCAR},
        {"policecar", &id_POLICECAR},
        {"twoface", &id_TWOFACE},
        {"harleyquinn", &id_HARLEYQUINN},
        {"catwoman", &id_CATWOMAN},
        {"whip", &id_WHIP},
        {"PenguinBomb", &id_PENGUIN_BOMB},
        {"p_goon", &id_PENGUIN_GOON},
        {"p_goon_gun", &id_PENGUIN_GOON_GUN},
        {"j_goon", &id_JOKER_GOON},
        {"j_goon_gun", &id_JOKER_GOON_GUN},
        {"r_goon", &id_RIDDLER_GOON},
        {"r_goon_gun", &id_RIDDLER_GOON_GUN},
        {"jackinabox", &id_JACKINABOX},
        {"mower", &id_MOWER},
        {"dodgem", &id_DODGEM},
        {"gyrocopter", &id_GYROCOPTER},
        {"clayface", &id_CLAYFACE},
        {"penguin", &id_PENGUIN},
        {"umbrella", &id_UMBRELLA},
        {"poisonivy", &id_POISONIVY},
        {"quigonjinn", &id_QUIGONJINN},
        {"macewindu", &id_MACEWINDU},
        {"obiwankenobi", &id_OBIWANKENOBI},
        {"princessleia", &id_PRINCESSLEIA},
        {"princessleia_slave", &id_PRINCESSLEIASLAVE},
        {"princessleia_Boushh", &id_PRINCESSLEIABOUSHH},
        {"captainantilles", &id_CAPTAINANTILLES},
        {"captaintarpals", &id_CAPTAINTARPALS},
        {"imperialguard", &id_IMPERIALGUARD},
        {"bodyguard", &id_BODYGUARD},
        {"darthmaul", &id_DARTHMAUL},
        {"gamorreanguard", &id_GAMORREANGUARD},
        {"battledroid", &id_BATTLEDROID},
        {"battledroid_commander", &id_BATTLEDROIDCOMMANDER},
        {"battledroid_geonosian", &id_BATTLEDROIDGEONOSIAN},
        {"battledroid_security", &id_BATTLEDROIDSECURITY},
        {"Clone_Ep3", &id_CLONEEP3},
        {"Clone_Ep3_Sand", &id_CLONEEP3SAND},
        {"naffdroid1", &id_NAFFDROID1},
        {"naffdroid2", &id_NAFFDROID2},
        {"naffdroid3", &id_NAFFDROID3},
        {"naffdroid4", &id_NAFFDROID4},
        {"mousedroid", &id_MOUSEDROID},
        {"probedroid", &id_PROBEDROID},
        {"pkdroid", &id_PKDROID},
        {"snake", &id_SNAKE},
        {"bat", &id_BAT},
        {"womprat", &id_WOMPRAT},
        {"wampa", &id_WAMPA},
        {"hanincarbonite", &id_HANINCARBONITE},
        {"yoda", &id_YODA},
        {"c3po", &id_C3PO},
        {"tc14", &id_TC14},
        {"yoda_ghost", &id_YODAGHOST},
        {"moseisleycitizen", &id_MOSEISLEYCITIZEN},
        {"cantinaaliens", &id_CANTINAALIEN},
        {"cloudcitycitizen", &id_CLOUDCITYCITIZEN},
        {"geonosian", &id_GEONOSIAN},
        {"watto", &id_WATTO},
        {"weirdo1", &id_WEIRDO1},
        {"weirdo2", &id_WEIRDO2},
        {"chewbacca", &id_CHEWBACCA},
        {"wookie", &id_WOOKIEE},
        {"atst", &id_ATST},
        {"atst_lowres", &id_ATST_LOWRES},
        {"atst", &id_ATST},
        {"barman", &id_BARMAN},
        {"destroyer", &id_DROIDEKA},
        {"superbattledroid", &id_SUPERBATTLEDROID},
        {"bobafett", &id_BOBAFETT},
        {"trainingremote", &id_TRAININGREMOTE},
        {"atat", &id_ATAT},
        {"service_car", &id_SERVICECAR},
        {"dragbomb", &id_DRAGBOMB},
        {"clonewalker", &id_CLONEWALKER},
        {"wicket", &id_WICKET},
        {"ewok", &id_EWOK},
        {"catapult", &id_CATAPULT},
        {"basketcannon", &id_BASKETCANNON},
        {"r2q5", &id_R2Q5},
        {"bantha", &id_BANTHA},
        {"bomarrmonk", &id_BOMARRMONK},
        {"dewback", &id_DEWBACK},
        {"speeder_land", &id_LANDSPEEDER},
        {"tauntaun", &id_TAUNTAUN},
        {"speederbike", &id_SPEEDERBIKE},
        {"heavyrepeatingcannon", &id_HEAVYREPEATINGCANNON},
        {"biggun", &id_BIGGUN},
        {"4lom", &id_4LOM},
        {"troopercannon", &id_TROOPERCANNON},
        {"moscannon", &id_MOSCANNON},
        {"cannon", &id_CANNON},
        {"jabba", &id_JABBA},
        {"bossnass", &id_BOSSNASS},
        {"slave1", &id_SLAVE1},
        {"ywing", &id_YWING},
        {"tiebomber", &id_TIEBOMBER},
        {"grievous", &id_GRIEVOUS},
        {"tiefighterdarth", &id_TIEFIGHTERDARTH},
        {"tiefighter", &id_TIEFIGHTER},
        {"grabberControl", &id_GRABCONTROL},
        {"grabberr2control", &id_GRABR2CONTROL},
        {"grabber", &id_GRABMACHINE},
        {"magnet", &id_GRABMAGNET},
        {"robot_base", &id_ROBOTBASE},
        {"lukeskywalker_dagobah", &id_LUKESKYWALKERDAGOBAH},
        {"SnowMob", &id_SNOWMOB},
        {"snowtrooper", &id_SNOWTROOPER},
        {"deathstartrooper", &id_DEATHSTARTROOPER},
        {"darthvader", &id_DARTHVADER},
        {"theemperor", &id_THEEMPEROR},
        {"millenniumfalcon", &id_MILLENNIUMFALCON},
        {"speederbikesnow", &id_SPEEDERBIKESNOW},
        {"Kaadu", &id_KAADU},
        {"Gungan", &id_GUNGAN},
        {"Falumpaset", &id_FALUMPASET},
        {"Stap2", &id_STAP2},
        {"jumbohomingdroid", &id_JUMBOHOMINGDROID},
        {"JarJarBinks", &id_JARJAR},
        {"PadmeClawed", &id_PADMECLAWED},
        {"Anakin_padawan", &id_ANAKINPADAWAN},
        {"ObiWanKenobi_Jedi", &id_OBIWANKENOBIJEDIMASTER},
        {"shaakti", &id_SHAAKTI},
        {"luminara", &id_LUMINARA},
        {"JangoFett", &id_JANGOFETT},
        {"JediStarfighter_Yellow_Ep3", &id_JEDISTARFIGHTERYELLOWEP3},
        {"JediStarfighter_Red_Ep3", &id_JEDISTARFIGHTERREDEP3},
        {"Rancor", &id_RANCOR},
        {"obiwankenobi_ep3", &id_OBIWANKENOBIEP3},
        {"anakin_jedi", &id_ANAKINJEDI},
        {"anakin_jedi_scarred", &id_ANAKINJEDISCARRED},
        {"republicgunship", &id_REPUBLICGUNSHIP},
        {"republicgunship_green", &id_REPUBLICGUNSHIP_GREEN},
        {"newrepublicgunship", &id_NEW_REPUBLIC_GUNSHIP},
        {"newrepublicgunship_green", &id_NEW_REPUBLIC_GUNSHIP_GREEN},
        {"countdooku", &id_COUNTDOOKU},
        {"palpatine", &id_PALPATINE},
        {"kitfisto", &id_KITFISTO},
        {"xwing", &id_XWING},
        {"royalguard", &id_ROYALGUARD},
        {"snowspeeder", &id_SNOWSPEEDER},
        {"tieinterceptor", &id_TIEINTERCEPTOR},
        {"imperialshuttle", &id_IMPERIALSHUTTLE},
        {"ugnaught", &id_UGNAUGHT},
        {"sentrydroid", &id_SENTRYDROID},
        {"kaminoandroid", &id_KAMINOANDROID},
        {"stormtrooper", &id_STORMTROOPER},
        {"beachtrooper", &id_BEACHTROOPER},
        {"ImperialShuttlePilot", &id_IMPERIALSHUTTLEPILOT},
        {"imperialofficer", &id_IMPERIALOFFICER},
        {"grandmofftarkin", &id_GRANDMOFFTARKIN},
        {"gonkdroid", &id_GONKDROID},
        {"jawa", &id_JAWA},
        {"mooncar", &id_MOONCAR},
        {"mapcar", &id_MAPCAR},
        {"wookieflyer", &id_WOOKIEFLYER},
        {"imperialspy", &id_IMPERIALSPY},
        {"greedo", &id_GREEDO},
        {"bossk", &id_BOSSK},
        {"cantinaband", &id_CANTINABAND},
        {"pitdroid", &id_PITDROID},
        {"TownCar", &id_TOWNCAR},
        {"Tractor", &id_TRACTOR},
        {"fireTruck", &id_FIRETRUCK},
        {"lifeBoat", &id_LIFEBOAT},
        {"lamasu", &id_LAMASU},
        {"taunwe", &id_TAUNWE},
        {"dexter", &id_DEXTER},
        {"bibfortuna", &id_BIBFORTUNA},
        {"admiralackbar", &id_ADMIRALACKBAR},
        {"lobot", &id_LOBOT},
        {"bespinguard", &id_BESPINGUARD},
        {"tuskenraider", &id_TUSKENRAIDER},
        {"buzzdroid", &id_BUZZDROID},
        {"clonearc", &id_CLONEARC},
        {"naboostarfighter", &id_NABOOSTARFIGHTER},
        {"naboostarfighter_lime", &id_NABOOSTARFIGHTERLIME},
        {"flashspeeder", &id_FLASHSPEEDER},
        {"skeleton", &id_SKELETON},
        {"mini_x_wing", &id_MINIXWING},
        {"mini_y_wing", &id_MINIYWING},
        {"mini_tie_interceptor", &id_MINITIEINTERCEPTOR},
        {"mini_tie_bomber", &id_MINITIEBOMBER},
        {"mini_atat", &id_MINIATAT},
        {"mini_star_destroyer", &id_MINISTARDESTROYER},
        {"mini_royal_stardestroyer", &id_MINIROYALSTARSHIP},
        {"mini_imperial_shuttle", &id_MINIIMPERIALSHUTTLE},
        {"Mini_Millennium_Falcon", &id_MINIMILLENNIUMFALCON},
        {"mini_atst", &id_MINIATST},
        {"mini_atte", &id_MINIATTE},
        {"mini_slave1", &id_MINISLAVE1},
        {"mini_droideka", &id_MINIDROIDEKA},
        {"mini_tie_fighter", &id_MINITIEFIGHTER},
        {"mini_tie_advanced", &id_MINITIEADVANCED},
        {"mini_sith_infiltrator", &id_MINISITHINFILTRATOR},
        {"mini_solar_sailer", &id_MINISOLARSAILOR},
        {"mini_sand_crawler", &id_MINISANDCRAWLER},
        {"anakinspod", &id_ANAKINSPOD},
        {"anakinspod_green", &id_ANAKINSPODGREEN},
        {"sebulbaspod", &id_SEBULBASPOD},
        {"gasganospod", &id_GASGANOSPOD},
        {"anothermiscpod", &id_ANOTHERMISCPOD},
        {"anothermiscpod2", &id_ANOTHERMISCPOD2},
        {"newanakinspod", &id_ANAKINSNEWPOD},
        {"newanakinspod_green", &id_ANAKINSNEWPODGREEN},
        {"anakinsspeeder", &id_ANAKINSSPEEDER},
        {"anakinsspeeder_green", &id_ANAKINSSPEEDER_GREEN},
        {"ZamsSpeeder", &id_ZAMSSPEEDER},
        {"vulturedroid", &id_VULTUREDROID},
        {"droidtrifighter", &id_DROIDTRIFIGHTER},
        {"droidstarfighter", &id_DROIDSTARFIGHTER},
        {NULL, NULL},
    };
}

i32 CHARCOUNT = 0;
CHARACTERDATA *CDataList = NULL;
GAMECHARACTERDATA *GCDataList = NULL;
// Original 0x120-byte data record @0x00666960. FixUpCharacters copies this
// canonical prefix into every character before character-specific overrides.
// These values cover the fully typed common movement and perception fields.
GAMECHARACTERDATA GCDATA_DEFAULT = {
    MakeLayerList_Index,
    0,
    0,
    0.1f,
    0.0f,
    1.0f,
    2.0f,
    3.0f,
    0.75f,
    -5.0f,
    0.0f,
    2.0f,
    1.0f,
    1.0f,
    2.0f,
    1.0f,
    1.0f,
    5.0f,
    0.0f,
    0.3375000059604645f,
    1.0471975803375244f,
    0.3490658700466156f,
    10.0f,
    -1.8325958251953125f,
    0.0f,
    6.0f,
    3.0f,
    0.5f,
    -0.5f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    0.0f,
    99.0f,
    0,
    0,
    0x80,
    1,
    1,
    1,
    1,
    1,
    0,
    2.75f,
    5.5f,
    10.0f,
    5.0f,
    0,
    0,
    0x204,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0x0456ffffu,
    0,
    1,
    0,
    0xff,
    0,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffffffffu,
    0xffff,
    0xff,
    0xff,
    0xffffffffu,
    0xffff,
    0,
};

i32 g_loadingCharacterInHub;


CharacterObjectInterface::CharacterObjectInterface(GameObject_s &value) : object(&value) {
    object->mech_object_interface = this;
}

f32 CharacterObjectInterface::GetHeight() const {
    return object->apiobj.scaled_height;
}

void CharacterObjectInterface::GetPos(VuVec &position, i32) const {
    position = VuVec(object->apiobj.collision_position.x, object->apiobj.collision_position.y,
                     object->apiobj.collision_position.z, 1.0f);
}

f32 CharacterObjectInterface::GetRadius() const {
    return object->field_0x1008;
}

const char *CharacterObjectInterface::GetTargetName() const {
    return "Character";
}

bool CharacterObjectInterface::IsDead() {
    if ((object->apiobj.field_0x1f8 & 0x1001) != 0x1001)
        return 1;
    return object->apiobj.field_0x287 != 0;
}

void CharacterObjectInterface::TargetedFlash() {
    object->targeted_flash = 1.0f;
}

CharacterObjectInterface::~CharacterObjectInterface() {
    object->mech_object_interface = NULL;
}

static __used__ void NewCharacterIdle(GameObject_s *, i32) {
}

static __used__ void ExtraDieSfx_LSW(GameObject_s *) {
}

static __used__ void ExtraHurtSfx_LSW(GameObject_s *) {
}

extern AREADATA *DAGOBAH_ADATA;
extern AREADATA *DEATHSTARESCAPE_ADATA;
extern AREADATA *DEATHSTARRESCUE_ADATA;
extern AREADATA *HOTHESCAPE_ADATA;
extern AREADATA *JABBASPALACE_ADATA;

extern "C" {



    extern void RootFn(NUMTX *, void *, NUVEC *, NUVEC *, NUVEC *, f32);
    extern void RootFnY(NUMTX *, void *, NUVEC *, NUVEC *, NUVEC *, f32);
    extern void BlendRootFn(NUMTX *, void *, NUVEC *, NUVEC *, NUVEC *, f32);

    extern i32 drawcharactermodel_nobsa;
    void (*APIObjResetShadowMapRenderingFn)(void);
    void (*APIObjEnableShadowMapRenderingFn)(void);
    i32 nurndr_force_lod;
    void NuRndrStartReflectionRender(i32);
    void NuRndrEndReflectionRender(void);

    // Original @0x3d0563. Hierarchy evaluation, DWA, locator storage, character
    // surface effects, transparency and reflection. The AddAnimEffects branch
    // still awaits reconstruction of its animation-event helper.
    i32 APIDrawCharacterModel(CHARACTERMODEL_s *model, CHARACTERDATA *, ANIMPACKET_s *animation, NUMTX *matrix, NUMTX *,
                              NUMTX *reflection_matrix, NUVEC *locator_positions, NUMTX *locator_matrices,
                              GameObject_s *object, u32 flags, NUJOINTANIM_s *joint_overrides, i32 joint_override_count,
                              WORLDINFO_s *, f32, NUMTX *output_matrices, i32, void *) {
        drawcharactermodel_locatorsupdated = 0;
        if (model == NULL)
            return 0;
        i32 result = 0;
        i32 evaluate_only = 0;
        if (animation != NULL) {
            if (animation->frame != 0xffff) {
                if (animation->field_0x3a >= 0 && animation->field_0x3a < apicharsys->model_id_capacity &&
                    model->model_data_b[animation->field_0x3a] != NULL && static_cast<i16>(animation->frame) >= 0 &&
                    static_cast<i16>(animation->frame) < apicharsys->model_id_capacity &&
                    model->model_data_b[static_cast<i16>(animation->frame)] != NULL) {
                    if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->field_0x3a])->flags & 0x220) !=
                            0 ||
                        (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->frame])->flags & 0x220) != 0)
                        evaluate_only = 1;
                }
            } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                       animation->blend_animation_a < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_a] != NULL && animation->blend_animation_b >= 0 &&
                       animation->blend_animation_b < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_b] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a])->flags &
                     0x220) != 0 ||
                    (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b])->flags &
                     0x220) != 0)
                    evaluate_only = 1;
            } else if (animation->blending != 0 && animation->blend_animation_b >= 0 &&
                       animation->blend_animation_b < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_b] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b])->flags &
                     0x220) != 0)
                    evaluate_only = 1;
            } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                       animation->blend_animation_a < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->blend_animation_a] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a])->flags &
                     0x220) != 0)
                    evaluate_only = 1;
            } else if (animation->blending == 0 && animation->animation_index >= 0 &&
                       animation->animation_index < apicharsys->model_id_capacity &&
                       model->model_data_b[animation->animation_index] != NULL) {
                if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->animation_index])->flags & 0x220) !=
                    0)
                    evaluate_only = 1;
            }
        }
        if (model->hierarchy == NULL || matrix == NULL)
            goto cleanup;

        {
            NUVEC bounds_min = model->hierarchy->bounds_min;
            NUVEC bounds_max = model->hierarchy->bounds_max;
            if ((object == NULL || (object->apiobj.field_0x1f4 & 0x200) == 0) &&
                NuCameraClipTestExtents(&bounds_min, &bounds_max, matrix, character_farclip, 0) == 0) {
                if (evaluate_only == 0)
                    goto cleanup;
                evaluate_only = 1;
            } else {
                evaluate_only = 0;
            }
            if (evaluate_only != 0)
                NuHGobjRestrictEvaluation(model->hierarchy);

            i16 render_indices[32];
            const i32 render_count = MakeLayerList != NULL ? MakeLayerList(model, render_indices, flags) : 0;
            if (render_count <= 0)
                goto cleanup;

            void **dwa = NULL;
            if (evaluate_only == 0 && animation != NULL && drawcharactermodel_nobsa == 0 &&
                drawcharactermodel_restpose == 0) {
                if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                    animation->blend_animation_a < apicharsys->model_id_capacity &&
                    model->model_data_c[animation->blend_animation_a] != NULL && animation->blend_animation_b >= 0 &&
                    animation->blend_animation_b < apicharsys->model_id_capacity &&
                    model->model_data_c[animation->blend_animation_b] != NULL) {
                    dwa = NuHGobjEvalDwaBlend2(
                        render_count, render_indices,
                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_a]),
                        animation->time,
                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_b]),
                        animation->blend_target_time, animation->blend_elapsed / animation->blend_duration);
                } else if (animation->blending != 0 && animation->blend_animation_b >= 0 &&
                           animation->blend_animation_b < apicharsys->model_id_capacity &&
                           model->model_data_c[animation->blend_animation_b] != NULL) {
                    dwa =
                        NuHGobjEvalDwa2(render_count, render_indices,
                                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_b]),
                                        animation->blend_target_time);
                } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                           animation->blend_animation_a < apicharsys->model_id_capacity &&
                           model->model_data_c[animation->blend_animation_a] != NULL) {
                    dwa =
                        NuHGobjEvalDwa2(render_count, render_indices,
                                        static_cast<nuanimdata2_s *>(model->model_data_c[animation->blend_animation_a]),
                                        animation->time);
                } else if (animation->blending == 0 && animation->animation_index >= 0 &&
                           animation->animation_index < apicharsys->model_id_capacity &&
                           model->model_data_c[animation->animation_index] != NULL) {
                    dwa = NuHGobjEvalDwa2(render_count, render_indices,
                                          static_cast<nuanimdata2_s *>(model->model_data_c[animation->animation_index]),
                                          animation->current_time);
                }
            }

            bool evaluated = false;
            if (animation != NULL && drawcharactermodel_noani == 0 && drawcharactermodel_restpose == 0) {
                if (animation->frame != 0xffff) {
                    if (animation->field_0x3a >= 0 && animation->field_0x3a < apicharsys->model_id_capacity &&
                        model->model_data_b[animation->field_0x3a] != NULL && static_cast<i16>(animation->frame) >= 0 &&
                        static_cast<i16>(animation->frame) < apicharsys->model_id_capacity &&
                        model->model_data_b[static_cast<i16>(animation->frame)] != NULL) {
                        NuHGobjEvalAnimBlend2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->field_0x3a]),
                            animation->time, static_cast<ani3_animheader_s *>(model->model_data_b[animation->frame]),
                            animation->time, animation->field_0x44, joint_override_count, joint_overrides,
                            output_matrices);
                        evaluated = true;
                    }
                } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                           animation->blend_animation_a < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_a] != NULL &&
                           animation->blend_animation_b >= 0 &&
                           animation->blend_animation_b < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_b] != NULL) {
                    if ((static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a])->flags &
                         0x20) != 0 ||
                        (static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b])->flags &
                         0x20) != 0) {
                        NuHGobjEvalAnimBlend2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, animation->blend_elapsed / animation->blend_duration,
                            joint_override_count, joint_overrides, output_matrices, BlendRootFn, object);
                    } else {
                        NuHGobjEvalAnimBlend2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, animation->blend_elapsed / animation->blend_duration,
                            joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                } else if (animation->blending != 0 && animation->blend_animation_b >= 0 &&
                           animation->blend_animation_b < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_b] != NULL) {
                    CHARACTERANIM_s *selected =
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_b]);
                    if ((selected->flags & 0x20) != 0) {
                        NuHGobjEvalAnim2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, joint_override_count, joint_overrides, output_matrices,
                            (selected->flags & 0x200) != 0 ? RootFnY : RootFn, object);
                    } else {
                        NuHGobjEvalAnim2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_b]),
                            animation->blend_target_time, joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                } else if (animation->blending != 0 && animation->blend_animation_a >= 0 &&
                           animation->blend_animation_a < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->blend_animation_a] != NULL) {
                    CHARACTERANIM_s *selected =
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->blend_animation_a]);
                    if ((selected->flags & 0x20) != 0) {
                        NuHGobjEvalAnim2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time, joint_override_count, joint_overrides, output_matrices,
                            (selected->flags & 0x200) != 0 ? RootFnY : RootFn, object);
                    } else {
                        NuHGobjEvalAnim2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->blend_animation_a]),
                            animation->time, joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                } else if (animation->blending == 0 && animation->animation_index >= 0 &&
                           animation->animation_index < apicharsys->model_id_capacity &&
                           model->model_data_b[animation->animation_index] != NULL) {
                    CHARACTERANIM_s *selected =
                        static_cast<CHARACTERANIM_s *>(model->model_data_a[animation->animation_index]);
                    if ((selected->flags & 0x20) != 0) {
                        NuHGobjEvalAnim2Root(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->animation_index]),
                            animation->current_time, joint_override_count, joint_overrides, output_matrices,
                            (selected->flags & 0x200) != 0 ? RootFnY : RootFn, object);
                    } else {
                        NuHGobjEvalAnim2(
                            model->hierarchy,
                            static_cast<ani3_animheader_s *>(model->model_data_b[animation->animation_index]),
                            animation->current_time, joint_override_count, joint_overrides, output_matrices);
                    }
                    evaluated = true;
                }
            }
            if (!evaluated) {
                NuHGobjEval(model->hierarchy, joint_override_count,
                            reinterpret_cast<nuhgobjjointoverride_s *>(joint_overrides), output_matrices);
            }
            StoreLocatorCoordinates(model, matrix, output_matrices, locator_positions, locator_matrices);
            drawcharactermodel_locatorsupdated = 1;

            const i32 render_flags = object == NULL || (object->apiobj.field_0x1f4 & 0x200) == 0;
            if (object != NULL && apicharsys != NULL && apicharsys->set_creature_lights != NULL) {
                apicharsys->set_creature_lights(&object->apiobj);
            }

            if (!evaluate_only) {
                if (object != NULL && (object->apiobj.field_0x1f4 & 0x20000) != 0) {
                    if (APIObjResetShadowMapRenderingFn != NULL)
                        APIObjResetShadowMapRenderingFn();
                    APITransparentCharDraw(model->hierarchy, matrix, render_count, render_indices, output_matrices, dwa,
                                           render_flags);
                    if (APIObjEnableShadowMapRenderingFn != NULL)
                        APIObjEnableShadowMapRenderingFn();
                }
                result = NuHGobjRndrMtxDwa(model->hierarchy, matrix, render_count, render_indices, output_matrices, dwa,
                                           render_flags);
                if (object != NULL && object->apiobj.surface_effect_count != 0) {
                    if (object->apiobj.surface_effect_count > 16)
                        object->apiobj.surface_effect_count = 16;
                    NUVEC points[16];
                    NuHGobjRndrRandShadowSurfacePoints(model->hierarchy, matrix, output_matrices,
                                                       object->apiobj.surface_effect_count, points, 0);
                    for (i32 i = 0; i < object->apiobj.surface_effect_count; ++i) {
                        AddVariableShotDebrisEffect(object->apiobj.surface_effect_id, &points[i], 1, 0, 0);
                    }
                    object->apiobj.surface_effect_count = 0;
                }
                if (reflection_matrix != NULL) {
                    model->hierarchy->data_0x198[8] = 1;
                    if (object == NULL || (object->apiobj.field_0x1f4 & 0x1000) == 0)
                        nurndr_force_lod = 1;
                    NuRndrStartReflectionRender(result);
                    NuHGobjRndrMtxDwa(model->hierarchy, reflection_matrix, render_count, render_indices,
                                      output_matrices, dwa, render_flags);
                    NuRndrEndReflectionRender();
                    if (object == NULL || (object->apiobj.field_0x1f4 & 0x1000) == 0)
                        nurndr_force_lod = 0;
                }
            }

            if (evaluate_only != 0)
                NuHGobjRestoreEvaluation();
        }
    cleanup:
        drawcharactermodel_nobsa = 0;
        drawcharactermodel_noani = 0;
        drawcharactermodel_restpose = 0;
        if (animation != NULL && drawcharactermodel_keepmergeaction == 0) {
            animation->frame = 0xffff;
        }
        drawcharactermodel_keepmergeaction = 0;
        return result;
    }

    void APITransparentCharDraw(nuhgobj_s *object, NUMTX *world_matrix, i32 render_count, i16 *render_indices,
                                NUMTX *joint_matrices, void **dwa, i32 render_flags) {
        i32 layer_six = 0;
        i32 layer_zero = 0;
        if (notransparentchardraw == 1 || APITrans_Mtl[0] == NULL) {
            return;
        }

        if (render_count > 1) {
            for (i32 i = 0; i < render_count; ++i) {
                if (render_indices[i] == 6) {
                    render_indices[i] = render_indices[render_count - 1];
                    layer_six = render_count - 1;
                    --render_count;
                }
                if (render_indices[i] == 0) {
                    render_indices[i] = render_indices[render_count - 1];
                    layer_zero = render_count - 1;
                    --render_count;
                }
            }
        }

        u8 previous_alpha_mode = object->data_0x198[8];
        object->data_0x198[8] = 1;
        NuSpecialConstAlpha(1, 0.0f);
        NuHGobjRndrMtxDwa(object, world_matrix, render_count, render_indices, joint_matrices, dwa, render_flags);
        NuSpecialConstAlpha(0, 0.0f);
        object->data_0x198[8] = previous_alpha_mode;
        if (layer_six != 0) {
            render_indices[layer_six] = 6;
        }
        if (layer_zero != 0) {
            render_indices[layer_zero] = 0;
        }
    }

    void APITransparentInit(void) {
        APITrans_Mtl[0] = NuMtlCreate3D(1);
        APITrans_Mtl[0]->attribs.unknown_1_1_2 = 1;
        APITrans_Mtl[0]->attribs.unknown_1_4_8 = 1;
        APITrans_Mtl[0]->diffuse_color.r = 0.0f;
        APITrans_Mtl[0]->diffuse_color.g = 0.0f;
        APITrans_Mtl[0]->diffuse_color.b = 0.0f;
        APITrans_Mtl[0]->opacity = 1.0f;
        APITrans_Mtl[0]->attribs.alpha_mode = 2;
        APITrans_Mtl[0]->attribs.z_mode = 0;
        APITrans_Mtl[0]->attribs.alpha_ref = 0;
        APITrans_Mtl[0]->sort_pri = 0x24;
        NuMtlUpdate(APITrans_Mtl[0]);

        APITrans_Mtl[1] = NuMtlCreate3D(1);
        APITrans_Mtl[1]->attribs.unknown_1_1_2 = 1;
        APITrans_Mtl[1]->attribs.unknown_1_4_8 = 1;
        APITrans_Mtl[1]->diffuse_color.r = 0.0f;
        APITrans_Mtl[1]->diffuse_color.g = 0.0f;
        APITrans_Mtl[1]->diffuse_color.b = 0.0f;
        APITrans_Mtl[1]->opacity = 1.0f;
        APITrans_Mtl[1]->attribs.alpha_mode = 2;
        APITrans_Mtl[1]->attribs.z_mode = 1;
        APITrans_Mtl[1]->attribs.alpha_ref = 0;
        APITrans_Mtl[1]->sort_pri = 0x24;
        NuMtlUpdate(APITrans_Mtl[1]);
    }

} // extern "C"
