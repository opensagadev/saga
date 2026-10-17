#include "editor/edpath.h"
#include "legoapi/ai/core/ai_sys_stubs.h"
#include "legoapi/world/world_shared.h"
#include "decomp.h"
#include "editor/aieditor_state.h"
#include "editor/antinode_editor_private.h"
#include "gameapi/ai/aisys/aisys.h"
#include "globals.h"
#include "gameapi/edtools/edfile.h"
#include "nu2api/nu3d/nucamera.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nuspecial.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/numath/nuang.h"
#include "nu2api/numath/nufloat.h"
#include "nu2api/numath/nurand.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/numath/nuvec.h"
#include "nu2api/numath/nuplane.h"

#include <stdio.h>
#include <float.h>
#include <string.h>


extern "C" f32 (*GetCharacterGoalSpeedFn)(APIOBJECT *);

static __used__ i32 RowMoveWander(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);

static __used__ i32 RowMoveTowards(AIGROUP *group, AIROW *row, AIROW *previous, APIOBJECT *leader);

static void FormationMove(AIGROUP *group, i32 (*move)(AIGROUP *, AIROW *, AIROW *, APIOBJECT *));

static void AISysCheckAntinode_Rectangle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

static void AISysCheckAntinode_Ellipse(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

static void AISysCheckAntinode_Circle(APIOBJECT *object, AIANTINODE *antinode, NUVEC *difference, f32 radius);

extern "C" {
extern void (*checkantinodefns[3])(APIOBJECT *, AIANTINODE *, NUVEC *, f32);
extern char * AiLevelPathName;
extern AISCRIPTPROCESS * pSetStateDebugee;
extern SCRIPTPROCESSFIRSTTIMEACTION * ScriptProcessFirstTimeActionFn;
    extern f32 default_path_heighttol;
    extern u8 default_activate_difficulty;
    extern u8 default_min_n_respawns;
    extern u8 default_max_n_respawns;
    extern f32 default_min_t_respawn;
    extern f32 default_max_t_respawn;
    extern f32 default_stagger_start;
    extern void (*checkantinodefns[3])(APIOBJECT_s *, AIANTINODE_s *, NUVEC *, f32);
}

void AIPathCalcExtents(AIPATH *path);
i32 AIPathCheckExtents(AIPATH *path, NUVEC *position);
void AIMoveToDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIRetreatFromDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AICircle(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIWander(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIMoveToDestinationAvoidingCamera(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);
void AIMoveDirectlyToDestination(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);

typedef i32 MIDSPECIALMOVE(AISYS *system, AIPACKET *packet, APIOBJECT *object);
extern MIDSPECIALMOVE *MidSpecialMoveFn;

extern "C" i32 AISysSetLevelPath(AISYS *system, char *path_name);
extern "C" void (*GameAISYSRebuildFromEditorDataFn)(AISYS *, VARIPTR *, VARIPTR *);
extern "C" void AISysFindRoute(AIPACKET *packet);
extern "C" void AISysCharacterSetPath(AIPACKET *packet, AIPATH *path);
extern "C" void AISysCharacterSetPathCnx(AIPACKET *packet, NUVEC *position, AIPATHCNX *connection, i32 direction);

extern "C" void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);






#include "legoapi/ai/core/ai_pathgeometry.h"

static u32 AISysCharacterTestPathCnx(AISYS *system, APIOBJECT *object, AIPACKET *packet, AIPATHCNX *connection,
                                     i32 direction, f32 *nearest_distance);










extern "C" {

    AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name);

void AIMoveInstruction(AIPACKET *packet, NUVEC *destination, f32 stopping_distance, AIPATHINFO *path_info, i32 mode,
                           f32 movement_parameter);

void *AIPAthFindPathCnx(AISYS_s *system, AIPATH_s *path, char *from_name, char *to_name, i32 *direction);

AILOCATOR *AIPathFindLocator(AISYS *sys, char *name);

AILOCATORSET *AIPathFindLocatorSet(AISYS *system, char *name);

AIPATHNODE *AIPathFindNode(AISYS *system, AIPATH *path, char *name);

void AIPathNodeUpdatePos(AISYS *system, AIPATH *path, AIPATHNODE *node);

#include "legoapi/ai/core/ai_sys_stubs_types.h"
    DECOMP_ASSERT(sizeof(RebuildEditorCreature) == 0xac, "editor creature rebuild stride");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, path_check) == 0x38, "editor creature path offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation_area) == 0x6c, "editor creature area offset");
    DECOMP_ASSERT(offsetof(RebuildEditorCreature, activation) == 0x8f, "editor creature activation offset");

void AISYSRebuildFromEditorData(void);

void AIScriptForceParamReEval(AISCRIPTPROCESS *processor);

char *AIScriptNameFromIx(AISYS *system, i32 index);

} // extern "C"


#if defined(__i386__)
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone, regparm(2)))
#else
#define AI_SYS_STUBS_CLONE_ATTR2 __attribute__((noinline, noclone))
#endif

// Retail keeps only the IPA-SRA clone, without the unused object and elapsed time.
static AI_SYS_STUBS_CLONE_ATTR2 AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions,
                                                                           AIPACKET *packet,
                                                                           AISCRIPTPROCESS *processor,
                                                                           i32 stack_index)
    __asm__("_ZL30AIScriptProcessStateConditionsP7AISYS_sP11nulisthdr_sP11APIOBJECT_s"
            "P10AIPACKET_sP17AISCRIPTPROCESS_sfi.isra.77");

