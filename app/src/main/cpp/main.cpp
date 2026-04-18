#include <jni.h>
#include "unisplice.h"
#include "unisplice/Main.h"

extern "C" {

JNIEXPORT void JNICALL
Java_de_unisplice_loader_ModdedActivity_initializeOnUnityCreate(JNIEnv *env, jobject thiz) {
    LOGI("initializeOnUnityCreate called, installing dlopen hook...");
    UniSplice::Main::HookDlOpen();
}

} // extern "C"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("Bootstrapper JNI_OnLoad called!");
    LOGI("Bootstrapper Version: %s", UNISPLICE_BOOTSTRAP_VERSION);

    UniSplice::Main::injected = getenv("UNISPLICE_INJECTED");

    if(UniSplice::Main::injected) {
        LOGI("UniSplice was Injected, not Patched!");
    }

    if (!UniSplice::Main::Initialize()) {
        LOGE("Error in UniSplice Native Bootstrapper!");
    }

    return JNI_VERSION_1_6;
}