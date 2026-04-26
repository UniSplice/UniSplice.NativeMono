#pragma once
#include <android/log.h>

#define UNISPLICE_BOOTSTRAP_VERSION "1.1.2"
#define UNISPLICE_BOOTSTRAP_LOG_TAG "UniSplice.Native"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, UNISPLICE_BOOTSTRAP_LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, UNISPLICE_BOOTSTRAP_LOG_TAG, __VA_ARGS__))