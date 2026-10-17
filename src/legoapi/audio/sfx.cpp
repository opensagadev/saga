#include "legoapi/world/world_shared.h"
#include "legoapi/audio/audio.h"
#include "legoapi/audio/sfx.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/core/config/cheat.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/world/area.h"
#include "legoapi/world/mission.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/light/surfaces.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"
#include "nu2api/nufile/nufile.h"
#include "decomp.h"

#include <stdio.h>
#include <string.h>

#include "legoapi/audio/sfx_types.h"


DECOMP_ASSERT(sizeof(RepeatSfx) == 0x10, "RepeatSfx size");

static i32 repsfxcount;
static RepeatSfx repsfxtab[32];
static i32 ticktock;

extern "C" {
extern u16 GlobalSfxBits[100];
extern u16 SfxBits[100];
    SoundTable CurrentSFXTAB;
    void (*ExtraDieSfxFn)(GameObject_s *);
    void (*ExtraHurtSfxFn)(GameObject_s *);
    i32 CruiserD_LiftChase;
}

void PlayAMusic(i32 stream, i32 track, i32 volume, i32 one_shot);

extern "C" void NuGCutSetCutAudioStream(i32 stream);

extern "C" void PlaySfxById(i32 sfx_id, nuvec_s *position);
void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume);
void SetSfxBit_OnEx(i32);
void SetSfxBit_OffEx(i32);
void SetSfxBitTab_OnEx(SoundTable *, i32);
void SetSfxBitTab_OffEx(SoundTable *, i32);
i32 SfxBitEx(i32);
i32 SfxBitTabEx(const SoundTable *, i32);
void AddLevelSfxFromName(char *sfx_name, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count);
void AddLevelSfxGizmoSys(GIZMOSYS_s *gizmo_sys, void *world_info, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count);
void SetSpecialSfxBits(i32 *sfx_ids, i32 *sfx_count, WORLDINFO_s *world);
int SpecialSfxLoad(char *path, WORLDINFO_s *world);
void SetupBlowupSfx(WORLDINFO_s *world, specialsfx_s *special_sfx);
void Pulses_AddSfx(PULSESYS_s *system, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count);
void Move_BEAST(GameObject_s *object);
void PlayFootStepSfx(GameObject_s *object);
i32 qrand(void);
void GameCam_NewShake(GAMECAMERA_s *camera, f32 amount, f32 duration, f32 speed);
void GameCam_Judder(GAMECAMERA_s *camera, f32 amount, i32 axis, nuvec_s *source);
void NewRumbleAllPlayers(f32 intensity, f32 duration, i32 flags, i32 player_index);
i32 Players_AveragePos(nuvec_s *position, SOCKPOSITION_s *socket_position);
i32 Hub_Outside(void);
i32 KaminoInside(void);
i32 KaminoDiscoOn(void);
i32 DeathStarShieldDown(void);
i32 SarlaccPitDiscoActive(WORLDINFO_s *world);

extern AREADATA *BONUS_GUNSHIP_ADATA;
extern AREADATA *DAGOBAH_ADATA;
extern AREADATA *GUNSHIP_ADATA;
extern AREADATA *HOTHESCAPE_ADATA;
extern AREADATA *JABBASPALACE_ADATA;
extern AREADATA *NEWTOWN_ADATA;
extern AREADATA *PODRACE_ADATA;
extern AREADATA *PODSPRINT_ADATA;
extern AREADATA *SPEEDERCHASE_ADATA;
extern i16 id_CHEWBACCA;
extern i16 id_EWOK;
extern i16 id_GAMORREANGUARD;
extern i16 id_WICKET;
extern "C" f32 chattersfxwait;
extern "C" i32 DoubleScore;

static GAMEAUDIO GameAudio_Default;
static GAMEAUDIO *GameAudio = &GameAudio_Default;
extern "C" void MenuRegisterSoundFX(i32, i32, i32, i32);
i32 GameAudio_GetSfxId(i32);

extern "C" {
    void SetSfxBit_On(i32 sound);
    void SetSoundBitsById(const i32 *sound_ids, void (*set_bit)(i32));
    void SfxBitsStore(SoundTable *table);
    void MusicPreSeek(i32 track);
    void NuSound3FlushLoops(void);
    i32 NuSound3SetReverb(i32 mode);
}

extern "C" void ResetSounds(void);

extern "C" void PrepareSounds(const u16 *sounds);

extern "C" void MaskSounds(const u16 *mask);

