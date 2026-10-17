#ifdef __cplusplus

extern float minikittime;
extern i32 drawautosaveicon;

struct WORLDINFO_s;

void InitGameBeforeConfig(void);
void InitGameAfterConfig(void);

void CompleteLevel(struct WORLDINFO_s *world);
void StoreLevelProgress(struct WORLDINFO_s *world);
void NewAreaMusicChanges(void);
void Game_WorldInfo_InitLast(struct WORLDINFO_s *world);
void Game_WorldInfo_InitMenu(struct WORLDINFO_s *world, int *menu_id, int *menu_y);

#endif

// Declared here for the other units that use them.
struct BOLT_s;
struct CUTINFO;
struct GIZMOBLOWUP_s;
struct GameObject_s;
struct MENU_s;
struct NUGCUTCHAR_s;
struct PART_s;
struct nufpcomjmp_s;
struct nuvec_s;
extern nufpcomjmp_s LevelConfigKeywords_BeforeLoad[];
i32 Bolt_HitCustomFn_LSW(BOLT_s *, nuvec_s *);
i32 Bolt_HitPlatFn_LSW(BOLT_s *);
i32 CutScene_ReplaceCharacterModelFn_LSW(CUTINFO *, NUGCUTCHAR_s *);
i32 CutScene_StartFn_LSW(CUTINFO *);
i32 DoBuckStart(GameObject_s *);
i32 InitBolt_AddMomentumType_LSW(BOLT_s *, GameObject_s *, nuvec_s *);
u16 MakeSaveHash(void);
void BuckStartExtra_LSW(GameObject_s *);
void BuyAllShopExtras();
void CutScene_PostUpdateFn_LSW(void);
void CutScene_PreUpdateFn_LSW(CUTINFO *);
void DeflectPart(PART_s *, GameObject_s *, f32, f32, i32, i32);
void GameBlowUpBlownUpFn_LSW(GIZMOBLOWUP_s *);
void GameObjectDimensionsExtra_LSW(GameObject_s *);
void Game_KillPart(PART_s *part, i32 reason);
void GizmoBlowUp_Sfx(GIZMOBLOWUP_s *, nuvec_s *position);
void InitGameMode();
void MenuDrawEndChallenge(MENU_s *menu);
void MenuUpdateEndChallenge(MENU_s *menu);
void NewGame(void);
void PartCollide_2D(PART_s *);
void ResetAdaptiveDifficulty();
