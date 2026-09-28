#include "decomp.h"
#include <stddef.h>

#include "java/java.h"
#include "java/android.h"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/android/NuInputDevice_android.h"
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nu3d/nuscreen.hpp"

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

static bool g_isStopped = true;
static bool g_isPaused = true;
static bool g_validSurface;

static void UpdateApplicationStatus() {
    NuApplicationState *state = NuCore::GetApplicationState();
    if (state != NULL)
        state->SetStatus(!g_isPaused && !g_isStopped && g_validSurface ? NUAPPLICATIONSTATUS_IDLE
                                                                       : NUAPPLICATIONSTATUS_RENDERING);
}

extern "C" {

    jint JNI_OnLoad(JavaVM *vm, void *reserved) {
        return JNI_OK;
    }

    void Java_com_tt_tech_CheckGamepadStatus_nativeSetGamePadConnected(JNIEnv *, jobject, jboolean connected) {
        NuInputDevicePS::HandleGamepPadStatusConnect(connected == JNI_TRUE);
    }

    void Java_com_tt_tech_TTActivity_nativeCacheJNIVars(JNIEnv *env, jobject) {
        env->GetJavaVM(&g_javaVM);
    }

    void Java_com_tt_tech_TTActivity_nativeOnCreate(JNIEnv *, jobject) {
        NuApplicationState *state = NuCore::GetApplicationState();
        if (state != NULL)
            state->SetStatus(NUAPPLICATIONSTATUS_RENDERING);
    }

    void Java_com_tt_tech_TTActivity_nativeOnKeyDown(JNIEnv *, jobject, jint key) {
        NuInputDevicePS::HandleKeyDown_ANDROID_SPECIFIC(key);
    }

    void Java_com_tt_tech_TTActivity_nativeOnKeyUp(JNIEnv *, jobject, jint key) {
        NuInputDevicePS::HandleKeyUp_ANDROID_SPECIFIC(key);
    }

    void Java_com_tt_tech_TTActivity_nativeOnPause(JNIEnv *, jobject) {
        g_isPaused = true;
        UpdateApplicationStatus();
    }

    void Java_com_tt_tech_TTActivity_nativeOnResume(JNIEnv *, jobject) {
        g_isPaused = false;
        UpdateApplicationStatus();
    }

    void Java_com_tt_tech_TTActivity_nativeOnSensorUpdate(JNIEnv *, jobject, jint sensor, jfloat x, jfloat y, jfloat z) {
        NuInputDevicePS::HandleSensor_ANDROID_SPECIFIC(sensor, x, y, z);
    }

    void Java_com_tt_tech_TTActivity_nativeOnStart(JNIEnv *, jobject) {
        g_isStopped = false;
        UpdateApplicationStatus();
    }

    void Java_com_tt_tech_TTActivity_nativeOnStop(JNIEnv *, jobject) {
        g_isStopped = true;
        UpdateApplicationStatus();
    }

    void Java_com_tt_tech_TTActivity_nativeOnTouchDown(JNIEnv *, jobject, jint device, jint touch, jfloat x, jfloat y) {
        NuInputDevicePS::HandleTouch_ANDROID_SPECIFIC(0, device, touch, x, y);
    }

    void Java_com_tt_tech_TTActivity_nativeOnTouchMove(JNIEnv *, jobject, jint device, jint touch, jfloat x, jfloat y) {
        NuInputDevicePS::HandleTouch_ANDROID_SPECIFIC(2, device, touch, x, y);
    }

    void Java_com_tt_tech_TTActivity_nativeOnTouchUp(JNIEnv *, jobject, jint device, jint touch) {
        NuInputDevicePS::HandleTouch_ANDROID_SPECIFIC(1, device, touch, 0.0f, 0.0f);
    }

    void Java_com_tt_tech_TTActivity_nativeSetAndroidVersion(JNIEnv *env, jobject, jstring version) {
        const char *text = env->GetStringUTFChars(version, NULL);
        strcpy(g_androidOsVersion, text);
        env->ReleaseStringUTFChars(version, text);
    }

    void Java_com_tt_tech_TTActivity_nativeSetAssetManager(JNIEnv *, jobject, jobject manager) {
        // Host asset access uses filesystem paths; the manager is an opaque token.
        g_assetManager = reinterpret_cast<AAssetManager *>(manager);
    }

    void Java_com_tt_tech_TTActivity_nativeSetCaps(JNIEnv *, jobject, jint caps) {
        g_flashAvailable = caps;
    }

    void Java_com_tt_tech_TTActivity_nativeSetLanguage(JNIEnv *env, jobject, jstring language) {
        const char *text = env->GetStringUTFChars(language, NULL);
        strcpy(g_language, text);
        env->ReleaseStringUTFChars(language, text);
    }

    void Java_com_tt_tech_TTActivity_nativeSetManufacturer(JNIEnv *env, jobject, jstring manufacturer) {
        const char *text = env->GetStringUTFChars(manufacturer, NULL);
        strcpy(g_deviceManufacturer, text);
        env->ReleaseStringUTFChars(manufacturer, text);
    }

    void Java_com_tt_tech_TTActivity_nativeSetModel(JNIEnv *env, jobject, jstring model) {
        const char *text = env->GetStringUTFChars(model, NULL);
        strcpy(g_deviceModel, text);
        env->ReleaseStringUTFChars(model, text);
    }

    void Java_com_tt_tech_TTActivity_nativeSetObbInfo(JNIEnv *env, jobject, jint main_version, jint main_size,
                                                      jint patch_version, jint patch_size, jstring version,
                                                      jint force_etc1) {
        g_obbMainVersion = main_version;
        g_obbMainSize = main_size;
        g_obbPatchVersion = patch_version;
        g_obbPatchSize = patch_size;
        g_forceETC1 = force_etc1;
        const char *text = env->GetStringUTFChars(version, NULL);
        strcpy(g_versionName, text);
        env->ReleaseStringUTFChars(version, text);
    }

    void Java_com_tt_tech_TTActivity_nativeSetPaths(JNIEnv *env, jobject, jstring internal, jstring external) {
        const char *text = env->GetStringUTFChars(internal, NULL);
        strcpy(g_internalDataPath, text);
        env->ReleaseStringUTFChars(internal, text);
        text = env->GetStringUTFChars(external, NULL);
        strcpy(g_externalDataPath, text);
        env->ReleaseStringUTFChars(external, text);
    }

    void Java_com_tt_tech_TTActivity_nativeSetScreenDimesions(JNIEnv *, jobject, jfloat width, jfloat height) {
        if (!NuScreen::Exists())
            NuScreen::Create();
        NuScreen::Get()->SetSceeenDimensions(width, height);
    }

    void Java_com_tt_tech_TTActivity_nativeSetSurface(JNIEnv *, jobject, jobject surface) {
        g_appWindow = reinterpret_cast<ANativeWindow *>(surface);
        g_validSurface = surface != NULL;
        UpdateApplicationStatus();
    }

    void Java_com_tt_tech_TTActivity_nativeUpdateGamepadAxisValues(JNIEnv *, jobject, jfloat x, jfloat y, jfloat z,
                                                                   jfloat rz, jfloat left, jfloat right) {
        NuInputDevicePS::HandleGamePadAxis_ANDROID_SPECIFIC(x, y, z, rz, left, right);
    }

} // extern "C"