void SetLevelSfxBits(WORLDINFO *world) {
    i32 sfx_ids[1024];
    i32 sfx_count = 0;
    i32 sound_id;

#define ADD_SFX(name) AddLevelSfxFromName(name, sfx_ids, &sfx_count, 0x400)
#define ADD_GAME_SFX(id) GameAudio_AddSfx(id, sfx_ids, &sfx_count, 0x400)

    for (i32 i = 0; i < world->level_sfx_count; ++i) {
        sfx_ids[sfx_count++] = world->level_sfx[i].id;
    }

    if (world->spinner_count > 0) {
        ADD_SFX("fly_paddle_rotate_lp");
        ADD_SFX("fly_paddle_stuck");
    }
    if (world->teleport_count > 0) {
        ADD_SFX("env_door_flap");
    }

    ADD_GAME_SFX(0x50);
    ADD_GAME_SFX(0x51);
    ADD_GAME_SFX(0x52);
    ADD_SFX("Grv_GuardWeaponLp");

    if (world->grabber != NULL) {
        if (world->current_level == CLOUDCITYTRAPA_LDATA) {
            ADD_SFX("CarbonFreezeCraneLp");
            ADD_SFX("CarbonFreezeCrane");
        } else if (world->current_level == JABBASPALACEB_LDATA) {
            ADD_SFX("env_hover_box_lp");
        } else {
            ADD_SFX("env_crane_mvt_lp");
        }
        ADD_SFX("env_crane_in");
        ADD_SFX("env_grabber_down");
        ADD_SFX("env_grabber_pickup");
        ADD_SFX("env_grabber_up");
        ADD_SFX("Explode2");
        if (world->current_level == CLOUDCITYESCAPEC_LDATA) {
            ADD_SFX("imp_C3PO_magnet");
            ADD_SFX("imp_C3PO_magnet_drop");
        }
    }

    if (world->cutscene_sys != NULL) {
        for (i32 i = 0; i < world->cutscene_sys->count; ++i) {
            CUTINFO *cut = world->cutscene_sys->cuts[i];
            // The original visits the six cutscene cues as separate checks.
            sound_id = cut->sfx[0].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[1].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[2].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[3].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[4].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
            sound_id = cut->sfx[5].id;
            if (sound_id != -1) {
                sfx_ids[sfx_count++] = sound_id;
            }
        }
    }

    Pulses_AddSfx(world->pulses_sys, sfx_ids, &sfx_count, 0x400);
    if (world->hat_machine_sys != NULL && world->hat_machine_sys->count > 0) {
        ADD_SFX("SwLever");
        ADD_SFX("HatOn");
    }
    if (world->nlevers > 0) {
        ADD_SFX("SwLever");
    }

    if (world->area != NULL && (world->area->flags & 1) != 0) {
        ADD_SFX("XWing_Torpedo");
        ADD_SFX("imp_proton_torp");
        ADD_SFX("XWing_LoopDeLoop");
    } else {
        ADD_SFX("Char_Slide_Lp");
        if (world->push_block_count > 0) {
            ADD_SFX("Block_Shove");
            ADD_SFX("Block_Push_Lp");
        }
        ADD_SFX("exp_thermalDet");
        ADD_SFX("imp_thermalDet_attach");
        if (world->area != NULL && world->area == SPEEDERCHASE_ADATA) {
            ADD_SFX("XWing_LoopDeLoop");
        }
    }
    ADD_SFX("Explode1");

    LEVELDATA *level = world->current_level;
    if (level != NULL) {
        if (level->unknown_0a2 != -1) {
            sfx_ids[sfx_count++] = level->unknown_0a2;
        }
    }
    if (level != NULL && (level == STATUS_LDATA || (level->flags & LEVEL_STATUS) != 0)) {
        ADD_SFX("StatusAward");
        ADD_SFX("Status_GoldBarDec");
        ADD_SFX("TrueJedi_100pc");
        ADD_SFX("TrueJedi_NOT");
        ADD_SFX("MK-Panel");
        ADD_SFX("Char_Icon_App");
        ADD_SFX("Char_Icon_Slide");
        ADD_SFX("LegoClicks");
        ADD_SFX("Shop_BuyCheat");
        ADD_SFX("Explode1");
        ADD_SFX("Jp_Ana_Jump");
        ADD_SFX("PickupCoin");
    } else if (world->area != NULL && (world->area->flags & 4) != 0) {
        ADD_SFX("Victory");
        ADD_SFX("exp_debris");
    } else if (level != NULL && level == CREDITS_LDATA) {
        ADD_SFX("StatusAward");
    }

    if (world->area != NULL) {
        if (world->area->super_counter_count != 0) {
            ADD_GAME_SFX(0x53);
        }
        if (world->area == PODRACE_ADATA || world->area == PODSPRINT_ADATA) {
            ADD_SFX("PodX_TuskenBlast");
            ADD_SFX("Pod_TuskHit");
            ADD_SFX("PodX_EngSebulba_Lp");
            ADD_SFX("PodX_EngAnakin_Lp");
            ADD_SFX("PodX_EngGeneric1_Lp");
            ADD_SFX("PodX_EngGeneric2_Lp");
            ADD_SFX("PodX_EngGeneric3_Lp");
            ADD_SFX("PodX_EngStartup1");
            ADD_SFX("PodX_EngStartup2");
            ADD_SFX("Pod_Race_Go");
            ADD_SFX("Pod_Race_Light");
            ADD_SFX("PodX_EngGeneric_Lp");
            ADD_SFX("PodX_Collide");
            ADD_SFX("PodX_Crash");
            ADD_SFX("PodX_Booster");
            ADD_SFX("CountdownTimerTick");
            ADD_SFX("CountdownTimerTock");
            ADD_SFX("PodX_PurpCrysHit");
        } else if (world->area == GUNSHIP_ADATA || world->area == BONUS_GUNSHIP_ADATA) {
            ADD_SFX("GC_GunshipBlasterFire");
            ADD_SFX("GC_GunshipEngineLp");
            ADD_SFX("GC_GunshipDeath");
            ADD_SFX("GC_LaserBeam");
            ADD_SFX("Explode1");
        } else if (world->area == HUB_ADATA) {
            ADD_SFX("Shop_BuyCheat");
            ADD_SFX("Shop_NotEnufMuny");
            ADD_SFX("ui_ZoomIn");
            ADD_SFX("ui_ZoomOut");
            ADD_SFX("ui_hover_lp");
        } else if (world->area == JABBASPALACE_ADATA) {
            ADD_SFX("swdisco");
        } else if (world->area == NEWTOWN_ADATA) {
            ADD_SFX("BBounce");
        }
    }

    level = world->current_level;
    if (level != NULL) {
        if (level == NEGOTIATIONSA_LDATA) {
            ADD_SFX("NegA_CoinMDoorUp");
            ADD_SFX("NegA_CoinMDoorDn");
        } else if (level == NEGOTIATIONSB_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("NegC_Crate");
        } else if (level == NEGOTIATIONSC_LDATA) {
            ADD_SFX("NegC_MagnetCoil");
            ADD_SFX("NegC_MagnetCoilH");
            ADD_SFX("NegC_MagnetCoilS");
            ADD_SFX("FField");
            ADD_SFX("NegC_Crate");
        } else if (level == GUNGAN_A_LDATA) {
            ADD_SFX("GunA_TreeFall");
            ADD_SFX("FS_CreaRun");
            ADD_SFX("IkopiGrowl");
            ADD_SFX("KaaduBark");
            ADD_SFX("KaaduGrowl");
        } else if (level == RESCUEC_LDATA) {
            ADD_SFX("ResX_MarbColLp");
            ADD_SFX("ResX_PoleLift_Rise");
            ADD_SFX("ResX_PoleLift_Lower");
        } else if (level == RETAKEB_LDATA) {
            ADD_SFX("ResX_MarbColLp");
        } else if (level == RETAKED_LDATA) {
            ADD_SFX("ResX_PortCulLp");
            ADD_SFX("ResX_PortCulEnd");
            ADD_SFX("Ep1_5_OutdoorPlatform");
        } else if (level == RETAKEE_LDATA) {
            ADD_SFX("Ep1_5_OutdoorPlatform");
            ADD_SFX("ResX_PortCulLp");
            ADD_SFX("ResX_PortCulEnd");
        } else if (level == MAULA_LDATA) {
            ADD_SFX("Ep1_7_BDroidPlatf");
            ADD_SFX("Ep1_7_SFPowerUp");
            ADD_SFX("Ep1_7_SFHoverLp");
            ADD_SFX("Ep1_7_SFTakeOff");
        } else if (level == MAULD_LDATA) {
            ADD_SFX("Ep1_7_FloatPlatRise");
            ADD_SFX("NegC_Crate");
        } else if (level == KAMINOE_LDATA) {
            ADD_SFX("Kam_Slave1BlasterFire");
            ADD_SFX("Slave1_EngineLp");
        } else if (level == KAMINOC_LDATA) {
            ADD_SFX("Kam_DiscoFloorPanelOn");
            ADD_SFX("Kam_DiscoFloorPanelDone");
            ADD_SFX("Kam_SparkLp");
            ADD_SFX("Kam_SparkLp");
            ADD_SFX("Kam_ForceFieldLp");
            ADD_SFX("Kam_ForceFieldOff");
        } else if (level == FACTORYB_LDATA) {
            ADD_SFX("FacB_BeltLp");
            ADD_SFX("FacB_ConvStop");
            ADD_SFX("Fac_StampHydros");
            ADD_SFX("Fac_StampImpacts");
            ADD_SFX("Fac_BonusCylUp");
            ADD_SFX("Fac_BonusBeep");
        } else if (level == FACTORYD_LDATA) {
            ADD_SFX("Fac_BucketGearUD");
            ADD_SFX("Fac_BucketAwayFB");
            ADD_SFX("Fac_BucketLp");
            ADD_SFX("Fac_BucketArriveLock");
            ADD_SFX("Fac_BucketOpens");
        } else if (level == FACTORYF_LDATA) {
            ADD_SFX("Fac_LaserGateLoop");
        } else if (level == FACTORYG_LDATA) {
            ADD_SFX("Fac_ObiRestraintLoop");
        } else if (level == GUNSHIPB_LDATA) {
            ADD_SFX("CountdownTimerTick");
            ADD_SFX("CountdownTimerTock");
        } else if (level == DOOKUC_LDATA) {
            ADD_SFX("Dooku_LightningLp");
        } else if (level == DOGFIGHTA_LDATA) {
            ADD_SFX("Ep3_1_StarDestEngLp");
            ADD_SFX("Ep3_1_ProtoXWingMissile");
            ADD_SFX("Ep3_1_ExplosionM");
            ADD_SFX("Ep3_1_ExplosionXXL");
            ADD_SFX("Dog_TurretFire");
            ADD_SFX("Dog_CloneARC170Gun");
            ADD_SFX("Dog_JediStFighterGun");
            ADD_SFX("Dog_JediStFighterGun2");
            ADD_SFX("Dog_JediStFighterGun3");
            ADD_SFX("Dog_JediStFighterEngLp");
            ADD_SFX("Dog_CloneARC170EngLp");
            ADD_SFX("Dog_DroidFighterBlast");
            ADD_SFX("Dog_TriFighterGuns");
            ADD_SFX("Dog_TriFighterGuns2");
            ADD_SFX("Dog_DroidFighterEngLp");
            ADD_SFX("Dog_TriFighterEngLp");
            ADD_SFX("Dog_DroidFighterHit");
            ADD_SFX("Dog_TriFighterHit");
            ADD_SFX("Dog_HugeBeamGunFire");
            ADD_SFX("Dog_HugeBeamGunLp");
            ADD_SFX("Dog_SepShieldPwrDown");
            ADD_SFX("Dog_ShipBreakExplo");
            ADD_SFX("Dog_ShipDoorOpen");
            ADD_SFX("Dog_StDestTurretSpins");
            ADD_SFX("Dog_TowerBreakExplo");
        } else if (level == CRUISERD_LDATA) {
            ADD_SFX("Cru_HugeWallMoveLp");
        } else if (level == CRUISERG_LDATA) {
            ADD_SFX("NegA_CoinMDoorUp");
            ADD_SFX("NegA_CoinMDoorDn");
        } else if (level == KASHYYYKD_LDATA) {
            ADD_SFX("Kas_BoulderLoop");
            ADD_SFX("Kas_BoulderExplo");
        } else if (level == VADERA_LDATA) {
            ADD_SFX("CountdownTimerTick");
            ADD_SFX("CountdownTimerTock");
        } else if (level == TATOOINEA_LDATA) {
            ADD_SFX("env_shower_lp");
        } else if (level == TATOOINEB_LDATA) {
            ADD_SFX("greenlighton");
            ADD_SFX("env_suckerspit_3PO");
            ADD_SFX("env_suckerspit_gonk");
            ADD_SFX("conveyorlp");
        } else if (level == MOSEISLEYA_LDATA) {
            ADD_SFX("Landspeeder_EngineLp");
            ADD_SFX("env_jacuzzi_lp");
        } else if (level == MOSEISLEYC_LDATA) {
            ADD_SFX("Droid_BeamLp");
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
            ADD_SFX("SwPPad");
        } else if (level == DEATHSTARRESCUEB_LDATA) {
            ADD_SFX("env_tractorbeam_off");
            ADD_SFX("env_tractorbeam_lp");
        } else if (level == DEATHSTARRESCUEC_LDATA) {
            ADD_SFX("ffield");
        } else if (level == DEATHSTARRESCUEE_LDATA) {
            ADD_SFX("Turret_PitchLp");
            ADD_SFX("Turret_YawLp");
            ADD_SFX("Turbo_Fire");
        } else if (level == DEATHSTARESCAPEA_LDATA) {
            ADD_SFX("Dianoga_Groan");
            ADD_SFX("Dianoga_Roar");
        } else if (level == DEATHSTARESCAPED_LDATA) {
            ADD_SFX("Dianoga_Groan");
            ADD_SFX("Dianoga_Roar");
        } else if (level == DEATHSTARESCAPEB_LDATA) {
            ADD_SFX("SqueakWash");
        } else if (level == DEATHSTARESCAPEC_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLEA_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLEB_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLEC_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTARBATTLED_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == HOTHESCAPEA_LDATA) {
            ADD_SFX("ThermalDet_Beep");
            ADD_SFX("env_padLight_on");
        } else if (level == HOTHESCAPEC_LDATA) {
            ADD_SFX("env_padLight_on");
        } else if (level == ASTEROIDCHASED_LDATA) {
            ADD_SFX("exp_asteroid");
        } else if (level == CLOUDCITYTRAPA_LDATA) {
            ADD_SFX("Env_Steam_Lp");
        } else if (level == CLOUDCITYESCAPEA_LDATA) {
            ADD_SFX("env_steam_lp");
            ADD_SFX("Env_ctrl_desk_on");
        } else if (level == CLOUDCITYESCAPEC_LDATA) {
            ADD_SFX("env_steam_lp");
        } else if (level == DEATHSTAR2BATTLEB_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == DEATHSTAR2BATTLED_LDATA) {
            ADD_SFX("ForceLightningLp");
        } else if (level == JABBASPALACEA_LDATA) {
            ADD_SFX("SwDisco");
            ADD_SFX("Leia_Blaster");
        } else if (level == JABBASPALACEB_LDATA) {
            ADD_SFX("SwDisco");
            ADD_SFX("Leia_Blaster");
        } else if (level == JABBASPALACED_LDATA) {
            ADD_SFX("SwDisco");
            ADD_SFX("Leia_Blaster");
        } else if (level == JABBASPALACEE_LDATA) {
            ADD_SFX("exp_minecart");
        } else if (level == SARLACCPITB_LDATA) {
            ADD_SFX("env_curtain_lp");
            ADD_SFX("Kam_DiscoFloorPanelOn");
            ADD_SFX("Kam_DiscoFloorPanelDone");
        } else if (level == SARLACCPITC_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
            ADD_SFX("imp_c3po_magnet_drop");
            ADD_SFX("env_magnet_on");
        } else if (level == SPEEDERCHASEA_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == ENDORBATTLEA_LDATA) {
            ADD_SFX("env_lantern_lp");
        } else if (level == ENDORBATTLEB_LDATA) {
            ADD_SFX("waterfall");
            ADD_SFX("drd_r2_mvt_water_lp");
            ADD_SFX("env_lantern_lp");
        } else if (level == ENDORBATTLED_LDATA) {
            ADD_SFX("FField");
            ADD_SFX("FFieldOff");
        } else if (level == EMPERORFIGHTA_LDATA) {
            ADD_SFX("env_block_light_on");
            ADD_SFX("env_padlight_on");
        }
    }

    if (VehicleArea != 0) {
        ADD_SFX("TieDoorsOpen");
    }
    if (Mission_Active(NULL) != NULL) {
        ADD_SFX("Victory");
    }
    if (Arcade != 0) {
        ADD_SFX("env_padLight_on");
    }
    if (world->area != NULL) {
        if (world->area == HOTHESCAPE_ADATA || world->area == JABBASPALACE_ADATA) {
            ADD_SFX("fs_ice");
        } else if (world->area == DAGOBAH_ADATA) {
            ADD_SFX("fs_swamp");
        }
    }
    if (apicharsys->loaded_model_count > 0 && world->current_level != NULL && (world->current_level->flags & 2) != 0 &&
        VehicleArea == 0) {
        ADD_GAME_SFX(0x14);
        ADD_GAME_SFX(0x15);
    }

    for (i32 character_id = 0; character_id < apicharsys->character_count; ++character_id) {
        i16 model_id = apicharsys->playermodelids[character_id];
        if (model_id != -1 && (apicharsys->models[model_id].flags & 1) != 0) {
            CHARACTERDATA *character = &CDataList[character_id];
            GAMECHARACTERDATA *game_character = static_cast<GAMECHARACTERDATA *>(character->field11_0x24);

            {
                const i16 configured_sfx = game_character->sfx_die;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                } else if ((character->model_flags & 0x44002010) == 0) {
                    ADD_GAME_SFX((static_cast<i32>(game_character->flags_090 << 14) >> 31) + 0x1c);
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_hurt;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                } else if ((character->model_flags & 0x44002010) == 0) {
                    ADD_GAME_SFX((static_cast<i32>(game_character->flags_090 << 14) >> 31) + 0x17);
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_grunt;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                } else if ((character->model_flags & 0x44002010) == 0) {
                    ADD_GAME_SFX((static_cast<i32>(game_character->flags_090 << 14) >> 31) + 0x15);
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_engine;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_shoot;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_footstep;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_chatter;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            {
                const i16 configured_sfx = game_character->sfx_sabre;
                if (configured_sfx != -1) {
                    sfx_ids[sfx_count++] = configured_sfx;
                }
            }
            for (i32 i = 0; i < 6 && game_character->sfx_misc[i] != -1; ++i) {
                sfx_ids[sfx_count++] = game_character->sfx_misc[i];
            }

            if ((character->model_flags & 0x40) != 0) {
                ADD_SFX("drd_r2_scope_up");
                ADD_SFX("drd_r2_scope_down");
                ADD_SFX("drd_r2_mvt_water_lp");
            }
            if ((game_character->flags_090 & 0x400) != 0) {
                ADD_SFX("TowCable_Fire");
                ADD_SFX("TowCable_Latch");
                ADD_SFX("TowCable_Detach");
                ADD_SFX("TowCable_Snap");
            }
            if ((character->model_flags & 0x2000) != 0) {
                ADD_SFX("XWing_Torpedo");
                ADD_SFX("env_tractorbeam_lp");
                if ((character->model_flags & 0x04000000) == 0) {
                    ADD_SFX("Explode1");
                }
            }
            if (character_id == id_CHEWBACCA) {
                ADD_SFX("C3_Hurt");
                ADD_SFX("C3_Death");
            }
            if (character->move_fn == Move_BEAST) {
                ADD_SFX("Lego_Poo");
                ADD_SFX("Lego_PLOP");
                ADD_SFX("FliesLp");
            }
            if (game_character->uses_weapon_action == 10) {
                ADD_SFX("veh_tie_by");
                ADD_SFX("Tie_Spins");
            }
            if (character_id == id_EWOK || character_id == id_WICKET) {
                ADD_SFX("wpn_bomb_drop");
                ADD_SFX("exp_bomb");
            }
            if ((game_character->flags_090 & 4) != 0) {
                ADD_SFX("ForceLightningLp");
            } else if ((game_character->flags_090 & 2) != 0) {
                ADD_SFX("ForceChokeCrunch");
            } else if ((character->model_flags & 8) != 0) {
                ADD_SFX("ForceMindTrick");
            }
            if (game_character->uses_weapon_action == 12 && (character->model_flags & 8) != 0 &&
                id_GAMORREANGUARD != -1 && apicharsys->playermodelids[id_GAMORREANGUARD] != -1) {
                ADD_SFX("ForceChokeCrunch");
            }
            if ((game_character->flags_094[3] & 0x20) != 0) {
                ADD_GAME_SFX(0x4c);
                ADD_GAME_SFX(0x4d);
                ADD_GAME_SFX(0x4e);
            }

            CHARACTER_EFFECT_s *effect = character->effects;
            if (effect != NULL) {
                while (effect->character_id != -1) {
                    if (effect->sound_id != -1) {
                        sfx_ids[sfx_count++] = effect->sound_id;
                    }
                    ++effect;
                }
            }
            if ((character->model_flags & 0x20) != 0) {
                ADD_SFX("TC14_VLA");
                ADD_SFX("TC14_VLN");
            }
            if ((character->model_flags & 0x40) != 0) {
                ADD_SFX("R2D2_VLA");
            }
        }
    }

    i32 double_score_sfx = Arcade != 0;
    if (!double_score_sfx) {
        level = world->current_level;
        double_score_sfx = level != NULL && (level->flags & 0x800) != 0;
    }
    if (!double_score_sfx) {
        i32 portal = 0x13;
        while (!double_score_sfx && portal <= 0x17) {
            if (world->portal_places[portal] != NULL) {
                double_score_sfx = 1;
            } else {
                ++portal;
            }
        }
    }
    if (double_score_sfx) {
        ADD_SFX("ui_DoubleScoreEntry");
        ADD_SFX("ui_DoubleScoreText");
    }

    AddLevelSfxGizmoSys(world->gizmo_sys, world, sfx_ids, &sfx_count, 0x400);
    SetSpecialSfxBits(sfx_ids, &sfx_count, world);
    sfx_ids[sfx_count] = -1;
    if (sfx_count > 0) {
        SetSoundBitsById(sfx_ids, SetSfxBit_On);
    }
    SfxBitsStore(&CurrentSFXTAB);

#undef ADD_GAME_SFX
#undef ADD_SFX
}
void ResetLevSfx(WORLDINFO *world) {
    for (i32 i = 0; i < 0x40; i++) {
        world->level_sfx[i].id = -1;
    }
    world->level_sfx_count = 0;
}

bool InitSpecialSfx(WORLDINFO *world) {
    bool result = false;
    if (world != NULL) {
        world->special_sfx_count = 0;
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 4);
        world->special_sfx = reinterpret_cast<specialsfx_s *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += 0xf00;
        memset(world->special_sfx, 0, 0xf00);

        world->special_sfx_event_count = 0;
        world->giz_buffer.addr = ALIGN(world->giz_buffer.addr, 0x10);
        world->special_sfx_events = reinterpret_cast<SPECIALSFXEVENT_s *>(world->giz_buffer.void_ptr);
        world->giz_buffer.addr += 0xc00;
        memset(world->special_sfx_events, 0, 0xc00);
        result = true;
    }
    return result;
}
void LoadSpecialSfxFile(WORLDINFO *world) {
    char path[268];
    sprintf(path, "%s.sfx", world->config_file);
    if (NuFileExists(path) != 0 && SpecialSfxLoad(path, world) != 0) {
        i32 count = world->special_sfx_count;
        specialsfx_s *entry = world->special_sfx;
        if (count > 0) {
            i32 i = 0;
            do {
                const i32 flags = entry->flags & 0xf;
                if (flags == 1) {
                    SetupBlowupSfx(world, entry);
                }
                ++i;
                count = world->special_sfx_count;
                ++entry;
            } while (i < count);
        }
    }
}

