#include "java/java.h"
#include "java/android.h"

#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>
#include <pthread.h>
#include <string.h>

#include "batman.h"
#include "gamelib/util/gamelib_util_types.h"
#include "globals.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nu3d/nuscreen.hpp"
#include "nu2api/nucore/NuInputDevice.h"
#include "nu2api/nucore/android/NuInputDevice_android.h"
#include "nu2api/nucore/nucore.hpp"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nuplatform/nudevicespecs.hpp"
#include "nu2api/nuplatform/nuplatform.h"

JavaVM *g_javaVM;
jclass g_activityClass;
extern "C" char g_language[16];
pthread_t g_appPthread;
bool g_appPthreadStarted;

static bool g_isStopped = true;
static bool g_isPaused = true;
static bool g_validSurface;
static i32 android_argc;
static char *android_argv[32];

void *AndroidMain(void *) {
    JNIEnv *env = NULL;
    g_javaVM->AttachCurrentThread(&env, NULL);
    char *package = strstr(g_internalDataPath, "/com.");
    if (package != NULL) {
        char *begin = package + 1;
        char *end = strchr(package + 2, '/');
        if (end != NULL) {
            usize length = end - begin;
            if (length < sizeof(g_activityName)) {
                memcpy(g_activityName, begin, length);
                g_activityName[length] = '\0';
            }
        }
    }
    android_argv[android_argc++] = "app.so";
    g_disallowGlobalNew = true;
    AndroidOBBUtils::InitPackagePaths();
    NuPlatform::Create();
    if (!NuScreen::Exists()) {
        NuScreen::Create();
    }
    g_renderDevice.Initialize();
    g_isLowestEndDevice = 1;
    g_isLowEndDevice = 1;
    g_isMidRangeDevice = 0;
    switch (NuDeviceSpecs::ms_instance->specs) {
        case 1:
            g_isLowestEndDevice = 0;
            g_isLowEndDevice = 0;
            g_isMidRangeDevice = 1;
            break;
        case 2:
        case 3:
            g_isLowestEndDevice = 0;
            g_isLowEndDevice = 0;
            break;
    }
    g_lowEndLevelBehaviour = g_isLowEndDevice;
    return reinterpret_cast<void *>(NuMain(android_argc, android_argv));
}

static inline void UpdateApplicationStatus() {
    NuApplicationState *state = NuCore::GetApplicationState();
    NUAPPLICATIONSTATUS previous = state->GetStatus();
    if (!g_isPaused && !g_isStopped && g_validSurface) {
        state->SetStatus(NUAPPLICATIONSTATUS_IDLE);
    } else {
        // Status 1 blocks presentation; wait for the game thread to acknowledge
        // the transition before Android destroys the window.
        state->SetStatus(NUAPPLICATIONSTATUS_RENDERING);
        if (previous == NUAPPLICATIONSTATUS_IDLE) {
            while (!g_isBlockedInSwapScreen) {
                NuThreadSleep(1);
            }
        }
    }
}

extern "C" {

jint JNI_OnLoad(JavaVM *vm, void *) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    jclass activity = env->FindClass("com/tt/tech/TTActivity");
    g_activityClass = static_cast<jclass>(env->NewGlobalRef(activity));
    return JNI_VERSION_1_6;
}

void Java_com_tt_tech_TTActivity_nativeCacheJNIVars(JNIEnv *env, jobject) {
    env->GetJavaVM(&g_javaVM);
}

void Java_com_tt_tech_TTActivity_nativeSetObbInfo(JNIEnv *env, jobject, jint main_version, jint main_size,
                                               jint patch_version, jint patch_size, jstring version, jint force_etc1) {
    g_obbMainVersion = main_version;
    g_obbMainSize = main_size;
    g_obbPatchVersion = patch_version;
    g_obbPatchSize = patch_size;
    g_forceETC1 = force_etc1;
    const char *text = env->GetStringUTFChars(version, NULL);
    strcpy(g_versionName, text);
    env->ReleaseStringUTFChars(version, text);
}

void Java_com_tt_tech_TTActivity_nativeSetCaps(JNIEnv *, jobject, jint caps) {
    g_flashAvailable = caps;
}

void Java_com_tt_tech_TTActivity_nativeSetPaths(JNIEnv *env, jobject, jstring internal, jstring external) {
    const char *text = env->GetStringUTFChars(internal, NULL);
    strcpy(g_internalDataPath, text);
    env->ReleaseStringUTFChars(internal, text);
    text = env->GetStringUTFChars(external, NULL);
    strcpy(g_externalDataPath, text);
    env->ReleaseStringUTFChars(external, text);
}

void Java_com_tt_tech_TTActivity_nativeSetLanguage(JNIEnv *env, jobject, jstring language) {
    const char *text = env->GetStringUTFChars(language, NULL);
    strcpy(g_language, text);
    env->ReleaseStringUTFChars(language, text);
}