static AICONDITION *AIScriptProcessStateConditions(AISYS *system, NULISTHDR *conditions, AIPACKET *packet,
                                                   AISCRIPTPROCESS *processor, i32 stack_index);

extern "C" {

void AIScriptProcess(AISYS *system, APIOBJECT *object, AIPACKET *packet, AISCRIPTPROCESS *processor, f32 elapsed);

void AIScriptSetLevelPath(char *path);

void *AISysBufferAlloc(VARIPTR *cursor, VARIPTR *buf_end, u32 size);

void AISysCharacterMovement(AISYS *system, AIPACKET *packet, APIOBJECT *object, i32 checks);

void AISysCreatureAntinodeInteraction(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *);

void AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

void AISysCreatureInteraction3D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                    f32 delta_time);

AIAREA *AISysFindArea(AISYS *sys, char *name);

AIPATH *AISysFindPath(AISYS *sys, char *name);

void AISysGetCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, i32 ground);

u32 AISysGetPathColour(i32 index);

i32 AISysGetPathColourCount(void);

void AISysGetPathPos(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks);

void AISysGetPathPosEx(AISYS *system, NUVEC *position, AIPATHINFO *info, AIPATH *path, i32 checks,
                           i32 search_all_paths, f32 *nearest_distance_squared);

void *AISysLoadEx(void *buffer, void *buffer_end, i32 storage_size, void *scene, char *directory, char *name,
                      char *parameter, char *load_directory);

void AISysProcess(AISYS *system, APIOBJECT *player_1, APIOBJECT *player_2);

void AISysProcessCharacter(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed,
                               i32 use_three_dimensions, i32 process_ai);

i32 AISysSetLevelPath(AISYS *system, char *path_name);

void AISysUpdateCharacterPathPos(AISYS *system, APIOBJECT *object, AIPACKET *packet, i32 checks, f32 elapsed);

void AddToAIGroup(AIGROUP *group, APIOBJECT *object);

extern f32 AiRndrCullDistance;
void SetAiRndrCullDistance(f32 distance);

extern AIEDITOR_RENDER_STATE * aieditor;
    void NuRndrLine3dDbg(f32, f32, f32, f32, f32, f32, i32);
void AiRndrLine3d(NURND_VERTEX3D *vertices, NUMTL *, NUMTX *matrix);
void AiRndrLine3dDbg(f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, u32 colour);

void AiSysOnlyUsePakFile(i32 enabled);

void AiSysSetStateDebugee(AISCRIPTPROCESS *processor);

void AiSysUsePackFile(i32 enabled);

    void CalculateLocatorDirection(i32 direction, numtx_s *matrix, NUVEC *out) {
        switch (direction) {
            case 0:
            case 1:
                out->z = 0.0f;
                out->x = 0.0f;
                out->y = 1.0f;
                return;
            case 2:
                out->z = 0.0f;
                out->x = 0.0f;
                out->y = -1.0f;
                return;
            case 3:
                out->x = -1.0f;
                out->y = 0.0f;
                out->z = 0.0f;
                break;
            case 4:
                out->x = 1.0f;
                out->y = 0.0f;
                out->z = 0.0f;
                break;
            case 5:
                out->x = 0.0f;
                out->y = -1.0f;
                out->z = 0.0f;
                break;
            case 6:
                out->x = 0.0f;
                out->y = 1.0f;
                out->z = 0.0f;
                break;
            case 7:
                out->x = 0.0f;
                out->y = 0.0f;
                out->z = 1.0f;
                break;
            case 8:
                out->x = 0.0f;
                out->y = 0.0f;
                out->z = -1.0f;
                break;
            default:
                return;
        }
        NuVecMtxRotate(out, out, matrix);
    }

AIGROUP *CreateAIGroup(AISYS *system, i32 count_across, f32 x_spacing, f32 z_spacing, f32 max_speed);

void DestroyAIGroup(AIGROUP *group);

void FindAIDirectionedRandomPointOnNetwork2D(AIPACKET *packet, NUVEC *position, NUVEC *direction, f32 distance,
                                                 f32 offset, i32 flags, NUVEC *result);

void FollowAPIObject(APIOBJECT *object, APIOBJECT *target, i32 flags, f32 movement_parameter);

    typedef void AIEDITORCALLBACK(void);
    typedef void AIEDITORREBUILDCALLBACK(AISYS *, VARIPTR *, VARIPTR *);

extern AIEDITORCALLBACK * AIPathDeletedFn;
extern AIEDITORPATHNODECALLBACK * AIPathNodeDeletedFn;
extern AIEDITORPATHNODECALLBACK * AIPathNodeMovedFn;
extern AIEDITORREBUILDCALLBACK * GameAISYSRebuildFromEditorDataFn;
extern AIEDITORCALLBACK * GameAISaveFn;

void InitFn_AIPathDeleted(AIEDITORCALLBACK *function);

void InitFn_AIPathNodeDeleted(AIEDITORPATHNODECALLBACK *function);

void InitFn_AIPathNodeMoved(AIEDITORPATHNODECALLBACK *function);

void InitFn_GameAISYSRebuildFromEditorData(AIEDITORREBUILDCALLBACK *function);

void InitFn_GameAISave(AIEDITORCALLBACK *function);

void InitFn_GameParamToFloat(GAMEPARAMTOFLOAT *function);

void InitFn_ScriptProcessFirstTimeAction(SCRIPTPROCESSFIRSTTIMEACTION *function);

void LEGO_AISysCreatureInteraction2D(AISYS *system, i32 object_count, APIOBJECT **objects, i32 *immovable,
                                         f32 delta_time);

void *QueryLocalMessage(void *context, void *message);

} // extern "C"