extern "C" {

    f32 sfx_wait;

i32 GetSfxIdN(char *name, i32 length);

char *GetSfxName(i32 sfx_id);

i32 IsSfxLooping(i32 sfx_id);

void PauseGameAudio(i32);

void PauseGameMusic(void);

void PauseGameSfx(void);

void PlayAltGameMusic(i32 track);

i32 PlayCutMusic(i32 track, i32 state, void *context);

void PlayMusic(i32 track, i32 mode);

void PlaySfx(char *name, struct nuvec_s *position);

void PlaySfxAndSetPitch(char *name, nuvec_s *position, f32 pitch);

void PlaySfxAndSetVolume(char *name, nuvec_s *position, f32 volume);

void PlaySfxAndSetVolumeAndPitch(char *name, nuvec_s *position, f32 volume, f32 pitch);

void PlaySfxById(i32 sfx_id, nuvec_s *position);

void PlaySfxByIdAndSetPitch(i32 sfx_id, nuvec_s *position, f32 pitch);

void PlaySfxByIdAndSetVolume(i32 sfx_id, nuvec_s *position, f32 volume);

void PlaySfxByIdAndSetVolumeAndPitch(i32 sfx_id, nuvec_s *position, f32 volume, f32 pitch);

i32 PlayingCutMusic(void);

void ClearLinkedCutSceneMusic(void *context);

void PrepareAllSounds(void);

void RegisterSounds(SoundTable *table);

void ResetPreSeek(void);

void RestoreGameMusic(void);

void ResumeGameAudio(void);

    void SOUND_SFXRequest_Table(void) {
        // Platform placeholder in the Android build.
    }

void SetAudioFadeLevel(f32 level);

void SetCutVolume(f32 volume);

void SetLinkedCutSceneMusic(void *context, i32 state);

void SetMusicVolume(f32 volume);

void SetPreSeekStartPoint(f32 start_point);

void SetSfxBitTab_Off(SoundTable *table, i32 sound);

void SetSfxBitTab_On(SoundTable *table, i32 sound);

void SetSfxBit_Off(i32 sound);

void SetSfxBit_On(i32 sound);

i32 SfxBit(i32 sound);

void SfxBitMaskTable(u16 *bits, const u16 *mask);

i32 SfxBitTab(const SoundTable *table, i32 sound);

void SfxBitsRestore(SoundTable *table);

void SfxBitsSetAll(u16 *bits);

void SfxBitsStore(SoundTable *table);

void StopAltGameMusic(void);

void SwapMusic(i32 mode);

} // extern "C"