void Java_com_tt_tech_TTActivity_nativeSetAndroidVersion(JNIEnv *env, jobject, jstring version) {
    const char *text = env->GetStringUTFChars(version, NULL);
    strcpy(g_androidOsVersion, text);
    env->ReleaseStringUTFChars(version, text);
}

void Java_com_tt_tech_TTActivity_nativeSetAssetManager(JNIEnv *env, jobject, jobject manager) {
    g_assetManager = AAssetManager_fromJava(env, env->NewGlobalRef(manager));
}

void Java_com_tt_tech_TTActivity_nativeOnCreate(JNIEnv *, jobject) {
    NuCore::GetApplicationState()->SetStatus(NUAPPLICATIONSTATUS_RENDERING);
}

void Java_com_tt_tech_TTActivity_nativeOnStart(JNIEnv *, jobject) {
    g_isStopped = false;
    UpdateApplicationStatus();
    if (!g_appPthreadStarted) {
        pthread_attr_t attributes;
        pthread_attr_init(&attributes);
        pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
        pthread_create(&g_appPthread, &attributes, AndroidMain, NULL);
        g_appPthreadStarted = true;
    }
}

void Java_com_tt_tech_TTActivity_nativeOnResume(JNIEnv *, jobject) {
    g_isPaused = false;
    UpdateApplicationStatus();
}

void Java_com_tt_tech_TTActivity_nativeOnPause(JNIEnv *, jobject) {
    g_isPaused = true;
    UpdateApplicationStatus();
}

void Java_com_tt_tech_TTActivity_nativeOnStop(JNIEnv *, jobject) {
    g_isStopped = true;
    UpdateApplicationStatus();
}

void Java_com_tt_tech_TTActivity_nativeOnKeyUp(JNIEnv *, jobject, jint key) {
    NuInputDevicePS::HandleKeyUp_ANDROID_SPECIFIC(key);
}

void Java_com_tt_tech_TTActivity_nativeOnKeyDown(JNIEnv *, jobject, jint key) {
    NuInputDevicePS::HandleKeyDown_ANDROID_SPECIFIC(key);
}

void Java_com_tt_tech_TTActivity_nativeOnTouchDown(JNIEnv *, jobject, jint device, jint touch, jfloat x, jfloat y) {
    NuInputDevicePS::HandleTouch_ANDROID_SPECIFIC(0, device, touch, x, y);
}

void Java_com_tt_tech_TTActivity_nativeOnTouchUp(JNIEnv *, jobject, jint device, jint touch) {
    NuInputDevicePS::HandleTouch_ANDROID_SPECIFIC(1, device, touch, 0.0f, 0.0f);
}

void Java_com_tt_tech_TTActivity_nativeOnTouchMove(JNIEnv *, jobject, jint device, jint touch, jfloat x, jfloat y) {
    NuInputDevicePS::HandleTouch_ANDROID_SPECIFIC(2, device, touch, x, y);
}

void Java_com_tt_tech_TTActivity_nativeUpdateGamepadAxisValues(JNIEnv *, jobject, jfloat x, jfloat y, jfloat z,
                                                           jfloat rz, jfloat left, jfloat right) {
    NuInputDevicePS::HandleGamePadAxis_ANDROID_SPECIFIC(x, y, z, rz, left, right);
}

void Java_com_tt_tech_CheckGamepadStatus_nativeSetGamePadConnected(JNIEnv *, jobject, jboolean connected) {
    NuInputDevicePS::HandleGamepPadStatusConnect(connected == JNI_TRUE);
}

void Java_com_tt_tech_TTActivity_nativeOnSensorUpdate(JNIEnv *, jobject, jint sensor, jfloat x, jfloat y, jfloat z) {
    NuInputDevicePS::HandleSensor_ANDROID_SPECIFIC(sensor, x, y, z);
}

void Java_com_tt_tech_TTActivity_nativeSetSurface(JNIEnv *env, jobject, jobject surface) {
    ANativeWindow *window = NULL;
    if (surface != NULL) {
        window = ANativeWindow_fromSurface(env, surface);
        i32 width = ANativeWindow_getWidth(window);
        i32 height = ANativeWindow_getHeight(window);
        if (width <= 0 || height <= 0 || width < height) {
            window = NULL;
        }
    }
    if (window != NULL) {
        g_renderDevice.OnWindowCreated(window);
        g_appWindow = window;
        g_validSurface = true;
        UpdateApplicationStatus();
    } else {
        g_validSurface = false;
        UpdateApplicationStatus();
        g_renderDevice.OnWindowDestroy();
    }
}

void Java_com_tt_tech_TTActivity_nativeSetScreenDimesions(JNIEnv *, jobject, jfloat width, jfloat height) {
    NuScreen::Create();
    NuScreen::Get()->SetSceeenDimensions(width, height);
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

} // extern "C"
