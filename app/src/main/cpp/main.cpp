#include <jni.h>
#include "unisplice.h"
#include "unisplice/Main.h"

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("Bootstrapper JNI_OnLoad called!");
    LOGI("Bootstrapper Version: %s", UNISPLICE_BOOTSTRAP_VERSION);

    if (!UniSplice::Main::Initialize()) {
        LOGE("Error in UniSplice Native Bootstrapper!");
    }

    return JNI_VERSION_1_6;
}