void PlayDieSfx(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    GAMECHARACTERDATA_s *config = character->game_character;
    i32 sfx;
    {
        const i16 configured_sfx = config->sfx_die;
        if (configured_sfx != -1) {
            sfx = configured_sfx;
            goto play;
        }
    }
    {
        sfx = -1;
        if ((config->flags_090 & 0x800) != 0) {
            sfx = GameAudio->sfx_ids[0x18];
        } else {
            const u32 flags = character->model_flags;
            if ((flags & 0x2000) != 0) {
                sfx = GameAudio->sfx_ids[0x18];
            } else if ((flags & 0x04000000) != 0) {
                sfx = GameAudio->sfx_ids[0x19];
            } else if ((flags & 0x10) != 0) {
                sfx = GameAudio->sfx_ids[0x1a];
            } else if ((flags & 0x40000000) == 0) {
                if ((object->field_0xf01 & 8) != 0) {
                    sfx = GameAudio->sfx_ids[0x1b];
                } else {
                    sfx = GameAudio->sfx_ids[0x1c];
                }
            }
        }
        if (sfx == -1) {
            goto extra;
        }
    }
play:
    GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, 0, 0);
extra:
    auto extra_sfx = ExtraDieSfxFn;
    if (extra_sfx != NULL) {
        extra_sfx(object);
    }
}

