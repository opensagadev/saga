#include "decomp.h"
#include <stddef.h>

#include "java/java.h"
#include "java/android.h"

ANativeWindow *g_appWindow;
i32 g_obbMainVersion;
i32 g_obbMainSize;
i32 g_obbPatchVersion;
i32 g_obbPatchSize;
i32 g_forceETC1;
char g_versionName[64];
i32 g_flashAvailable;
char g_internalDataPath[256];
char g_externalDataPath[256];
char g_deviceManufacturer[256];
char g_deviceModel[256];
extern "C" {
    char g_language[64];
}
char g_androidOsVersion[64];
AAssetManager *g_assetManager;
char g_activityName[64];

static jint AttachCurrentThread(JavaVM *vm, JNIEnv **env, void *args) {
    *env = NULL;

    return JNI_OK;
}

static jint DetachCurrentThread(JavaVM *vm) {
    return JNI_OK;
}

static jint GetEnv(JavaVM *vm, void **env, jint version) {
    *env = NULL;

    return JNI_ERR;
}

struct JNIInvokeInterface stub = {
    .reserved0 = NULL,
    .reserved1 = NULL,
    .reserved2 = NULL,

    .DestroyJavaVM = NULL,
    .AttachCurrentThread = &AttachCurrentThread,
    .DetachCurrentThread = &DetachCurrentThread,
    .GetEnv = &GetEnv,
    .AttachCurrentThreadAsDaemon = NULL,
};

static JavaVM host_javaVM = {
    .functions = &stub,
};

JavaVM *g_javaVM = &host_javaVM;

jclass g_activityClass;

extern "C" {

    jint JNI_OnLoad(JavaVM *vm, void *reserved) {
        return JNI_OK;
    }

    void Java_com_tt_tech_CheckGamepadStatus_nativeSetGamePadConnected(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeCacheJNIVars(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnCreate(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnKeyDown(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnKeyUp(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnPause(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnResume(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnSensorUpdate(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnStart(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnStop(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnTouchDown(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnTouchMove(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeOnTouchUp(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetAndroidVersion(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetAssetManager(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetCaps(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetLanguage(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetManufacturer(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetModel(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetObbInfo(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetPaths(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetScreenDimesions(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeSetSurface(void) {
        STUBBED();
    }

    void Java_com_tt_tech_TTActivity_nativeUpdateGamepadAxisValues(void) {
        STUBBED();
    }

} // extern "C"
