// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

// Startup / permanent-data loading.
//
// This TU owns the permanent arena bootstrap and the loading-screen frame
// loop.  It corresponds to the original `startup` module:
//
//   StartPerm  0x470db0  — reset the permanent bump pointer to the super-buffer base.
//   EndPerm    0x470de0  — no-op tail (kept for link compatibility).
//   LoadPerm   0x1bf310  — public entry: chooses the background vs synchronous path.
//   LoadPermData (file-static) 0x1bebd0 — runs on the bg thread (or inline) and
//               actually carves the permanent arena: fonts, strings, music, SFX,
//               level tables, character tables, fades, gizmos, etc.
//
// File-statics `legal_tid` / `loadlegal_done` / `legal_mtl` live here in the
// original as `_ZL9legal_tid` etc., so they stay file-scoped.  Behaviour is
// faithfully preserved (including the PAL language-select spin and the
// deliberately inverted legal fade-in quirk).

#include "legogame/startup.h"
#include "legoapi/core/config/cheat.h"

#include <string.h>
#include <time.h>

#include "globals.h"
#include "java/android.h"
#include "legoapi/gizmo/object/gizmoblowups.h"
#include "legoapi/gizmo/base/gizactions.h"
#include "MechInputTouch/MechInputTouch_types.h"
#include "gameframework/saveload.h"
#include "legoapi/characters/core/character.h"
#include "legoapi/characters/core/players.h"
#include "legoapi/items/collect/minikits.h"
#include "legoapi/core/input/timer.h"
#include "legoapi/menus/core/panel.h"
#include "legoapi/menus/core/text.h"
#include "legoapi/render/core/screen.h"
#include "legoapi/world/area.h"
#include "legoapi/world/levels/episode.h"
#include "legoapi/world/levels/levels.h"
#include "legoapi/legoapi_types.h"
#include "legogame/game.h"
#include "nu2api/nu3d/nugscn.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/numtx.h"
#include "nu2api/numusic/numusic.h"
#include "nu2api/numusic/sfx.h"
#include "nu2api/nusound/nusound.h"
#include "decomp.h"
#include "legoapi/world/world.h"
#include <stdio.h>
#include "gameapi/gui/apimenu.h"
#include "gameapi/ai/aisys/aisys.h"
#include "legoapi/characters/core/charconfig.h"
#include "legoapi/cutscenes/cutscenes.h"
#include "legoapi/items/objects/gameobjects.h"
#include "legoapi/world/level.h"
#include "legoapi/characters/motion/gameanim.h"
#include "legoapi/props/doors/door.h"
#include "legoapi/core/input/gamepads.h"
#include "legoapi/props/system/socksys.h"
#include "legoapi/render/fx.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "legoapi/audio/audio.h"
#include "legoapi/characters/motion/action_info.h"
#include "legoapi/actions/character/legoapi_tag.h"
#include "legoapi/ai/core/gameai.h"
#include "legoapi/ai/game/creature.h"
#include "legoapi/characters/core/characters.h"
#include "legoapi/core/startup/text.h"
#include "legoapi/items/objects/things.h"
#include "legoapi/lib/text.h"
#include "legoapi/menus/core/gamehint.h"
#include "legoapi/menus/screens/gamestatus_lsw.h"
#include "legoapi/world/levels.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/nuandroid/ios_device.h"
#include "gameapi/edtools/edgra.h"
#include "legoapi/gizmo/base/gizmessage.h"
#include "legoapi/gizmos/object/gizobstacles.h"
#include "legoapi/items/collect/bolt.h"
#include "legoapi/menus/screens/gamemenuall.h"
#include "legoapi/menus/screens/menu.h"
#include "legoapi/menus/screens/movies.h"
#include "nu2api/nucore/nuapi.h"

// buffer globals owned by the batman TU (batman.h declares them extern "C")
extern "C" {
    extern VARIPTR characterbuffer_ptr;
    extern VARIPTR characterbuffer_base;
    extern VARIPTR characterbuffer_end;
}

// Globals / objects declared elsewhere.
extern i16 id_OBIWANKENOBI;
extern "C" GAMEPAD_s GamePad[64]; // gamepads.cpp, bss @0x127a500
extern u8 RAP_WARNING_R, RAP_WARNING_G, RAP_WARNING_B;