void PlayHurtSfx(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    i32 sfx;
    {
        const i16 configured_sfx = character->game_character->sfx_hurt;
        if (configured_sfx != -1) {
            sfx = configured_sfx;
            goto selected;
        }
    }
    if ((character->model_flags & 0x44002010) != 0) {
        goto extra;
    }
    if ((object->field_0xf01 & 8) != 0) {
        sfx = GameAudio->sfx_ids[0x16];
    } else {
        sfx = GameAudio->sfx_ids[0x17];
    }
selected:
    if (sfx == -1) {
        goto extra;
    }
    GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, 0, 0);
extra:
    auto extra_sfx = ExtraHurtSfxFn;
    if (extra_sfx != NULL) {
        extra_sfx(object);
    }
}

void PlayJumpSfx(GameObject_s *object, i32 type) {
    i32 sfx;
    const u32 flags = object->apiobj.character_data->model_flags;
    if ((flags & 0x40) != 0) {
        sfx = GameAudio->sfx_ids[1];
    } else if ((flags & 8) != 0) {
        switch (type) {
            case 0:
                sfx = GameAudio->sfx_ids[2];
                break;
            case 1:
                sfx = GameAudio->sfx_ids[3];
                break;
            case 2:
                sfx = GameAudio->sfx_ids[6];
                break;
            case 3:
                sfx = GameAudio->sfx_ids[4];
                break;
            case 4:
                sfx = GameAudio->sfx_ids[5];
                break;
            default:
                return;
        }
    } else {
        sfx = GameAudio->sfx_ids[0];
    }
    if (sfx != -1) {
        if (!object->apiobj.player_controlled && (object->field_0xefb & 8) == 0) {
            PlaySfxByIdAndSetVolume(sfx, &object->apiobj.lower_position, 0.5f);
        } else {
            GameAudio_PlaySfxById(sfx, &object->apiobj.lower_position, 0, 1);
        }
    }
}

