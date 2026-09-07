#include "java/android.h"

i32 g_obbMainVersion;
i32 g_obbMainSize;
i32 g_obbPatchVersion;
i32 g_obbPatchSize;
char g_versionName[64];
i32 g_flashAvailable;
char g_internalDataPath[256];
char g_externalDataPath[256];
char g_androidOsVersion[64];
AAssetManager *g_assetManager;
char g_activityName[64];