struct CHARFIXUP;
extern CHARFIXUP CharFixUp[222]; // characters.cpp

// ---------------------------------------------------------------------------
// File-local constants and helpers.
// ---------------------------------------------------------------------------

namespace {

    constexpr i32 kLevelHackTableBytes = 0x80;
    constexpr i32 kExtraGizAIMessageCount = 0x40;
    constexpr usize kSmallHeapSize = 0x200;
    constexpr usize kLegalTextureReserve = 0x400000; // carved from top of super buffer
    constexpr f32 kLegalFadeInDuration = 0.3f;
    constexpr f32 kLegalHoldEnd = 5.3f;
    constexpr f32 kLegalFadeOutDuration = 0.30000019f;
    constexpr f32 kLegalVisibleEnd = 5.6f;
    constexpr f32 kLegalTimerMax = 5.80000019f;
    constexpr f32 kIntroFadeInStart = 0.3f;
    constexpr f32 kIntroFadeInDuration = 0.29999995f;
    constexpr f32 kIntroHoldEnd = 3.3f;
    constexpr f32 kIntroFadeOutDuration = 0.29999995f;
    constexpr f32 kIntroDuration = 3.8f;
    constexpr f32 kMenuFlashPeriod = 0.2f;
    constexpr f32 kMenuFlashThreshold = 0.1f;

    // Convert IEEE-754 f32 -> IEEE-754 binary16 (half) for the half-UV vertex
    // format used by `LegalVertex` when `g_NuPrim_NeedsHalfUVs` is set.
    u16 F32ToF16(f32 value) {
        u32 bits;
        memcpy(&bits, &value, sizeof(bits));

        const u32 sign = (bits >> 16) & 0x8000u;
        i32 exp = static_cast<i32>((bits >> 23) & 0xffu) - 127 + 15;
        const u32 mant = bits & 0x7fffffu;

        if (exp >= 0x1f) {
            return static_cast<u16>(sign | 0x7c00u); // inf / overflow -> inf
        }
        if (exp <= 0) {
            return static_cast<u16>(sign); // subnormals flushed to zero
        }
        return static_cast<u16>(sign | (static_cast<u32>(exp) << 10) | (mant >> 13));
    }

} // namespace

// ---------------------------------------------------------------------------
// File-statics — original TU statics (_ZL* in the original ELF).
// ---------------------------------------------------------------------------

static i32 legal_tid;      // texture id of the loaded legal screen
static i32 loadlegal_done; // legal texture already attempted
static NUMTL *legal_mtl;   // material wrapping the legal texture

// ---------------------------------------------------------------------------
// Public permutations — thin wrappers that set up the arena.
// ---------------------------------------------------------------------------

void StartPerm(void) {
    permbuffer_ptr.addr = permbuffer_base.addr;
    permbuffer_end.addr = superbuffer_end.addr;
}

// libTTapp.so 0x470de0: close the permbuffer, carve out the character
// buffer, and split the remaining superbuffer in half between the two world
// bump buffers. This is what gives each WORLDINFO its buffer_start /
// giz_buffer / buffer_end cursors that WorldInfo_Reset restores.
void EndPerm(void) {
    usize aligned = ALIGN(permbuffer_ptr.addr, 4);
    permbuffer_end.addr = aligned;
    permbuffer_size = (i32)(aligned - permbuffer_base.addr);

    characterbuffer_ptr.addr = aligned;
    characterbuffer_base.addr = aligned;
    usize world_start = aligned + static_cast<usize>(CHARACTERBUFFERSIZE);
    characterbuffer_end.addr = world_start;
    superbuffer_ptr.addr = world_start;
    superbuffer_base.addr = world_start;

    usize half = (superbuffer_end.addr - world_start) >> 1;
    WorldInfo[0].giz_buffer.addr = world_start;
    WorldInfo[0].buffer_start = reinterpret_cast<void *>(world_start);
    WorldInfo[0].unknown_0108.addr = world_start + half;
    WorldInfo[1].giz_buffer.addr = world_start + half;
    WorldInfo[1].buffer_start = reinterpret_cast<void *>(world_start + half);
    WorldInfo[1].unknown_0108.addr = superbuffer_end.addr;
    editbuffer_end.addr = superbuffer_end.addr - static_cast<usize>(EDITBUFFERENDSIZE);
}