void PlayLandSfx(GameObject_s *object, i32 type, i32) {
    i32 sfx;
    if (type == 1) {
        if ((object->apiobj.character_data->model_flags & 8) == 0) {
            return;
        }
        sfx = GameAudio->sfx_ids[0xb];
    } else if (type == 2) {
        sfx = GameAudio->sfx_ids[0xc];
    } else if (type == 3) {
        sfx = GameAudio->sfx_ids[0xd];
    } else if (type == 4) {
        sfx = GameAudio->sfx_ids[0xe];
    } else {
        if (object->apiobj.is_underwater != 0 || object->apiobj.intersects_water != 0) {
            return;
        }
        const i32 alternate = WorldInfo_CurrentlyActive()->current_level->flags & 0x1000;
        if ((object->apiobj.character_data->model_flags & 0x10) == 0) {
            if (type != 0) {
                return;
            }
            if (alternate != 0) {
                sfx = GameAudio->sfx_ids[8];
            } else {
                sfx = GameAudio->sfx_ids[7];
            }
        } else {
            if (alternate != 0) {
                sfx = GameAudio->sfx_ids[10];
            } else {
                sfx = GameAudio->sfx_ids[9];
            }
        }
    }
    if (sfx != -1) {
        if (static_cast<i8>(object->apiobj.flags_low) < 0 || (object->field_0xefb & 8) != 0) {
            GameAudio_PlaySfxById(sfx, &object->apiobj.lower_position, 0, 1);
        } else {
            PlaySfxByIdAndSetVolume(sfx, &object->apiobj.lower_position, 0.5f);
        }
    }
}

i32 SfxBitTabEx(SoundTable const *table, i32 sound);

void TickTockSfx() {
    if (ticktock != 0) {
        GameAudio_PlaySfx(0x1d, NULL, 0, 0);
        ticktock = 0;
    } else {
        GameAudio_PlaySfx(0x1e, NULL, 0, 0);
        ticktock = 1;
    }
}

void AddFootSteps(GameObject_s *object) {
    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if (packet.blending != 0 || packet.animation_index == -1)
        return;
    CHARACTERMODEL_s *model = object->apiobj.character_model;
    const i32 animation = packet.animation_index;
    if (model->model_data_b[animation] == NULL)
        return;
    CHARACTERANIM_s *config = static_cast<CHARACTERANIM_s *>(model->model_data_a[animation]);
    if ((config->flags & CHARACTER_ANIMATION_FLAG_FOOTSTEPS) == 0 || packet.previous_time == packet.current_time)
        return;
    const i32 advancing = (packet.flags & ANIMPACKET_FLAG_LOOPED) != 0 ? packet.previous_time > packet.current_time
                                                                       : packet.current_time > packet.previous_time;
    bool crossed = false;
    if ((config->flags & 0x10000) == 0) {
        if (!(packet.previous_time > packet.current_time)) {
            if (!advancing)
                return;
            for (i32 i = 2; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (frame >= 1.0f && packet.current_time >= frame && frame > packet.previous_time)
                    crossed = true;
            }
        } else {
            if (!advancing)
                return;
            for (i32 i = 2; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (frame >= 1.0f && (packet.current_time >= frame || frame > packet.previous_time))
                    crossed = true;
            }
        }
    } else {
        if (!advancing)
            return;
        if (!(packet.previous_time > packet.current_time)) {
            for (i32 i = 0; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (frame >= 1.0f && packet.current_time >= frame && frame > packet.previous_time)
                    crossed = true;
            }
        } else {
            for (i32 i = 0; i < 4; ++i) {
                const f32 frame = config->event_frames[i];
                if (frame >= 1.0f && (packet.current_time >= frame || frame > packet.previous_time))
                    crossed = true;
            }
        }
    }
    if (crossed)
        PlayFootStepSfx(object);
}

void PlayGruntSfx(GameObject_s *object) {
    CHARACTERDATA *character = object->apiobj.character_data;
    const i16 configured_sfx = character->game_character->sfx_grunt;
    i32 sfx = configured_sfx;
    if (configured_sfx == -1) {
        if ((character->model_flags & 0x44002010) != 0) {
            return;
        }
        if ((object->field_0xf01 & 8) != 0) {
            sfx = GameAudio->sfx_ids[0x14];
        } else {
            sfx = GameAudio->sfx_ids[0x15];
        }
    }
    if (sfx != -1) {
        GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, 0, 0);
    }
}

