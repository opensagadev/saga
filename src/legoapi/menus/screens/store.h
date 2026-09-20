#pragma once

#include <stddef.h>

#include "decomp.h"
#include "nu2api/numath/nuvec.h"

struct WORLDINFO_s;
struct LEVEL_PROGRESS_s;
struct GameObject_s;
struct MENU_s;
struct DOOR_s;

struct storepack_s {
    char *name;
    u8 field1_0x4;
    u8 field2_0x5;
    u8 field3_0x6;
    u8 field4_0x7;
    union {
        struct {
            u8 field5_0x8;
            u8 field6_0x9;
        };
        i16 message_text_index;
    };
    u8 field7_0xa;
    u8 field8_0xb;
    void (*unlock_fn)();
    char *floor_target_door_name; // 0x10
    char *custodian_locator_set;  // 0x14
    i32 offset_floor_target;      // 0x18
    DOOR_s *floor_target_door;    // 0x1c
    i16 *id;                      // 0x20
    NUVEC custodian_position;     // 0x24
    u16 custodian_angle;          // 0x30
    u8 field44_0x32;              // 0x32, camera socket or 0xff
    u8 field45_0x33;
};
DECOMP_ASSERT(sizeof(storepack_s) == 0x34, "STOREPACK size");
typedef struct storepack_s STOREPACK;
extern STOREPACK StorePack[11];

enum STORE_PACK_INDEX {
    STORE_PACK_EPISODE_II = 0,
    STORE_PACK_EPISODE_III = 1,
    STORE_PACK_EPISODE_IV = 2,
    STORE_PACK_EPISODE_V = 3,
    STORE_PACK_EPISODE_VI = 4,
    STORE_PACK_ARCADE = 5,
    STORE_PACK_BONUS = 6,
    STORE_PACK_BOUNTY = 7,
    STORE_PACK_CHALLENGE = 8,
    STORE_PACK_JEDI = 9,
    STORE_PACK_SITH = 10,
};

typedef struct COLLECTID {
    i16 id;
    u8 type;
    u8 field2_0x3;
    i32 field3_0x4;
    u8 can_buy;
    u8 field5_0x9;
    u16 field6_0xa;
    union {
        char cheat_code[16];
        struct {
            u8 grid_reserved[8];
            f32 grid_x;
            f32 grid_y;
        };
    };
} COLLECTID;

i32 Store_FindPack(i32 id, char *name);
bool Store_IsPackUnlocked(i32 pack);
bool Store_IsPackAvailable(i32 pack, char *reason);
void Store_UnlockPack(i32 pack, bool save);
void Store_RestorePurchases(void);
void StoreBundle_FindByName(char *name);
void Store_HubInitFloorTargets(WORLDINFO_s *world);
void Store_HubDrawFloorTargets(WORLDINFO_s *world);
void Store_RootPackCustodian(i32 pack, GameObject_s *custodian);
void Store_UprootPackCustodian(i32 pack, GameObject_s *custodian);

void MenuUpdateDebugStore(MENU_s *menu);
void MenuDrawDebugStore(MENU_s *menu);
void MenuInitStoreHolding(MENU_s *menu);
void MenuUpdateStoreHolding(MENU_s *menu);
void MenuDrawStoreHolding(MENU_s *menu);
void MenuExitStoreHolding(MENU_s *menu);
void MenuInitStore(MENU_s *menu);
void MenuUpdateStore(MENU_s *menu);
void MenuDrawStore(MENU_s *menu);
void MenuExitStore(MENU_s *menu);
void MenuInitStoreRestoring(MENU_s *menu);
void MenuUpdateStoreRestoring(MENU_s *menu);
void MenuDrawStoreRestoring(MENU_s *menu);
void MenuExitStoreRestoring(MENU_s *menu);
void MenuInitStorePurchase(MENU_s *menu);
void MenuDrawStorePurchase(MENU_s *menu);
void MenuExitStorePurchase(MENU_s *menu);
void MenuUpdateStorePurchase(MENU_s *menu);
