#pragma once
#include "unisplice.h"
#include "shadowhook.h"
#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "hook/DlOpen.h"
#include "hook/Mono.h"

namespace UniSplice {

    class Main {
    private:
        static bool _initialized;
        static void* _shadowhook_handle;

        static bool mLoadShadowHook();

    public:
        static void* mono_handle;
        static bool Initialize();
        static void OnMonoLoaded();
    };

} // UniSplice