struct PLAYERPACKET_s;

// ---- Misc player/gameobject helpers relocated from doorstubs.cpp ----

// ---------------------------------------------------------------------------
// Permanent-data initialisation (background thread entry).
// ---------------------------------------------------------------------------

static void LoadPermData(BGPROCINFO *proc) {
    InitMemCard();

    // Font path depends on language.  `Text_Language == 0` is Japanese in the
    // original; everything else uses the Latin font.
    if (Text_Language != 0) {
        Text_LoadFont((char *)"stuff\\text\\starwars_font", &permbuffer_ptr, &permbuffer_end);
    } else {
        Text_LoadFont((char *)"stuff\\text\\starwars_font_j", &permbuffer_ptr, &permbuffer_end);
    }

    // PAL builds show the language menu — the loader spins here until the menu
    // writes `LoadPerm_LanguageSelect = 3` ("selection confirmed").
    while (LoadPerm_LanguageSelect != 3) {
    }

    Text_InitStringTable(0x70d, &permbuffer_ptr, &permbuffer_end);
    Text_InitTable(reinterpret_cast<TEXTENTRY *>(LSW_Text), 0, 0x70c);
    Text_LoadStrings(&permbuffer_ptr, &permbuffer_end);
    Text_InitDefaultStrings();
    LoadPerm_StringsLoaded = 1;

    LOG_INFO("LoadPermData: proc=%p langsel=%d", static_cast<void *>(proc), LoadPerm_LanguageSelect);

    // Legal-screen texture — carved from the top of the super buffer so it
    // never collides with the permanent bump allocator.  Only attempted once.
    if (proc != nullptr && legal_tid == 0 && !loadlegal_done) {
        VARIPTR legal_tex_end = superbuffer_end;
        VARIPTR legal_tex_base;
        legal_tex_base.addr = legal_tex_end.addr - kLegalTextureReserve;

        char legal_path[128] __attribute__((aligned(16)));
        char legal_name[64];
        NuStrCpy(legal_path, "stuff\\legal\\");
        NuIOS_GetDeviceLanguage();
        if (Text_Language == 2) {
            NuStrCpy(legal_name, "LEGAL_FRENCH");
        } else {
            NuStrCpy(legal_name, "LEGAL_ENGLISH");
        }
        NuStrCat(legal_path, legal_name);
        legal_tid = NuTexRead(legal_path, &legal_tex_base, legal_tex_end);
        loadlegal_done = 1;
        LOG_INFO("LoadPermData: legal_tid=%d", legal_tid);
    }

    MechSystems::Get()->LoadPerm();

    NuStringFilterLoad((char *)"stuff\\text\\badwords.txt", &permbuffer_ptr, permbuffer_end);

    // Audio / rendering permanents.
    MusicInfo = ConfigureMusic((char *)"audio\\music.txt", &permbuffer_ptr, &permbuffer_end);
    RegisterMusic(MusicInfo);
    InitSfx(&permbuffer_ptr, permbuffer_end, "Audio\\Audio.cfg");
    InitStillRender(&permbuffer_ptr, permbuffer_end);

    LevelHackData = GameBufferAlloc(&permbuffer_ptr, &permbuffer_end, kLevelHackTableBytes);
    OldLevelHackData = GameBufferAlloc(&permbuffer_ptr, &permbuffer_end, kLevelHackTableBytes);
    LevelHackSize = 0;

    LevelProgress_ReserveBufferSpace(&permbuffer_ptr, permbuffer_end);

    // Fade system — the four global fade objects are registered once.
    FadeSys.Init();
    FadeSys.AddFade(&fade);
    FadeSys.AddFade(&fadeWipe);
    FadeSys.AddFade(&fadeStillWipe);
    FadeSys.AddFade(&fadeStill);
    pFadeInfo = &FadeSys;

    LevelObjects_InitForGame(reinterpret_cast<LEVELOBJECT *>(ObjTab), &permbuffer_ptr, &permbuffer_end, 0x2ee, 0x1f40);
    LevelSplines_InitForGame(SplTab);
    ObjTab[0].name = lsw_memcard_objname;

    saveicon_scene = NuGScnRead(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\ps2_bits.gsc");
    button_scene = NuGScnRead(&permbuffer_ptr, permbuffer_end, (char *)"stuff\\pc_bits.gsc");

    // ------------------------------------------------------------------
    // Tiny bump allocator embedded in `theMemoryManager`.
    // ------------------------------------------------------------------
    {
        MemoryManager &manager = theMemoryManager;
        const usize aligned_base = ALIGN(permbuffer_ptr.addr, 0x10);
        manager.cursor_cell = &manager.cursor;
        manager.allocated = 0;
        manager.cursor = aligned_base;
        manager.end = aligned_base + kSmallHeapSize;
        permbuffer_ptr.addr = aligned_base + kSmallHeapSize;
        manager.remaining = kSmallHeapSize;
        manager.end_cell = &manager.end;
        manager.high_water = 0;
        memset(manager.free_lists, 0, sizeof(manager.free_lists));
    }

    CreateThingManager();
    RegisterGizmoTypes_LSW(&permbuffer_ptr, &permbuffer_end);

    GameAnimSys_AllocateLevelProgressData(&permbuffer_ptr, &permbuffer_end, 0x280, 0xc);
    LSW_registerStatusScreen();
    initGameHintSys_LSW();

    // Wire LSW gameplay hooks.
    CutScene_StartFn = CutScene_StartFn_LSW;
    CutScene_PreUpdateFn = CutScene_PreUpdateFn_LSW;
    CutScene_PostUpdateFn = CutScene_PostUpdateFn_LSW;
    CutScene_StoppedFn = CutScene_StoppedFn_LSW;
    CutScene_ReplaceCharacterModelFn = CutScene_ReplaceCharacterModelFn_LSW;

    InitBolt_AddMomentumType = InitBolt_AddMomentumType_LSW;
    Bolt_HitPlatFn = Bolt_HitPlatFn_LSW;
    Bolt_HitCustomFn = Bolt_HitCustomFn_LSW;
    GameBlowUpBlownUpFn = GameBlowUpBlownUpFn_LSW;
    GizObstacle_SetDefaultSFXFn = GizObstacle_SetDefaultSFXFn_LSW;

    Movies_ConfigureList((char *)"movies\\movies.txt", &permbuffer_ptr, &permbuffer_end);

    SetProceduralAnimationFn(reinterpret_cast<void *>(NuAnimBuffProceduralAnimation));

    // Characters, levels, areas, episodes — each carves from the perm buffer.
    CDataList = ConfigureCharacterList((char *)"chars\\chars.txt", &permbuffer_ptr, &permbuffer_end, 0x154, &CHARCOUNT,
                                       static_cast<i32>(sizeof(GAMECHARACTERDATA)), &GCDataList);
    CharScenes_Init(&permbuffer_ptr, &permbuffer_end);
    IconScenes_Init((char *)"stuff\\icons\\", &permbuffer_ptr, &permbuffer_end);
    FixUpCharacters(CharFixUp);
    MiniKits_Init(&permbuffer_ptr, &permbuffer_end);

    id_DEFAULTCHARACTER[0] = id_QUIGONJINN;
    id_DEFAULTCHARACTER[1] = id_OBIWANKENOBI;

    CharCategories_Init(LSW_CharCategory);
    Cheats_Init(reinterpret_cast<CHEAT *>(Cheat));
    PlayerID[0] = id_DEFAULTCHARACTER[0];
    PlayerID[1] = id_DEFAULTCHARACTER[1];
    CharVariants_Init(CharVariants_Game, 0x17);

    LDataList = Levels_ConfigureList((char *)"levels\\levels.txt", &permbuffer_ptr, &permbuffer_end, 0x16d, &LEVELCOUNT,
                                     &Level_SetDefaults);
    FixUpLevels(&LevFixUp);

    ADataList = Areas_ConfigureList((char *)"levels\\areas.txt", &permbuffer_ptr, &permbuffer_end, 0x48, &AREACOUNT);
    FixUpAreas();

    EDataList =
        Episodes_ConfigureList((char *)"levels\\episodes.txt", &permbuffer_ptr, &permbuffer_end, 6, &EPISODECOUNT);

    NewGame();
    InitGameAfterConfig();

    APICharacterSysInit(&permbuffer_ptr, permbuffer_end, CHARCOUNT, 0x30, 0xe9, 0x400, CDataList, SetCreatureLights);
    SetActionInfo(ActionInfo, ExtraActionData);

    gizaimessagesys = CreateGizAIMessageSys(&permbuffer_ptr, &permbuffer_end, kExtraGizAIMessageCount);
    permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 0x10);
    BackDrop_Init((char *)"stuff\\starfield.gsc", &permbuffer_ptr, &permbuffer_end);

    LOG_INFO("LoadPermData: before LoadPerm1");
    LoadPerm1();
    LOG_INFO("LoadPermData: before LoadPerm2");
    LoadPerm2();
    LOG_INFO("LoadPermData: after LoadPerm2");

    static_cast<GameThingManager *>(theGameThings)->AddOnceOnlyThings();

    LOG_INFO("LoadPermData: before RegisterHelpers");
    RegisterHelpers();

    permbuffer_ptr.addr = ALIGN(permbuffer_ptr.addr, 0x10);
    PermDataLoaded = 1;
}