void PlaySabreSfx(char *name, GameObject_s *object, nuvec_s *position, i32) {
    const u8 player = static_cast<u8>(object->apiobj.field_0x27c);
    if (player == 0xff && WORLD->rooms_visible_ptr[object->room_id] == 0) {
        return;
    }
    u32 player_bits = 0;
    if (object->apiobj.player_controlled) {
        player_bits = 1u << (player & 0x1f);
    }
    if (name == NULL) {
        const i16 sfx = object->apiobj.character_data->game_character->sfx_sabre;
        if (sfx == -1) {
            GameAudio_PlaySfx(0x40, &object->apiobj.collision_position, 0, 1);
        } else {
            GameAudio_PlaySfxById(sfx, &object->apiobj.collision_position, player_bits, 1);
        }
    } else {
        const i32 named_sfx = GetSfxId(name);
        if (position == NULL) {
            position = &object->apiobj.collision_position;
        }
        GameAudio_PlaySfxById(named_sfx, position, 0, 1);
    }
}

void LevChatterSfx(char *name, nuvec_s *position) {
    if (chattersfxwait <= 0.0f && qrand() < 0x400) {
        PlaySfx(name, position);
        const i32 random = qrand();
        chattersfxwait = static_cast<f32>(random) * (1.0f / 65535.0f) * 2.0f + 3.0f;
    }
}

void PlayRepeatSfx(char *name, i32 sfx_id, f32 initial_delay, char play_count, f32 interval, nuvec_s *position) {
    if (play_count == 1 && initial_delay == 0.0f) {
        if (sfx_id == -1) {
            PlaySfx(name, position);
        } else {
            GameAudio_PlaySfxById(sfx_id, position, 0, 0);
        }
        return;
    }

    const i32 sound_slot = repsfxcount;
    repsfxtab[sound_slot].state = REPEAT_SFX_PLAY;
    if (initial_delay > 0.0f) {
        repsfxtab[sound_slot].state = REPEAT_SFX_INITIAL_DELAY;
    }

    if (sfx_id == -1)
        sfx_id = GetSfxId(name);

    repsfxtab[sound_slot].sfx_id = static_cast<i16>(sfx_id);
    RepeatSfx &repeat = repsfxtab[repsfxcount];
    repsfxcount = (repsfxcount + 1) & 31;
    repeat.timer = initial_delay;
    repeat.plays_remaining = play_count;
    repeat.interval = interval;
    repeat.position = position;
}

void ResetRepeatSfx() {
    repsfxcount = 0;
    memset(repsfxtab, 0, sizeof(repsfxtab));
    repsfxtab[0].sfx_id = -1;
}

void SetSfxBit_OnEx(i32 sound);

void UpdateLevelSfx(WORLDINFO_s *world, i32 paused) {
    SfxBitsRestore(&CurrentSFXTAB);

    if (paused != 0) {
        goto disable_reverb_and_return;
    }
    if (GameAudio->check_reverb_fn != NULL && GameAudio->check_reverb_fn() != 0) {
        goto enable_reverb;
    }
    NuSound3SetReverb(0);

update_ambient:
    if (CUTSTOPGAME == 0 && world->current_level->unknown_0a2 != -1) {
        GameAudio_PlaySfxById(world->current_level->unknown_0a2, NULL, 0, 0);
    }
    return;

disable_reverb_and_return:
    NuSound3SetReverb(0);
    return;

enable_reverb:
    NuSound3SetReverb(1);
    goto update_ambient;
}

void PlayFootStepSfx(GameObject_s *object) {
    i32 on_platform = 1;
    if (object->field_0x1078 == -1 && object->apiobj.supporting_platform_id == -1) {
        const i8 surface = static_cast<i8>(object->apiobj.field_0x281);
        if (surface == -1 || (TerSurface[surface].flags & 2) == 0) {
            on_platform = 0;
        }
    }

    i32 sfx = object->apiobj.character_data->game_character->sfx_footstep;
    if (sfx == -1) {
        if (GameAudio->override_footstep_fn != NULL) {
            sfx = GameAudio->override_footstep_fn(object, on_platform);
        }
        if (sfx == -1) {
            if (object->apiobj.is_underwater == 0 && object->apiobj.intersects_water == 0) {
                const i32 alternate = WorldInfo_CurrentlyActive()->current_level->flags & 0x1000;
                if ((object->apiobj.character_data->model_flags & 0x10) != 0) {
                    if (alternate != 0) {
                        sfx = GameAudio->sfx_ids[0x12];
                    } else {
                        sfx = GameAudio->sfx_ids[0x11];
                    }
                } else {
                    if (alternate != 0) {
                        sfx = GameAudio->sfx_ids[0x10];
                    } else {
                        sfx = GameAudio->sfx_ids[0xf];
                    }
                }
            } else {
                sfx = GameAudio->sfx_ids[0x13];
            }
        }
    }

    if (sfx != -1) {
        if (object->apiobj.player_controlled || (object->field_0xefb & 8) != 0) {
            GameAudio_PlaySfxById(sfx, &object->apiobj.lower_position, 0, 0);
        } else {
            PlaySfxByIdAndSetVolume(sfx, &object->apiobj.lower_position, 0.5f);
        }
    }

    ANIMPACKET_s &packet = object->apiobj.anim_packet;
    if (packet.blending != 0 || packet.animation_index == -1)
        return;
    CHARACTERANIM_s *config =
        static_cast<CHARACTERANIM_s *>(object->apiobj.character_model->model_data_a[packet.animation_index]);
    if (config == NULL)
        return;
    if ((config->flags & 0x20000) != 0) {
        GameCam_Judder(GameCam, -0.25f, 0, &object->apiobj.collision_position);
        NewRumbleAllPlayers(0.6f, 0.0f, 0, 0);
    } else if ((config->flags & 0x40000) != 0 && VehicleArea == 0) {
        GameCam_NewShake(GameCam, 0.5f, 0.5f, 1.0f);
        NewRumbleAllPlayers(0.5f, 0.0f, 0, 0);
    }
}

void SetSfxBit_OffEx(i32 sound);

void UpdateRepeatSfx() {
    for (RepeatSfx &repeat : repsfxtab) {
        switch (repeat.state) {
            case REPEAT_SFX_INITIAL_DELAY:
                if (repeat.timer > 0.0f) {
                    repeat.timer -= FRAMETIME;
                } else {
                    repeat.state = REPEAT_SFX_PLAY;
                }
                break;

            case REPEAT_SFX_PLAY:
                GameAudio_PlaySfxById(repeat.sfx_id, repeat.position, 0, 0);
                --repeat.plays_remaining;
                if (repeat.plays_remaining > 0) {
                    repeat.timer = repeat.interval;
                    repeat.state = REPEAT_SFX_INTERVAL;
                } else {
                    memset(&repeat, 0, sizeof(repeat));
                }
                break;

            case REPEAT_SFX_INTERVAL:
                if (repeat.timer > 0.0f) {
                    repeat.timer -= FRAMETIME;
                } else {
                    repeat.state = REPEAT_SFX_PLAY;
                }
                break;

            default:
                break;
        }
    }
}