void LoadPerm(void) {
    SetBackgroundMusic(1);
    WORLDINFO_s *saved_world = WORLD;
    PermDataLoaded = 0;
    WORLD = nullptr;

    if (BGLOAD == 0 || LOADEROFF != 0) {
        LoadPerm_LanguageSelect = 3;
        LoadPermData(nullptr);
        if (legal_mtl != nullptr) {
            NuMtlDestroy(legal_mtl);
            legal_mtl = nullptr;
        }
        WORLD = saved_world;
        return;
    }

    FRAMETIME = DEFAULTFRAMETIME;

    pNuCam->mtx = numtx_identity;
    pNuCam->fov = 20.0f;
    pNuCam->aspect = 1.77f;
    NuCameraSet(pNuCam);
    InitPanel(Game.options_save.field11_0xb);
    NuQFntSetCoordinateSystem(static_cast<NUQFNT_CSMODE>(3));

    NUMTX scale;
    NUVEC scale_vec = {0.125f, 0.125f, 0.125f};
    NuMtxSetScale(&scale, &scale_vec);
    scale.m32 = 1.0f;

    if (PAL == 0) {
        LoadPerm_LanguageSelect = 3;
    }

    i32 device_language = 0;
    if (NuStrICmp(g_deviceManufacturer, (char *)"Amazon") == 0 ||
        (device_language = NuIOS_GetDeviceLanguage(), LANGUAGECOUNT < 1)) {
        LoadPerm_LanguageSelect = 0;
    } else {
        i32 language_index = 0;
        while (Text_LanguageList[language_index].language != device_language) {
            ++language_index;
            if (language_index == LANGUAGECOUNT) {
                break;
            }
        }
        if (language_index == LANGUAGECOUNT) {
            LoadPerm_LanguageSelect = 0;
        } else {
            NuLanguageSet(device_language);
            Text_Language = static_cast<u32>(device_language);
            if (language_index >= LANGUAGECOUNT) {
                LoadPerm_LanguageSelect = 0;
            }
        }
    }

    bgPostRequest(LoadPermData, nullptr, nullptr, 0);

    f32 intro_timer = 0.0f;
    f32 legal_timer = 0.0f;
    f32 ready_timer = 0.0f;
    f32 language_timer = 0.0f;
    f32 tail_timer = 0.2f;

    while (PermDataLoaded == 0 || tail_timer > 0.0f) {
        NuFrameBegin();
        NuCameraSet(&global_camera);
        readpads_always = 1;
        ReadPads();
        UpdateGameMenu(GamePad, 0);
        UpdateTimer(&GlobalTimer);
        menu_flash = NuFmod(GlobalTimer.time_elapsed_mod_seconds, 0.2f) < 0.1f;

        if (language_timer < 0.5f && Text_IsFontLoaded() != nullptr) {
            if (LoadPerm_LanguageSelect == 0) {
                NewMenu(7, 0, -1);
                LoadPerm_LanguageSelect = 1;
                FadeSys.fade = -1.0f;
            }
            if (LoadPerm_LanguageSelect == 1 && GameMenuLevel == 0) {
                LoadPerm_LanguageSelect = 2;
            }
            if (LoadPerm_LanguageSelect == 2) {
                language_timer += FRAMETIME;
                if (language_timer >= 0.5f) {
                    LoadPerm_LanguageSelect = 3;
                }
            }
        }

        if (ready_timer < 1.0f && LoadPerm_LanguageSelect == 3 && LoadPerm_StringsLoaded != 0 && saveload_status == 1 &&
            GameMenuLevel == 0) {
            ready_timer += FRAMETIME;
            if (ready_timer >= 1.0f) {
                ready_timer = 1.0f;
            }
        }

        if (legal_timer < 5.8f && LoadPerm_LanguageSelect == 3 && ready_timer >= 1.0f && loadlegal_done) {
            if (!(legal_timer == 0.0f && legal_mtl == nullptr)) {
                if (PermDataLoaded != 0 &&
                    ((GamePad[0].buttons_down_08 &
                      (GAMEPAD_JUMP | GAMEPAD_START | GAMEPAD_SPECIAL | GAMEPAD_ACTION | GAMEPAD_TAG)) != 0 ||
                     MechInputTouchMenuController::AnyTouchesThisFrame > 0)) {
                    MechInputTouchMenuController::AnyTouchesThisFrame = 0;
                    if (legal_timer >= 0.3f && legal_timer < 5.3f) {
                        legal_timer = 5.3f;
                    }
                }
                legal_timer += FRAMETIME;
                if (legal_timer > 5.8f) {
                    legal_timer = 5.8f;
                }
            } else {
                const i32 texture_id = legal_tid;
                if (texture_id != 0) {
                    NUMTL *mtl = NuMtlCreate(1);
                    if (mtl != nullptr) {
                        mtl->opacity = 1.0f;
                        mtl->diffuse_color = {1.0f, 1.0f, 1.0f};
                        mtl->shader_desc.flags = 0x1000;
                        mtl->tex_id = static_cast<i16>(texture_id);
                        u8 *attrib = reinterpret_cast<u8 *>(&mtl->attribs);
                        attrib[1] = (attrib[1] & 0xcf) | 0xe0;
                        attrib[0] = (attrib[0] & 0xc0) | 0x22;
                        NuMtlUpdate(mtl);
                        legal_mtl = mtl;
                    } else {
                        legal_timer = 5.8f;
                    }
                } else {
                    legal_timer = 5.8f;
                }
            }
        }

        if (intro_timer < 3.8f && LoadPerm_LanguageSelect == 3 && ready_timer >= 1.0f && legal_timer >= 5.8f &&
            Text_IsFontLoaded() != nullptr && LoadPerm_StringsLoaded != 0 && IntroText_TextID != -1) {
            if (PermDataLoaded != 0 && ((GamePad[0].buttons_down_08 & (GAMEPAD_JUMP | GAMEPAD_START | GAMEPAD_SPECIAL |
                                                                       GAMEPAD_ACTION | GAMEPAD_TAG)) != 0 ||
                                        MechInputTouchMenuController::AnyTouchesThisFrame > 0)) {
                MechInputTouchMenuController::AnyTouchesThisFrame = 0;
                if (intro_timer >= 0.3f && intro_timer < 3.3f) {
                    intro_timer = 3.3f;
                }
            }
            intro_timer += FRAMETIME;
            if (intro_timer > 3.8f) {
                intro_timer = 3.8f;
            }
        }

        if (PermDataLoaded != 0 && LoadPerm_LanguageSelect == 3 && ready_timer >= 1.0f && legal_timer >= 5.8f &&
            intro_timer >= 3.8f) {
            tail_timer -= FRAMETIME;
            if (tail_timer < 0.0f) {
                tail_timer = 0.0f;
            }
        }

        NuRndrBeginScene(-1);
        NuRndrGradClear(0xf00, 0x80000000, 0x80000000, 1.0f);
        if (tail_timer == 0.2f) {
            DrawMenu(0);

            if (legal_timer > 0.0f && legal_timer < 5.8f && legal_mtl != nullptr) {
                f32 alpha;
                if (!(legal_timer < 0.3f)) {
                    if (legal_timer < 5.3f) {
                        alpha = 1.0f;
                    } else if (legal_timer >= 5.6000003814697266f) {
                        alpha = 0.0f;
                    } else {
                        alpha = 1.0f - (legal_timer - 5.3f) / 0.30000019f;
                    }
                } else {
                    alpha = legal_timer / 0.3f;
                }

                if (alpha > 0.0f) {
                    if (legal_timer < 0.0f) {
                        if (legal_timer >= -0.3f) {
                            alpha *= 1.0f - (legal_timer + 0.3f) / 0.3f;
                        }
                        const i32 text_alpha = static_cast<i32>(alpha * 128.0f);
                        SmartTextEx(TTab[1517] != nullptr ? TTab[1517] : (char *)"Test", 0.0f,
                                    TTab[1578] != nullptr ? 0.4f : 0.0f, 1.0f, 0.5f, 0.5f, 0.5f, 0, 255, 255, 255, 1.9f,
                                    1, nullptr, 0, text_alpha);
                        if (TTab[1578] != nullptr) {
                            SmartTextEx(TTab[1578], 0.0f, -0.25f, 1.0f, 0.5f, 0.5f, 0.5f, 0, RAP_WARNING_R,
                                        RAP_WARNING_G, RAP_WARNING_B, 1.9f, 4, nullptr, 0, text_alpha);
                        }
                    } else {
                        if (legal_timer < 0.3f) {
                            alpha *= (legal_timer - 0.3f) / 0.3f;
                        }

                        const f32 aspect = NuIOS_GetAspectRatio();
                        f32 half_w;
                        f32 half_h;
                        if (!(aspect > 1.7777778f)) {
                            half_w = 0.5f;
                            half_h = aspect * 0.5f / 1.7777778f;
                        } else {
                            half_h = 0.5f;
                            half_w = aspect * 0.5f / 1.7777778f;
                        }

                        NuRndrClear(0xb00, 0, 1.0f);
                        const u32 colour = SAGA_X86_F32_TO_U32(alpha * 255.0f) << 24;
                        NuPrimCSPos++;
                        NuPrimSetCoordinateSystem(NUPRIM_SCALEMODE_ABSOLUTE);
                        NuPrim2DBegin(4, 7, legal_mtl);

                        struct LegalVertex {
                            f32 x, y, z;
                            u32 colour;
                            union {
                                struct {
                                    f32 u, v;
                                } full;
                                struct {
                                    u16 u, v;
                                    u32 pad;
                                } half;
                            };
                        };

                        LegalVertex *vert = reinterpret_cast<LegalVertex *>(g_NuPrim_StreamBufferPtr->addr);
                        vert->colour = colour | (g_NuPrim_NeedsOverbrightening ? 0x808080u : 0x404040u);
                        if (!g_NuPrim_NeedsHalfUVs) {
                            vert->full.u = 0.0f;
                            vert->full.v = 0.0f;
                        } else {
                            vert->half.u = F32ToF16(0.0f);
                            vert->half.v = F32ToF16(0.0f);
                        }
                        NuPrim2DAddXYZ(0.5f - half_w, 0.5f - half_h, 0.0f);

                        vert = reinterpret_cast<LegalVertex *>(g_NuPrim_StreamBufferPtr->addr);
                        vert->colour = colour | (g_NuPrim_NeedsOverbrightening ? 0x808080u : 0x404040u);
                        if (!g_NuPrim_NeedsHalfUVs) {
                            vert->full.u = 1.0f;
                            vert->full.v = 1.0f;
                        } else {
                            vert->half.u = F32ToF16(1.0f);
                            vert->half.v = F32ToF16(1.0f);
                        }
                        NuPrim2DAddXYZ(0.5f + half_w, 0.5f + half_h, 0.0f);
                        NuPrim2DEnd();
                        NuPrimCSPos--;
                        NuPrimSetCoordinateSystem(NuPrimCoordSystemStack[NuPrimCSPos]);
                    }
                }
            }

            if (intro_timer > 0.0f && intro_timer < 3.8f) {
                f32 alpha;
                if (!(intro_timer >= 0.3f)) {
                    alpha = intro_timer / 0.3f;
                } else if (intro_timer < 3.3f) {
                    alpha = 1.0f;
                } else if (!(intro_timer < 3.6f)) {
                    alpha = 0.0f;
                } else {
                    alpha = 1.0f - (intro_timer - 3.3f) / 0.29999995f;
                }
                if (alpha > 0.0f) {
                    IntroText_Draw(alpha);
                }
            }
        }

        NuRndrEndScene();
        edGraEnableTerrainSwap();
        FRAMETIME = NuFrameEnd();
        edGraDisableTerrainSwap();
        if (FRAMETIME < DEFAULTFRAMETIME) {
            FRAMETIME = DEFAULTFRAMETIME;
        } else if (FRAMETIME > DEFAULTFRAMETIME * 3.0f) {
            FRAMETIME = DEFAULTFRAMETIME * 3.0f;
        }
    }

    if (legal_mtl != nullptr) {
        NuMtlDestroy(legal_mtl);
        legal_mtl = nullptr;
    }

    WORLD = saved_world;
}