void AddLevelSfxFromId(i32 sfx_id, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx || sfx_id == -1) {
        return;
    }
    for (i32 index = 0; index < *sfx_count; ++index) {
        if (sfx_ids[index] == sfx_id) {
            return;
        }
    }
    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void SetSfxBitTab_OnEx(SoundTable *table, i32 sound);

void SetSfxBitTab_OffEx(SoundTable *table, i32 sound);

void SfxCheckMusicOnOff(OPTIONSSAVE_s *) {
    // Platform placeholder in the Android build.
}

void AddLevelSfxFromName(char *sfx_name, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count) {
    if (sfx_count == NULL || sfx_ids == NULL || *sfx_count >= max_sfx_count) {
        return;
    }

    i32 sfx_id = GetSfxId(sfx_name);
    if (sfx_id == -1) {
        return;
    }

    for (i32 i = 0; i < *sfx_count; i++) {
        if (sfx_ids[i] == sfx_id) {
            return;
        }
    }

    sfx_ids[*sfx_count] = sfx_id;
    ++*sfx_count;
}

void AddLevelSfxGizmoSys(GIZMOSYS_s *gizmo_sys, void *world_info, i32 *sfx_ids, i32 *sfx_count, i32 max_sfx_count) {
    if (gizmotypes == NULL || gizmo_sys == NULL) {
        return;
    }

    GIZMOSET *set = gizmo_sys->sets;
    GIZMOTYPE *type = gizmotypes->types;
    i32 i = 0;
    while (i < gizmotypes->count) {
        if (type->fns.add_level_sfx_fn != NULL) {
            type->fns.add_level_sfx_fn(world_info, set->unknown, sfx_ids, sfx_count, max_sfx_count);
        }
        ++i;
        ++set;
        ++type;
    }
}

void BlockSfx(GameObject_s *object) {
    GameObject_s *target = object->force_target;
    if (target != NULL) {
        const u32 context_flags = CInfo[target->character_context].flags;
        if ((context_flags & 0x4000000) != 0 || ((context_flags & 0x8000000) != 0 && (target->jump_flags & 2) != 0)) {
            const i32 sfx_bits = GameAudio_GetPlrSfxBits(target);
            GameAudio_PlaySfx(0x3f, &object->apiobj.collision_position, sfx_bits, 0);
        }
    }
}

i32 SfxBitEx(i32 sound);

void AddLevSfx(WORLDINFO_s *world, nuvec_s *position, char *name, i32 sfx) {
    if (sfx == -1 && name != NULL) {
        sfx = GetSfxId(name);
    }
    if (sfx == -1) {
        return;
    }

    const i32 count = world->level_sfx_count;
    i32 index;
    LEVELSFXENTRY_s *entry = world->level_sfx;
    for (index = 0; index < count; ++index, ++entry) {
        if (entry->id == sfx)
            break;
    }
    if (index == count) {
        if (index >= 64) {
            return;
        }
        if (position != NULL) {
            world->level_sfx[index].position = *position;
        } else {
            world->level_sfx[index].position = nuvec_zero;
        }
        world->level_sfx[index].id = static_cast<i16>(sfx);
        world->level_sfx[index].references = 1;
        world->level_sfx_count = index + 1;
    } else if (world->level_sfx[index].references != -1) {
        ++world->level_sfx[index].references;
    }
}

i32 GameAudio_GetPlrSfxBits(void *object_ptr) {
    APIOBJECT *object = static_cast<APIOBJECT *>(object_ptr);
    i32 sfx_bits = 0;
    if (object != NULL && static_cast<i8>(object->flags_low) < 0) {
        sfx_bits = 1 << object->field_0x27c;
    }
    return sfx_bits;
}

void GameAudio_Reset() {
    memset(&GameAudio_Default, 0, sizeof(GameAudio_Default));
    GameAudio = &GameAudio_Default;
    memset(GameAudio_Default.sfx_names, 0, sizeof(GameAudio_Default.sfx_names));
    for (i32 i = 0; i < 0x55; ++i) {
        GameAudio_Default.sfx_ids[i] = -1;
    }
}

void GameAudio_PlaySfxById(i32 sfx_id, nuvec_s *position, i32 flags, i32) {
    if (flags == 0) {
        PlaySfxById(sfx_id, position);
        return;
    }
    if ((flags & ~2) == 1) {
        nusound_special_positions[1] = *position;
        PlaySfxById(sfx_id, &nusound_special_positions[1]);
    }
    flags -= 2;
    if (static_cast<u32>(flags) <= 1) {
        nusound_special_positions[2] = *position;
        PlaySfxById(sfx_id, &nusound_special_positions[2]);
    }
}

void GameAudio_PlaySfx(i32 sfx, nuvec_s *position, i32 flags, i32 volume) {
    if ((u32)sfx < 0x55) {
        GameAudio_PlaySfxById(GameAudio->sfx_ids[sfx], position, flags, volume);
    }
}

void GameAudio_Init(GAMEAUDIO *audio) {
    GameAudio = audio;
    for (i32 i = 0; i < 0x55; ++i) {
        GAMEAUDIO *current_audio = GameAudio;
        current_audio->sfx_ids[i] = static_cast<i16>(GetSfxId(current_audio->sfx_names[i]));
    }

    MenuRegisterSoundFX(GameAudio_GetSfxId(0x2f), GameAudio_GetSfxId(0x30), GameAudio_GetSfxId(0x31),
                        GameAudio_GetSfxId(0x32));
}

i32 GameAudio_GetSfxId(i32 sfx) {
    if (static_cast<u32>(sfx) <= 0x54) {
        return GameAudio->sfx_ids[sfx];
    }
    return -1;
}

void GameAudio_PlaySfxAndSetVolume(i32 sfx, nuvec_s *position, f32 volume) {
    if (static_cast<u32>(sfx) < 0x55) {
        PlaySfxByIdAndSetVolume(GameAudio->sfx_ids[sfx], position, volume);
    }
}

extern "C" void MusicSeekOffset(i32 track, f32 seek_offset);



extern "C" void SoundUpdate(float frame_time);