void CheckForPlayersTurnedOff() {
    for (i32 source_index = 0; source_index < 2; ++source_index) {
        GameObject_s *source = Player[source_index];
        if (source != NULL && (source->apiobj.flags_high & APIOBJECT_HIGH_FLAG_CHARACTER) == 0) {
            for (i32 target_index = 2; target_index < 8; ++target_index) {
                GameObject_s *target = Player[target_index];
                if (target == NULL || (target->tag_flags & 2) != 0 || target->cannot_drop_in ||
                    target->character_context == 0x24 || target->character_context == 0x1f) {
                    continue;
                }

                if (target->apiobj.field_0x27d != 0 ||
                    (target->field_0xe31 == 1 && target->apiobj.character_data->game_character->field_0x28 > 0.0f)) {
                    TagCharacter(Player[source_index], target, 1);
                }
            }
        }
    }
}

i32 GetNumLocalPlayers() {
    i32 count = 0;
    if (Player[0] != NULL)
        count = static_cast<i8>(Player[0]->apiobj.field_0x1f8) < 0;
    if (Player[1] != NULL && Player[1]->apiobj.player_controlled)
        count++;
    return count;
}

i32 FreePlay_DropInToPlayerPos(GameObject_s *object) {
    i32 result = 0;
    if ((WORLD == NULL || WORLD->current_level != FACTORYF_LDATA) && player != NULL && FreePlay != 0 &&
        drop_in_teleport > 1 && VehicleArea == 0 && (Arcade != 0 || teleport_all_freeplay_modes != 0) &&
        object->field_0xcc0 == NULL && (CInfo[static_cast<i8>(object->character_context)].flags & 0x8000) == 0 &&
        object->character_context != 0x2b && object->apiobj.model_draw_result == 0 && (object->tag_flags & 2) == 0) {
        NUVEC position = player->apiobj.last_safe_position;
        if (position.x == player->apiobj.position.x && position.y == player->apiobj.position.y &&
            position.z == player->apiobj.position.z) {
            position.z += 0.01f;
        }

        if (NuCameraClipTestSphere(&position, object->apiobj.field_0x1e0, &numtx_identity) == 0) {
            ResetPlayerMoves(object);
            Player_ClearContext(object, 1);
            Player_ResetContexts(reinterpret_cast<PLAYERPACKET_s *>(object->player_packet));
            object->apiobj.start_position = position;
            object->apiobj.position = position;
            object->apiobj.velocity = v000;
            object->apiobj.field_0x276 = player->apiobj.field_0x276;
            ResetPlayerMoves(object);
            object->apiobj.movement_stuck_time = 0.0f;
            InitSurfaceInfo(object);
            SetObjOnSurface(object, 1);
            GameObjectOrigin(object);
            AddGameDebris(WORLD->debris_sys, 0x5c, &object->apiobj.collision_position);
            result = 1;
        }
    }
    return result;
}

i32 PlayersDropInOut() {
    CheckForPlayersTurnedOff();
    return 0;
}

